/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Editor/ComponentTestWidgets/SModioUITextValidatorTestWidget.h"
#include "UI/Editor/ModioWeakThis.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SModioUITextValidatorTestWidget)

void SModioUITextValidatorTestWidget::ToggleHandler()
{
	if (TargetObject.IsValid())
	{
		if (bHandlerAdded)
		{
			IModioUITextValidator::Execute_RemoveTextValidationCompletedHandler(TargetObject.Get(), TextValidationHandler);
		}
		else
		{
			IModioUITextValidator::Execute_AddTextValidationCompletedHandler(TargetObject.Get(), TextValidationHandler);
		}
		bHandlerAdded = !bHandlerAdded;
	}
}

void SModioUITextValidatorTestWidget::Construct(const FArguments& InArgs)
{
	TargetObject = InArgs._TargetObject.Get();
	DelegateBridge = TStrongObjectPtr<UModioUITextValidatorBridge>(NewObject<UModioUITextValidatorBridge>());
	DelegateBridge->OnTextValidated.BindSP(this, &SModioUITextValidatorTestWidget::HandleValidation);
	TextValidationHandler.BindUFunction(DelegateBridge.Get(), FName("HandleValidation"));

	// clang-format off
	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SNew(SButton)
			.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
			.Text_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return GetHandlerButtonLabel();} return FText{};})
			.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ToggleHandler();} return FReply::Handled();})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SAssignNew(InputText, SEditableTextBox)
			.MinDesiredWidth(200)
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SNew(SButton)
			.IsEnabled_Lambda([this](){return (TargetObject != nullptr) && bHandlerAdded;})
			.Text(FText::FromString("Validate Text"))
			.OnClicked_Lambda([this]() {
				FText ValidationResult;
				IModioUITextValidator::Execute_ValidateText(TargetObject.Get(), InputText->GetText(), ValidationResult);
				OutputText->SetText(ValidationResult);
				return FReply::Handled();
			})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SAssignNew(OutputText, STextBlock)
			.MinDesiredWidth(200)
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(EVerticalAlignment::VAlign_Center)
		.Padding(8,0)
		[
			SNew(STextBlock)
			.Text_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){return FText::FromString(bTextValid ? "Valid!": "Invalid");} return FText{};})
		]

	];
	// clang-format on
};
