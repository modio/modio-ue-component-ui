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

#include "Components/SlateWrapperTypes.h"
#include "Delegates/DelegateCombinations.h"
#include "UObject/Interface.h"

#include "IModioUISelectableWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FModioSelectableOnSelectionChanged, UObject*, SelectionContext, bool, bNewSelectedState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioSelectableOnSelectionChangedMulticast, UObject*, SelectionContext, bool, bNewSelectedState);

/**
 * @brief Interface for widgets that have a selection state 
*/
UINTERFACE(BlueprintType, Category = "mod.io|UI|Selectable", Meta = (DocGroup = "UI", Comment = "@brief Interface for widgets that have a selection state"))
class MODIOUICORE_API UModioUISelectableWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUISelectableWidget : public IInterface
{
	GENERATED_BODY()
public:
	/**
	 * @brief Registers a delegate that receives a callback when the selected state of the widget changes
	 * @param Handler Delegate to invoke on state change
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Selectable")
	void AddSelectedStateChangedHandler(const FModioSelectableOnSelectionChanged& Handler);

	/**
	 * @brief Unsubscribes a delegate from receiving state change callbacks
	 * @param Handler The delegate to remove
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Selectable")
	void RemoveSelectedStateChangedHandler(const FModioSelectableOnSelectionChanged& Handler);

	/**
	 * @brief Sets the selectable state of the implementing widget
	 * @param bNewSelectableState Whether the widget should accept selection or not
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Selectable")
	void SetSelectable(bool bNewSelectableState);

	/**
	 * @brief Retrieves the selectable state of the implementing widget
	 * @param bIsSelectable True if the implementing widget can be selected
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Selectable")
	void GetSelectable(bool& bIsSelectable);

	/**
	 * @brief Directly sets the selected state of the implementing widget
	 * @param bNewSelectedState The new selection state
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Selectable")
	void SetSelectedState(bool bNewSelectedState);

	/**
	 * @brief Toggles the selected state of the implementing widget
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Selectable")
	void ToggleSelectedState();

	/**
	 * @brief Retrieves the current selection state of the implementing widget
	 * @param bCurrentSelectedState True if the widget is currently selected
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Selectable")
	void GetSelectedState(bool& bCurrentSelectedState);

	/**
	 * @brief Enables or disables toggle-ability on the implementing widget
	 * @param bNewToggleableState True to enable the widget's selection state being toggled
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Selectable")
	void SetToggleable(bool bNewToggleableState);

	/**
	 * @brief Retrieves the toggle-able state of the implementing widget
	 * @param bIsToggleable True if the widget currently can have its selection state toggled
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Selectable")
	void GetToggleable(bool& bIsToggleable);
};
