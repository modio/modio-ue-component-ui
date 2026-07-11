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
#include "UI/Interfaces/IModioUICommandMenu.h"
#include "SModioUICommandMenuTestWidget.generated.h"

UCLASS()
class UModioUICommandMenuBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_TwoParams(FBridgedDelegate, TArray<UObject*>&, UObject*);

	UFUNCTION()
	void HandleCommentListBuild(TArray<UObject*>& CommandList, UObject* Context)
	{
		OnBuild.ExecuteIfBound(CommandList, Context);
	}

public:
	FBridgedDelegate OnBuild;
};

class SModioUICommandMenuTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUICommandMenuTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUICommandMenuBridge> DelegateBridge;

	FModioCommandListBuilder CommandListBuilderHandler;
	bool bBuilt;

	void HandleCommentListBuild(TArray<UObject*>& CommandList, UObject* Context)
	{
		bBuilt = true;
	}
	bool bHandlerAdded = false;
	FText GetHandlerButtonLabel() const
	{
		return FText::FromString(bHandlerAdded ? "Remove Command List Builder" : "Add Command List Builder");
	}

	void ToggleHandler();

	void Construct(const FArguments& InArgs);
};