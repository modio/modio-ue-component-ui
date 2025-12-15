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
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "SModioUIClickableTestWidget.generated.h"

UCLASS()
class UModioUIClickableBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_OneParam(FBridgedDelegate, UObject*);

	UFUNCTION()
	void HandleClick(UObject* ClickContext)
	{
		OnClicked.ExecuteIfBound(ClickContext);
	}

public:
	FBridgedDelegate OnClicked;
};

class SModioUIClickableTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIClickableTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUIClickableBridge> DelegateBridge;

	FModioClickableOnClicked ClickListener;
	bool bClicked;

	void HandleClick(UObject* ClickContext)
	{
		bClicked = true;
	}
	void ResetClick()
	{
		bClicked = false;
	}
	bool bHandlerAdded = false;
	bool bClickEnabled = true;
	FText GetHandlerButtonLabel() const
	{
		return FText::FromString(bHandlerAdded ? "Remove Click Handler" : "Add Click Handler");
	}
	FText GetClickEventButtonLabel() const
	{
		return FText::FromString(bClickEnabled ? "Disable Click Events" : "Enable Click Events");
	}

	void ToggleHandler();

	void ToggleEvents();

	void Construct(const FArguments& InArgs);
};