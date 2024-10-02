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
#include "UI/Interfaces/IModioUIObjectSelector.h"

#include "ModioEnumSelectorMenu.generated.h"

/**
 * @default_impl_for Enum Selector
 * @brief The Enum Selector is a default implementation of the Enum Selector component. This default implementation uses
 * an internal sub-component to display and manage selection of individual enum values.
 * @component_display_name Enum Selector Menu
 */
UCLASS()
class MODIOUICORE_API UModioEnumSelectorMenu : public UModioUIComponentBase, public IModioUIObjectSelector
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Passes `UObject*` as SelectedValue
	 * @default_component_event FModioOnObjectSelectionChanged
	 */
	UPROPERTY()
	FModioOnObjectSelectionChangedMulticast OnSelectionChanged;

	/**
	 * @brief Cached values to use as the source for data binding. Populated by SetValues. This also prevents the GC from collecting the values while they are in use
	 */
	UPROPERTY(Transient, meta = (BlueprintProtected))
	TArray<UObject*> CachedValues;

	UFUNCTION(meta = (BlueprintProtected))
	void InternalSelectionChangedHandler(UObject* SelectedObject);

	/**
	 * @brief Called when the selection changes
	 * @param SelectedObject The object that was selected
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = "mod.io|UI|EnumSelectorMenu")
	void NotifySelectionChanged(UObject* SelectedObject);

	/**
	 * @brief Provides a reference to the internal widget that actually displays the enum values and tracks the user's
	 * selection. The default implementation will forward IModioUIObjectSelector messages to this widget if it is valid
	 * @return The selector widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Enum Selector Menu|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIObjectSelector> GetSelectorWidget() const;

	//~ Begin IModioUIObjectSelector Interface
	virtual void SetValues_Implementation(const TArray<UObject*>& InValues) override;
	virtual UObject* GetSingleSelectedValue_Implementation() override;
	virtual void ClearSelectedValues_Implementation() override;
	virtual void SetSingleSelectionByIndex_Implementation(int32 Index, bool bEmitSelectionEvent) override;
	virtual void SetSingleSelectionByValue_Implementation(UObject* Value, bool bEmitSelectionEvent) override;
	virtual void AddSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler) override;
	virtual void RemoveSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler) override;
	virtual void SetSelectedStateForIndex_Implementation(int32 Index, bool bNewSelectionState, bool bEmitSelectionEvent) override;
	virtual void SetSelectedStateForValue_Implementation(UObject* Value, bool bNewSelectionState, bool bEmitSelectionEvent) override;
	virtual TArray<UObject*> GetSelectedValues_Implementation() override;
	virtual void SetMultiSelectionAllowed_Implementation(bool bMultiSelectionAllowed) override;
	virtual bool GetMultiSelectionAllowed_Implementation() override;
	//~ End IModioUIObjectSelector Interface

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface
};
