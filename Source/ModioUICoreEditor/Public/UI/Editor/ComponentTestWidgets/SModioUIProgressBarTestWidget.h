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
#include "UI/Interfaces/IModioUIProgressWidget.h"
#include "SModioUIProgressBarTestWidget.generated.h"


UCLASS()
class UModioUIProgressBarBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_OneParam(FBridgedDelegate, float);

	UFUNCTION()
	void HandleValueChanged(float NewValue)
	{
		OnValuechanged.ExecuteIfBound(NewValue);
	}

public:
	FBridgedDelegate OnValuechanged;
};

class SModioUIProgressBarTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIProgressBarTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUIProgressBarBridge> DelegateBridge;

	TSharedPtr<SNumericEntryBox<float>> DesiredIndex;

	float DesiredFloatValue;

	FModioProgressWidgetValueChanged ValueChangedListener;
	float CurrentValue = 0.1f;

	bool bValueChanged = false;
	virtual void HandleValueChanged(float NewValue)
	{
		CurrentValue = NewValue;
		bValueChanged = true;
	}

	void Construct(const FArguments& InArgs);
};
