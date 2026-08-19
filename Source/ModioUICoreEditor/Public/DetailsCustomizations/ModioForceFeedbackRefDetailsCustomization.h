/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Algo/Transform.h"
#include "Containers/Array.h"
#include "Core/ModioForceFeedbackRef.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"
#include "UObject/UnrealNames.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

class FModioForceFeedbackRefEditorWidgets
{
public:
	static TSharedRef<SWidget> GenerateForceFeedbackPresetChoiceWidget(TSharedPtr<FName> ForceFeedbackName)
	{
		return SNew(STextBlock)
			.Text(FText::FromName(ForceFeedbackName.IsValid() ? *ForceFeedbackName.Get() : NAME_None));
	}
};

class FModioForceFeedbackRefDetailsCustomization : public IPropertyTypeCustomization
{
	TArray<TSharedPtr<FName>> ForceFeedbackPresetNames;
	TSharedPtr<IPropertyHandle> UnderlyingForceFeedbackRefPropHandle;

public:
	FModioForceFeedbackRefDetailsCustomization() {};

	void UpdateForceFeedbackPresetList()
	{
		ForceFeedbackPresetNames.Empty();

		Algo::Transform(UModioForceFeedbackRefLibrary::GetValidForceFeedbackKeys(), ForceFeedbackPresetNames,
						[](const FName PresetName) { return MakeShared<FName>(PresetName); });
	}

	void UpdateUnderlyingForceFeedback(TSharedPtr<FName> ForceFeedbackName, ESelectInfo::Type)
	{
		FStructProperty* UnderlyingStruct =
			CastField<FStructProperty>(UnderlyingForceFeedbackRefPropHandle->GetProperty());

		if (ForceFeedbackName.IsValid() && UnderlyingStruct)
		{
			UnderlyingForceFeedbackRefPropHandle->NotifyPreChange();
			void* DataPointer;
			UnderlyingForceFeedbackRefPropHandle->GetValueData(DataPointer);
			if (DataPointer)
			{
				FModioForceFeedbackRef* ActualForceFeedbackReference = static_cast<FModioForceFeedbackRef*>(DataPointer);
				ActualForceFeedbackReference->ReferencedForceFeedbackName = *ForceFeedbackName.Get();
				UnderlyingForceFeedbackRefPropHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
			}
		}
	}

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		UnderlyingForceFeedbackRefPropHandle = PropertyHandle;
		TSharedPtr<SWidget> NameWidget = nullptr;
		TSharedPtr<IPropertyHandle> ParentHandle = PropertyHandle->GetParentHandle();
		// Allows more compact display of a ForceFeedbackRef inside an override
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
			TSharedPtr<SWidget> ReferencedForceFeedbackNameWidget = SNullWidget::NullWidget;
			TSharedPtr<IPropertyHandle> ReferencedForceFeedbackNamePropHandle =
				PropertyHandle->GetChildHandle("ReferencedForceFeedbackName");
			if (ReferencedForceFeedbackNamePropHandle)
			{
				FNameProperty* ReferencedForceFeedbackNameProperty =
					CastField<FNameProperty>(ReferencedForceFeedbackNamePropHandle->GetProperty());
				if (ReferencedForceFeedbackNameProperty)
				{
					ReferencedForceFeedbackNameWidget =
						ReferencedForceFeedbackNamePropHandle->CreatePropertyValueWidget(false);
					ReferencedForceFeedbackNameWidget->SetEnabled(false);
				}
				else
				{
					UE_LOG(LogTemp, Warning,
						   TEXT("ModioForceFeedbackRef missing ReferencedForceFeedbackName property or incorrect type"));
				}
			}
			// clang-format off
			HeaderRow.ValueContent()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot().AutoWidth().Padding(FMargin(0,0,8,0)).VAlign(VAlign_Center)
					[
						ReferencedForceFeedbackNameWidget.ToSharedRef()
					]
					+SHorizontalBox::Slot().AutoWidth().Padding(FMargin(8,0,8,0))
					[
						SNew(SComboBox<TSharedPtr<FName>>)
						.Content()
						[
							SNew(STextBlock).Text(FText::FromString("Force Feedback Presets"))
						]
						.OnComboBoxOpening(FOnComboBoxOpening::CreateSP(this, &FModioForceFeedbackRefDetailsCustomization::UpdateForceFeedbackPresetList))
						.OptionsSource(&ForceFeedbackPresetNames)
						.OnSelectionChanged(SComboBox<TSharedPtr<FName>>::FOnSelectionChanged::CreateSP(this, &FModioForceFeedbackRefDetailsCustomization::UpdateUnderlyingForceFeedback))
						.OnGenerateWidget(SComboBox<TSharedPtr<FName>>::FOnGenerateWidget::CreateStatic(&FModioForceFeedbackRefEditorWidgets::GenerateForceFeedbackPresetChoiceWidget))
					]
				];
			// clang-format on
		}
	}

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		ChildBuilder.AddProperty(PropertyHandle->GetChildHandle("FallbackForceFeedback").ToSharedRef());
	};
};
