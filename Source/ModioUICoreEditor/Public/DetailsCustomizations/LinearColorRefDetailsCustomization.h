/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Algo/Transform.h"
#include "Brushes/SlateColorBrush.h"
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "Core/LinearColorRef.h"
#include "Core/ModioUIHelpers.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailCustomization.h"
#include "IPropertyUtilities.h"
#include "PropertyHandle.h"
#include "UObject/UnrealNames.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

DECLARE_LOG_CATEGORY_EXTERN(LogProperty, Log, All);

class FLinearColorEditorWidgets
{
public:
	static TSharedRef<SWidget> GenerateColorPresetChoiceWidget(TSharedPtr<FName> ColorName)
	{
		bool bWasResolved = false;
		FLinearColor PreviewColor = ULinearColorRefLibrary::Resolve(FLinearColorRef(*ColorName.Get()), bWasResolved);
		return SNew(SHorizontalBox) +
			   SHorizontalBox::Slot()
				   .HAlign(HAlign_Fill)
				   .VAlign(VAlign_Fill)[SNew(SColorBlock)
											.Color(PreviewColor)
											.AlphaDisplayMode(EColorBlockAlphaDisplayMode::Ignore)
											.Size(FVector2D(64, 16))] +
			   SHorizontalBox::Slot()
				   .Padding(FMargin(8, 0))
				   .HAlign(HAlign_Fill)
				   .VAlign(VAlign_Fill)
				   .AutoWidth()[SNew(STextBlock).Text(FText::FromName(*ColorName.Get()))];
	}

	template<typename SourceType>
	static FLinearColor GetUnderlyingColor(TSharedRef<IPropertyHandle> PropertyHandle)
	{}

	template<typename SourceType>
	static FReply ShowColorPicker(TSharedRef<IPropertyHandle> PropertyHandle)
	{}
};

template<>
FLinearColor FLinearColorEditorWidgets::GetUnderlyingColor<FLinearColorRef>(TSharedRef<IPropertyHandle> PropertyHandle)
{
	FStructProperty* UnderlyingStruct = CastField<FStructProperty>(PropertyHandle->GetProperty());

	if (UnderlyingStruct)
	{
		void* DataPointer;
		PropertyHandle->GetValueData(DataPointer);
		if (DataPointer)
		{
			FLinearColorRef* ActualColorReference = static_cast<FLinearColorRef*>(DataPointer);
			bool bWasResolved = false;
			return ULinearColorRefLibrary::Resolve(*ActualColorReference, bWasResolved);
		}
	}
	return FLinearColor(0.25, 0.25, 0.25, 1);
}

class FLinearColorRefDetailsCustomization : public IPropertyTypeCustomization
{
	TArray<TSharedPtr<FName>> ColorPresetNames;
	TSharedPtr<IPropertyHandle> UnderlyingColorRefPropHandle;

public:
	FLinearColorRefDetailsCustomization() {

	};

	void UpdateColorPresetList()
	{
		ColorPresetNames.Empty();

		Algo::Transform(ULinearColorRefLibrary::GetValidColorKeys(), ColorPresetNames,
						[](const FName PresetName) { return MakeShared<FName>(PresetName); });
	}

	void UpdateUnderlyingColor(TSharedPtr<FName> ColorName, ESelectInfo::Type)
	{
		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(UnderlyingColorRefPropHandle->GetProperty());

		if (ColorName.IsValid() && UnderlyingStruct)
		{
			UnderlyingColorRefPropHandle->NotifyPreChange();
			void* DataPointer;
			UnderlyingColorRefPropHandle->GetValueData(DataPointer);
			if (DataPointer)
			{
				FLinearColorRef* ActualColorReference = static_cast<FLinearColorRef*>(DataPointer);
				ActualColorReference->ReferencedColorName = *ColorName.Get();
				UnderlyingColorRefPropHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
			}
		}
	}

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		UnderlyingColorRefPropHandle = PropertyHandle;
		TSharedPtr<SWidget> NameWidget = nullptr;
		TSharedPtr<IPropertyHandle> ParentHandle = PropertyHandle->GetParentHandle();
		// Allows more compact display of a ColorRef inside an override
		if (ParentHandle && PropertyHandle->HasMetaData("ShowOnlyInnerProperties"))
		{
			NameWidget = PropertyHandle->CreatePropertyNameWidget(ParentHandle->GetPropertyDisplayName());
		}
		else
		{
			NameWidget = PropertyHandle->CreatePropertyNameWidget();
		}
		NameWidget->SetEnabled(true);
		HeaderRow.NameContent()[NameWidget.ToSharedRef()];

		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(PropertyHandle->GetProperty());

