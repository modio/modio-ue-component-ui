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

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/Interfaces/IModioModFilterUIDetails.h"
#include "UI/Interfaces/IModioUIObjectSelector.h"

#include "ModioPresetFilterSelector.generated.h"

/**
 * @default_impl_for Preset Filter Selector
 * @brief The Preset Filter Selector is a default implementation of the Preset Filter Selector component. This
 * implementation uses an internal sub-component to display the available presets.
 * @component_display_name Preset Filter Selector
 */
UCLASS()
class MODIOUICORE_API UModioPresetFilterSelector : public UModioUIComponentBase, public IModioUIObjectSelector
{
	GENERATED_BODY()
protected:

	//~ Begin IModioUIObjectSelector Interface
	virtual void SetValues_Implementation(const TArray<UObject*>& InValues) override;
	virtual int32 GetNumEntries_Implementation() override;
	virtual UObject* GetSingleSelectedValue_Implementation() override;
	virtual int32 GetSingleSelectionIndex_Implementation() override;
	virtual void ClearSelectedValues_Implementation() override;
	virtual void SetSingleSelectionByIndex_Implementation(int32 Index, bool bEmitSelectionEvent) override;
	virtual void SetSingleSelectionByValue_Implementation(UObject* Value, bool bEmitSelectionEvent) override;
	virtual void AddSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler) override;
	virtual void RemoveSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler) override;
	virtual void SetSelectedStateForIndex_Implementation(int32 Index, bool bNewSelectionState,
												 bool bEmitSelectionEvent) override;
	virtual void SetSelectedStateForValue_Implementation(UObject* Value, bool bNewSelectionState,
												 bool bEmitSelectionEvent) override;
	virtual TArray<UObject*> GetSelectedValues_Implementation() override;
	virtual void SetMultiSelectionAllowed_Implementation(bool bMultiSelectionAllowed) override;
	virtual bool GetMultiSelectionAllowed_Implementation() override;
	//~ End IModioUIObjectSelector Interface

	virtual void NativePreConstruct() override;
	virtual TArray<TScriptInterface<IModioModFilterUIDetails>> GetFilterPresets_Implementation();

	/**
	 * @brief Gets a reference to the widget responsible for displaying and selecting the set of available presets. The default implementation
	 * will forward IModioUIObjectSelector messages to this widget if it is valid
	 * @return The widget that displays the presets
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Preset Filter Selector|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIObjectSelector> GetPresetListWidget() const;

	
	/**
	 * @brief Fetches the available filter presets that the selector should present to users
	 * @return Array of UObjects implementing UModioModFilterUIDetails, one for each preset
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Preset Filter Selector",
			  meta = (BlueprintProtected))
	TArray<TScriptInterface<IModioModFilterUIDetails>> GetFilterPresets();

	/**
	 * @brief Strong ref to bound presets to prevent GC if the concrete implementation of this component does not hold a ref to them
	 */
	UPROPERTY(meta = (BlueprintProtected))
	TArray<TObjectPtr<UObject>> CachedBoundPresets;
};
