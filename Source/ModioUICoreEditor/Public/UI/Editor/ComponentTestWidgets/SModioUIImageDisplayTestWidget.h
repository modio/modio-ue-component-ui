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
#include "UI/Interfaces/IModioUIImageDisplayWidget.h"
#include "SModioUIImageDisplayTestWidget.generated.h"


UCLASS()
class UModioUIImageDisplayBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_TwoParams(FBridgedDelegate, UObject*, bool);

	UFUNCTION()
	void HandleImageLoaded(UObject* LoadContext, bool bLoadSuccess)
	{
		OnValuechanged.ExecuteIfBound(LoadContext, bLoadSuccess);
	}

public:
	FBridgedDelegate OnValuechanged;
};

class SModioUIImageDisplayTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIImageDisplayTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUIImageDisplayBridge> DelegateBridge;

	FModioImageDisplayOnLoadStateChanged ImageLoadedListener;

	FVector2D DesiredBrushSize = {32, 32};

	TOptional<bool> ImageLoadStatus;

	virtual void HandleImageLoaded(UObject* LoadContext, bool bLoadSuccess)
	{
		ImageLoadStatus = bLoadSuccess;
	}

	void Construct(const FArguments& InArgs);
};