		if (UnderlyingStruct)
		{
			TSharedPtr<SWidget> ReferencedColorNameWidget = SNullWidget::NullWidget;
			TSharedPtr<IPropertyHandle> ReferencedColorNamePropHandle =
				PropertyHandle->GetChildHandle("ReferencedColorName");
			if (ReferencedColorNamePropHandle)
			{
				FNameProperty* ReferencedColorNameProperty =
					CastField<FNameProperty>(ReferencedColorNamePropHandle->GetProperty());
				if (ReferencedColorNameProperty)
				{
					ReferencedColorNameWidget = ReferencedColorNamePropHandle->CreatePropertyValueWidget(false);
					ReferencedColorNameWidget->SetEnabled(false);
				}
				else
				{
					UE_LOG(LogTemp, Warning,
						   TEXT("LinearColorRef missing ReferencedColorName property or incorrect type"));
				}
			}
			// clang-format off
			HeaderRow.ValueContent()
				[
					SNew(SHorizontalBox) 
					+SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center)
					[
						SNew(SVerticalBox)
						+SVerticalBox::Slot().MaxHeight(16.f).VAlign(VAlign_Center)
						[
							SNew(SHorizontalBox) 
							+SHorizontalBox::Slot().MaxWidth(64.f)
							[
								SNew(SColorBlock)
								.Color_Static(FLinearColorEditorWidgets::GetUnderlyingColor<FLinearColorRef>, PropertyHandle)
								.Size(FVector2D(64, 16))
							]
							+SHorizontalBox::Slot().MaxWidth(64.f)
							[
								SNew(SColorBlock)
								.Color_Static(FLinearColorEditorWidgets::GetUnderlyingColor<FLinearColorRef>, PropertyHandle)
								.AlphaDisplayMode(EColorBlockAlphaDisplayMode::Ignore)
								.Size(FVector2D(64,16))
							]
						]
					]
					+SHorizontalBox::Slot().AutoWidth().Padding(FMargin(8,0,8,0))
					[
						ReferencedColorNameWidget.ToSharedRef()
					]
					+SHorizontalBox::Slot().AutoWidth().Padding(FMargin(8,0,8,0))
					[
						SNew(SComboBox<TSharedPtr<FName>>)
						.Content()
						[
							SNew(STextBlock).Text(FText::FromString("Color Presets"))
						]
						.OnComboBoxOpening(FOnComboBoxOpening::CreateSP(this, &FLinearColorRefDetailsCustomization::UpdateColorPresetList))
						.OptionsSource(&ColorPresetNames)
						.OnSelectionChanged(SComboBox<TSharedPtr<FName>>::FOnSelectionChanged::CreateSP(this, &FLinearColorRefDetailsCustomization::UpdateUnderlyingColor))
						.OnGenerateWidget(SComboBox<TSharedPtr<FName>>::FOnGenerateWidget::CreateStatic(&FLinearColorEditorWidgets::GenerateColorPresetChoiceWidget))
					]
				];
			// clang-format on
		}
		else
		{
			HeaderRow.ValueContent()[SNew(SColorBlock)];
		}
	}

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		ChildBuilder.AddProperty(PropertyHandle->GetChildHandle("FallbackColor").ToSharedRef());
	};
};
