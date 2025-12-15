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
#include "UI/Interfaces/IModioUISelectableWidget.h"
#include "SModioUISelectableTestWidget.generated.h"


UCLASS()
class UModioUISelectableBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_TwoParams(FBridgedDelegate, UObject*, bool);

	UFUNCTION()
	void HandleSelectionChanged(UObject* SelectionContext, bool bNewState)
	{
		OnSelectionChanged.ExecuteIfBound(SelectionContext, bNewState);
	}

public:
	FBridgedDelegate OnSelectionChanged;
};

class SModioUISelectableTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUISelectableTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUISelectableBridge> DelegateBridge;

	FModioSelectableOnSelectionChanged OnSelectionChanged;
	bool bSelected;

	void HandleSelectionChanged(UObject* SelectionContext, bool bNewState)
	{
		bSelected = bNewState;
	}
	void ResetSelected();
	bool bHandlerAdded = false;
	bool bSelectionEnabled = true;
	bool bToggleableEnabled = true;
	FText GetHandlerButtonLabel() const
	{
		return FText::FromString(bHandlerAdded ? "Remove Selection Handler" : "Add Selection Handler");
	}
	FText GetSelectionEventButtonLabel() const
	{
		return FText::FromString(bSelectionEnabled ? "Disable Selectable" : "Enable Selectable");
	}
	FText GetToggleableButtonLabel() const
	{
		return FText::FromString(bToggleableEnabled ? "Disable Toggleable" : "Enable Toggleable");
	}
	void ToggleHandler();

	void ToggleEvents();
	void ToggleToggleable();
	void Construct(const FArguments& InArgs);
};