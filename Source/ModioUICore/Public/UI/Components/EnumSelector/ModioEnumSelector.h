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
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUIObjectSelector.h"

#include "ModioEnumSelector.generated.h"

/**
 * @default_impl_for Enum Selector
 * @brief The Enum Selector is a default implementation of the Enum Selector component. This default implementation uses
 * an internal sub-component as a menu.
 * @component_display_name Enum Selector
 */
UCLASS()
class MODIOUICORE_API UModioEnumSelector : public UModioUIComponentBase, public IModioUIObjectSelector
{
	GENERATED_BODY()
protected:
	IMPLEMENT_MODIO_WIDGET_GETTER_VALIDATION(GetSelectorMenuWidget(), GetMenuToggleClickableWidget());

	/**
	 * @brief Passes `this` as SelectedValue
	 * @default_component_event FModioOnObjectSelectionChanged
	 */
	UPROPERTY()
	FModioOnObjectSelectionChangedMulticast OnSelectionChanged;

	/**
	 * @brief Cached values to use as the source for data binding. Populated by SetValues. This also prevents the GC from collecting the values while they are in use
	 */
	UPROPERTY(Transient, meta = (BlueprintProtected))
	TArray<TObjectPtr<UObject>> CachedValues;

	/**
	 * @brief Provides a reference to the internal widget that provides a menu users can select an enum value from. The
	 * default implementation will forward IModioUIObjectSelector messages to this widget if it is valid
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Enum Selector|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIObjectSelector> GetSelectorMenuWidget() const;

	/**
	 * @brief Provides a reference to the internal widget that will toggle the display of the menu to allow users to make a selection
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Enum Selector|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetMenuToggleClickableWidget() const;

	virtual void SetValues_Implementation(const TArray<UObject*>& InValues) override;

	virtual UObject* GetSingleSelectedValue_Implementation() override;

	virtual void ClearSelectedValues_Implementation() override;

	virtual void SetSingleSelectionByIndex_Implementation(int32 Index, bool bEmitSelectionEvent) override;

	virtual void SetSingleSelectionByValue_Implementation(UObject* Value, bool bEmitSelectionEvent) override;

	virtual void AddSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler) override;

	virtual void RemoveSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler) override;

	UFUNCTION(meta = (BlueprintProtected))
	void InternalSelectionChangedHandler(UObject* SelectedObject);

	UFUNCTION(BlueprintCallable, meta = (BlueprintProtected), Category = "mod.io|UI|EnumSelector")
	void NotifySelectionChanged(UObject* SelectedObject);

	virtual void NativePreConstruct() override;

	virtual void SetSelectedStateForIndex_Implementation(int32 Index, bool bNewSelectionState,
	                                                     bool bEmitSelectionEvent) override;

	virtual void SetSelectedStateForValue_Implementation(UObject* Value, bool bNewSelectionState,
	                                                     bool bEmitSelectionEvent) override;

	virtual TArray<UObject*> GetSelectedValues_Implementation() override;

	virtual void SetMultiSelectionAllowed_Implementation(bool bMultiSelectionAllowed) override;

	virtual bool GetMultiSelectionAllowed_Implementation() override;
};
