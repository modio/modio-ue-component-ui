/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Editor/ComponentTestWidgets/SModioUIProgressBarTestWidget.h"

#include "UI/Editor/SModioToggleButton.h"
#include "Widgets/Input/SNumericEntryBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SModioUIProgressBarTestWidget)

void SModioUIProgressBarTestWidget::Construct(const FArguments& InArgs)
	{
		TargetObject = InArgs._TargetObject.Get();
		DelegateBridge = TStrongObjectPtr<UModioUIProgressBarBridge>(NewObject<UModioUIProgressBarBridge>());
		DelegateBridge->OnValuechanged.BindSP(this, &SModioUIProgressBarTestWidget::HandleValueChanged);
		ValueChangedListener.BindUFunction(DelegateBridge.Get(), FName("HandleValueChanged"));

		IModioUIProgressWidget::Execute_AddValueChangedHandler(TargetObject.Get(), ValueChangedListener);

		// clang-format off
		ChildSlot
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SModioToggleButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.bInitialState(true)
				.TrueText(FText::FromString("Disable Value Changed Handler"))
				.FalseText(FText::FromString("Enable Value Changed Handler"))
				.ClickedWhenTrue_Lambda([this](){IModioUIProgressWidget::Execute_RemoveValueChangedHandler(TargetObject.Get(), ValueChangedListener);})
				.ClickedWhenFalse_Lambda([this](){IModioUIProgressWidget::Execute_AddValueChangedHandler(TargetObject.Get(), ValueChangedListener);})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SModioToggleButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.TrueText(FText::FromString("Disable Marquee"))
				.FalseText(FText::FromString("Enable Marquee"))
				.ClickedWhenTrue_Lambda([this](){IModioUIProgressWidget::Execute_SetMarquee(TargetObject.Get(), false);})
				.ClickedWhenFalse_Lambda([this](){IModioUIProgressWidget::Execute_SetMarquee(TargetObject.Get(), true);})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Set Progress"))
				.OnClicked_Lambda([this](){IModioUIProgressWidget::Execute_SetProgress(TargetObject.Get(), DesiredFloatValue);return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SAssignNew(DesiredIndex, SNumericEntryBox<float>)
				.Value_Lambda([this](){return DesiredFloatValue;})
				.OnValueChanged_Lambda([this](float NewValue){DesiredFloatValue = NewValue;})
				.MinValue(0)
				.MaxValue(1)
				.MinSliderValue(0)
				.MaxSliderValue(1)
			]
			
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(STextBlock)
					.Text_Lambda([this]() {
					if (TargetObject.IsValid())
					{
						return FText::FromString("Value: " + FString::SanitizeFloat(IModioUIProgressWidget::Execute_GetProgress(TargetObject.Get())));
					}
					else
					{
						return FText::FromString("No Target Object");
					}
					})
				
			]+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Reset Value Changed"))
				.OnClicked_Lambda([this](){bValueChanged = false; return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(STextBlock)
					.Text_Lambda([this]() {
						return FText::FromString(bValueChanged? "Value Changed" : "Value not Changed");
					})
				
			]
		];
		// clang-format on
	};