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
#include "UI/Interfaces/IModioUIObjectSelector.h"
#include "SModioUIObjectSelectorTestWidget.generated.h"

UCLASS()
class UModioUIObjectSelectorBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_OneParam(FBridgedDelegate, UObject*);

	UFUNCTION()
	void HandleSelectionChanged(UObject* NewSelection)
	{
		OnSelectionChanged.ExecuteIfBound(NewSelection);
	}

public:
	FBridgedDelegate OnSelectionChanged;
};

class SModioUIObjectSelectorTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIObjectSelectorTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUIObjectSelectorBridge> DelegateBridge;

	TSharedPtr<SNumericEntryBox<int32>> DesiredIndex;

	int32 DesiredIndexValue;

	FModioOnObjectSelectionChanged SelectionListener;
	bool bSelected;

	virtual void HandleSelectionChanged(UObject* NewSelection)
	{
		bSelected = (NewSelection != nullptr);
	}
	void ClearSelection();
	void SetSelectionFromIndex();
	void Construct(const FArguments& InArgs);
};
