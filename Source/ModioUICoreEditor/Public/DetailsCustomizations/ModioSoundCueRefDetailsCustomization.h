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
#include "Core/ModioSoundCueRef.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"
#include "UObject/UnrealNames.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"

class FModioSoundCueRefEditorWidgets
{
public:
	static TSharedRef<SWidget> GenerateSoundPresetChoiceWidget(TSharedPtr<FName> SoundName)
	{
		return SNew(STextBlock).Text(FText::FromName(SoundName.IsValid() ? *SoundName.Get() : NAME_None));
	}
};

class FModioSoundCueRefDetailsCustomization : public IPropertyTypeCustomization
{
	TArray<TSharedPtr<FName>> SoundPresetNames;
	TSharedPtr<IPropertyHandle> UnderlyingSoundRefPropHandle;

public:
	FModioSoundCueRefDetailsCustomization() {};

	void UpdateSoundPresetList()
	{
		SoundPresetNames.Empty();

		Algo::Transform(UModioSoundCueRefLibrary::GetValidSoundKeys(), SoundPresetNames,
						[](const FName PresetName) { return MakeShared<FName>(PresetName); });
	}

	void UpdateUnderlyingSound(TSharedPtr<FName> SoundName, ESelectInfo::Type)
	{
		FStructProperty* UnderlyingStruct = CastField<FStructProperty>(UnderlyingSoundRefPropHandle->GetProperty());

		if (SoundName.IsValid() && UnderlyingStruct)
		{
			UnderlyingSoundRefPropHandle->NotifyPreChange();
			void* DataPointer;
			UnderlyingSoundRefPropHandle->GetValueData(DataPointer);
			if (DataPointer)
			{
				FModioSoundCueRef* ActualSoundReference = static_cast<FModioSoundCueRef*>(DataPointer);
				ActualSoundReference->ReferencedSoundName = *SoundName.Get();
				UnderlyingSoundRefPropHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
			}
		}
	}

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		UnderlyingSoundRefPropHandle = PropertyHandle;
		TSharedPtr<SWidget> NameWidget = nullptr;
		TSharedPtr<IPropertyHandle> ParentHandle = PropertyHandle->GetParentHandle();
		// Allows more compact display of a SoundCueRef inside an override
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
			TSharedPtr<SWidget> ReferencedSoundNameWidget = SNullWidget::NullWidget;
			TSharedPtr<IPropertyHandle> ReferencedSoundNamePropHandle =
				PropertyHandle->GetChildHandle("ReferencedSoundName");
			if (ReferencedSoundNamePropHandle)
			{
				FNameProperty* ReferencedSoundNameProperty =
					CastField<FNameProperty>(ReferencedSoundNamePropHandle->GetProperty());
				if (ReferencedSoundNameProperty)
				{
					ReferencedSoundNameWidget = ReferencedSoundNamePropHandle->CreatePropertyValueWidget(false);
					ReferencedSoundNameWidget->SetEnabled(false);
				}
				else
				{
					UE_LOG(LogTemp, Warning,
						   TEXT("ModioSoundCueRef missing ReferencedSoundName property or incorrect type"));
				}
			}
			// clang-format off
			HeaderRow.ValueContent()
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot().AutoWidth().Padding(FMargin(0,0,8,0)).VAlign(VAlign_Center)
					[
						ReferencedSoundNameWidget.ToSharedRef()
					]
					+SHorizontalBox::Slot().AutoWidth().Padding(FMargin(8,0,8,0))
					[
						SNew(SComboBox<TSharedPtr<FName>>)
						.Content()
						[
							SNew(STextBlock).Text(FText::FromString("Sound Presets"))
						]
						.OnComboBoxOpening(FOnComboBoxOpening::CreateSP(this, &FModioSoundCueRefDetailsCustomization::UpdateSoundPresetList))
						.OptionsSource(&SoundPresetNames)
						.OnSelectionChanged(SComboBox<TSharedPtr<FName>>::FOnSelectionChanged::CreateSP(this, &FModioSoundCueRefDetailsCustomization::UpdateUnderlyingSound))
						.OnGenerateWidget(SComboBox<TSharedPtr<FName>>::FOnGenerateWidget::CreateStatic(&FModioSoundCueRefEditorWidgets::GenerateSoundPresetChoiceWidget))
					]
				];
			// clang-format on
		}
	}

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		ChildBuilder.AddProperty(PropertyHandle->GetChildHandle("FallbackSound").ToSharedRef());
	};
};
