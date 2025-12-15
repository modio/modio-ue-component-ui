/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once
#include "Widgets/SCompoundWidget.h"
#include "UI/Interfaces/IModioUITextValidator.h"
#include "SModioUITextValidatorTestWidget.generated.h"

UCLASS()
class UModioUITextValidatorBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_OneParam(FBridgedDelegate, bool);

	UFUNCTION()
	void HandleValidation(bool bIsValid)
	{
		OnTextValidated.ExecuteIfBound(bIsValid);
	}

public:
	FBridgedDelegate OnTextValidated;
};

class SModioUITextValidatorTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUITextValidatorTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;
	TSharedPtr<SEditableTextBox> InputText;
	TSharedPtr<STextBlock> OutputText;

	TStrongObjectPtr<UModioUITextValidatorBridge> DelegateBridge;
	FModioOnTextValidationCompleted TextValidationHandler;

	void HandleValidation(bool bIsValid)
	{
		bTextValid = bIsValid;
	}
	bool bHandlerAdded = false;
	bool bTextValid = false;
	FText GetHandlerButtonLabel() const
	{
		return FText::FromString(bHandlerAdded ? "Remove Text Validation Handler" : "Add Text Validation Handler");
	}

	void ToggleHandler();

	void Construct(const FArguments& InArgs);
};