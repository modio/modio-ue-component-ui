/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Types/ModioErrorCode.h"
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUIModTagSelector.h"

#include "ModioFilterPanel.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioFilterPanel : public UModioUIComponentBase,
										  public IModioUIClickableWidget,
										  public IModioUIModTagSelector
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Filter Panel|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetClearButtonWidget() const;
	TScriptInterface<IModioUIClickableWidget> GetClearButtonWidget_Implementation() const
	{
		return nullptr;
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Filter Panel|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetClosePanelButtonWidget() const;
	TScriptInterface<IModioUIClickableWidget> GetClosePanelButtonWidget_Implementation() const
	{
		return nullptr;
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Filter Panel|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetBackgroundButtonWidget() const;
	TScriptInterface<IModioUIClickableWidget> GetBackgroundButtonWidget_Implementation() const
	{
		return nullptr;
	}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Filter Panel|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIModTagSelector> GetTagSelectorWidget() const;
	TScriptInterface<IModioUIModTagSelector> GetTagSelectorWidget_Implementation() const
	{
		return nullptr;
	}

	virtual void NativeOnInitialized() override;
	void SetModTagOptions(FModioErrorCode ErrorCode, TOptional<FModioModTagOptions> ModTagOptions);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Filter Panel", meta = (BlueprintProtected))
	TArray<FString> GetSelectedTags();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Filter Panel", meta = (BlueprintProtected))
	void ClearSelectedTags();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Filter Panel",
			  meta = (BlueprintProtected))
	void AddTagSelectionChangedHandler(const FModioOnTagSelectionChanged& Handler);
};
