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
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/Interfaces/IModioUIModTagSelector.h"
#include "UI/Interfaces/IModioUIObjectListWidget.h"

#include "ModioModTagSelectorMenu.generated.h"

class UModioModTagOptionsUIDetails;

/**
 * @default_impl_for Mod Tag Selector Menu
 * @brief A widget that allows the user to select tags from a list of available tags
 * @component_display_name Mod Tag Selector Menu
 */
UCLASS()
class MODIOUICORE_API UModioModTagSelectorMenu : public UModioUIComponentBase, public IModioUIModTagSelector
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "mod.io|UI|TagSelectorMenu")
	bool bShowHiddenTags = true;

	UPROPERTY(EditDefaultsOnly, Category = "mod.io|UI|TagSelectorMenu")
	bool bShowLockedTags = true;

	/**
	 * @default_component_event FModioOnTagSelectionChanged
	 */
	UPROPERTY()
	FModioOnTagSelectionChangedMulticast OnTagSelectionChanged;

	UPROPERTY(Transient, meta = (BlueprintProtected))
	TScriptInterface<UModioModTagOptionsUIDetails> CachedTagOptions;

	/**
	 * @brief Provides a reference to the internal widget that actually displays the tag categories
	 * @return The widget that displays the tag categories
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIObjectListWidget> GetCategoryContainerWidget() const;

	/**
	 * @brief Notify listeners that the selection state of the tags has changed
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Events|Tag Selector", meta = (BlueprintProtected))
	void NotifySelectionChanged();

	/**
	 * @brief Called when a category widget is created. Adds selection change handlers to the category widget
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "mod.io|UI|Tag Selector", meta = (BlueprintProtected))
	void OnCategoryWidgetCreated(UWidget* CategoryWidget, UObject* CategoryDataSource);

	/**
	 * @brief Called when a category entry is selected. Notifies listeners of the selection change
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "mod.io|UI|Tag Selector", meta = (BlueprintProtected))
	void OnCategoryEntrySelectionChanged(UObject* NewlySelectedEntry);

	/**
	 * @brief Filters through category tags based on their hidden/locked flags to compare with our local preferences if they should be shown
	 * @return Array of category tags allowed to be shown
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Tag Selector")
	TArray<TScriptInterface<UModioModTagCategoryUIDetails>> GetAllowedTags();

	//~ Begin IModioUIModTagSelector Interface
	virtual void SetAvailableTagsFromModTagOptions_Implementation(const FModioModTagOptions& InOptions) override;
	virtual void SetAvailableTagsFromBoundModTagOptions_Implementation(const TScriptInterface<UModioModTagOptionsUIDetails>& InOptions) override;
	virtual TArray<FString> GetSelectedTags_Implementation() override;
	virtual void ClearSelectedTags_Implementation() override;
	virtual void AddTagSelectionChangedHandler_Implementation(const FModioOnTagSelectionChanged& Handler) override;
	virtual void RemoveTagSelectionChangedHandler_Implementation(const FModioOnTagSelectionChanged& Handler) override;

	virtual bool GetAllowHiddenTags_Implementation() override;
	virtual bool GetAllowLockedTags_Implementation() override;
	//~ End IModioUIModTagSelector Interface

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface
};
