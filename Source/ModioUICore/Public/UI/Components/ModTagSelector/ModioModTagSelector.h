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

#include "Core/ModioTagOptionsUI.h"
#include "CoreMinimal.h"
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUIModTagSelector.h"

#include "ModioModTagSelector.generated.h"

/**
 * @default_impl_for Mod Tag Selector
 * @brief A widget that allows the user to select tags for a mod
 * @component_display_name Mod Tag Selector
 */
UCLASS()
class MODIOUICORE_API UModioModTagSelector : public UModioUIComponentBase, public IModioUIModTagSelector
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Cached reference to the tag options that are currently available
	 */
	UPROPERTY(Transient, meta = (BlueprintProtected))
	TScriptInterface<UModioModTagOptionsUIDetails> CachedTagOptions;

	/**
	 * @brief Provides a reference to the internal widget that displays the tag categories and values and tracks the user's selection
	 * @return The widget that displays the tag categories and values
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<UModioUIModTagSelector> GetSelectorMenuWidget() const;

	/**
	 * @brief Provides a reference to the internal widget that can be clicked to toggle the display of the selector menu
	 * @return The widget that can be clicked to toggle the display of the selector menu
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<UModioUIClickableWidget> GetMenuToggleClickableWidget() const;

	//~ Begin IModioUIModTagSelector Interface
	virtual void SetAvailableTagsFromModTagOptions_Implementation(const FModioModTagOptions& InOptions) override;
	virtual void SetAvailableTagsFromBoundModTagOptions_Implementation(const TScriptInterface<UModioModTagOptionsUIDetails>& InOptions) override;
	virtual TArray<FString> GetSelectedTags_Implementation() override;
	virtual void ClearSelectedTags_Implementation() override;
	virtual void AddTagSelectionChangedHandler_Implementation(const FModioOnTagSelectionChanged& Handler) override;
	virtual void RemoveTagSelectionChangedHandler_Implementation(const FModioOnTagSelectionChanged& Handler) override;
	//~ End IModioUIModTagSelector Interface
};
