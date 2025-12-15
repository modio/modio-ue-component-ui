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
#include "UI/Interfaces/IModioUIHoverableWidget.h"
#include "SModioUIHoverableTestWidget.generated.h"

UCLASS()
class UModioUIHoverableBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_TwoParams(FBridgedDelegate, UObject*, bool);

	UFUNCTION()
	void HandleHovered(UObject* HoverContext, bool bNewState)
	{
		OnHoverStateChanged.ExecuteIfBound(HoverContext, bNewState);
	}

public:
	FBridgedDelegate OnHoverStateChanged;
};

class SModioUIHoverableTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIHoverableTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUIHoverableBridge> DelegateBridge;

	FModioHoverableOnHoverStateChanged HoverListener;
	bool bHovered;

	void HandleHovered(UObject* ClickContext, bool bNewState)
	{
		bHovered = bNewState;
	}
	bool bHandlerAdded = false;
	bool bHoverEnabled = true;
	FText GetHandlerButtonLabel() const
	{
		return FText::FromString(bHandlerAdded ? "Remove Hover Handler" : "Add Hover Handler");
	}
	FText GetClickEventButtonLabel() const
	{
		return FText::FromString(bHoverEnabled ? "Disable Hover Events" : "Enable Hover Events");
	}

	void ToggleHandler();

	void ToggleEvents();

	void Construct(const FArguments& InArgs);
};
