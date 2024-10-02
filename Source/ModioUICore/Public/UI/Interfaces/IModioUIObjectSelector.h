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

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Templates/SubclassOf.h"
#include "UObject/Interface.h"

#include "IModioUIObjectSelector.generated.h"

class UWidget;

DECLARE_DYNAMIC_DELEGATE_OneParam(FModioOnObjectSelectionChanged, UObject*, SelectedValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioOnObjectSelectionChangedMulticast, UObject*, SelectedValue);

/**
 * @brief Interface class for any widget that allows a choice between objects
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|ObjectSelector",
		   Meta = (DocGroup = "UI",
				   Comment = "@brief Interface class for any widget that allows a choice between objects"))

class MODIOUICORE_API UModioUIObjectSelector : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIObjectSelector
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the set of available choices that the user can select between
	 * @param InValues Array of UObjects representing choices
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ObjectSelector")
	void SetValues(const TArray<UObject*>& InValues);

	/**
	 * @brief Retrieves the selection of the implementing widget
	 * @return UObject associated with the current selection of the implementing widget
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ObjectSelector")
	UObject* GetSingleSelectedValue();

	/**
	 * @brief Gets the number of entries in the implementing widget
	 * @return The number of entries in the implementing widget
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ObjectSelector")
	int32 GetNumEntries();

	/**
	 * @brief Gets the current selection for the implementing widget by index
	 * @return The 0-based Index of the current selection of the implementing widget
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ObjectSelector")
	int32 GetSingleSelectionIndex();

	/**
	 * @brief Clears the selection state on the implementing widget
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ObjectSelector")
	void ClearSelectedValues();

	/**
	 * @brief Sets the current selection for the implementing widget by index
	 * @param Index The 0-based index to use when determining the selected object
	 * @param bEmitSelectionEvent True if a selection changed event should be emitted
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ObjectSelector")
	void SetSingleSelectionByIndex(int32 Index, bool bEmitSelectionEvent);

	/**
	 * @brief Sets the current selection for the implementing widget by specifying the associated value
	 * @param Value The value to select
	 * @param bEmitSelectionEvent True if a selection changed event should be emitted
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ObjectSelector")
	void SetSingleSelectionByValue(UObject* Value, bool bEmitSelectionEvent);

	/**
	 * @brief Registers a delegate to receive callbacks when the selection state of the implementing widget changes
	 * @param Handler Delegate to invoke on selection state change
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|ObjectSelector")
	void AddSelectionChangedHandler(const FModioOnObjectSelectionChanged& Handler);

	/**
	 * @brief Unregisters a delegate so it will not receive callbacks when selection state changes
	 * @param Handler Delegate that will no longer receive callbacks
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|ObjectSelector")
	void RemoveSelectionChangedHandler(const FModioOnObjectSelectionChanged& Handler);

	/**
	 * @brief Sets the selected state for the specified index
	 * @param Index The 0-based index to set the selection state for
	 * @param bNewSelectionState The new selection state to set
	 * @param bEmitSelectionEvent True if a selection changed event should be emitted
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Object Selector")
	void SetSelectedStateForIndex(int32 Index, bool bNewSelectionState, bool bEmitSelectionEvent);

	/**
	 * @brief Sets the selected state for the specified value
	 * @param Value The value to set the selection state for
	 * @param bNewSelectionState The new selection state to set
	 * @param bEmitSelectionEvent True if a selection changed event should be emitted
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Object Selector")
	void SetSelectedStateForValue(UObject* Value, bool bNewSelectionState, bool bEmitSelectionEvent);

	/**
	 * @brief Retrieves the selected values from the implementing widget
	 * @return Array of UObjects representing the selected values
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Object Selector")
	TArray<UObject*> GetSelectedValues();

	/**
	 * @brief Sets whether the implementing widget should allow multiple selections
	 * @param bMultiSelectionAllowed True if multiple selections are allowed
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Object Selector")
	void SetMultiSelectionAllowed(bool bMultiSelectionAllowed);

	/**
	 * @brief Retrieves whether the implementing widget allows multiple selections
	 * @return True if multiple selections are allowed
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Object Selector")
	bool GetMultiSelectionAllowed();

	/**
	 * @brief Sets the class to use for list entries in the implementing widget
	 * @param InNewEntryClass The class to use for list entries
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Object Selector")
	void SetListEntryWidgetClass(TSubclassOf<UWidget> InNewEntryClass);

	/**
	 * @brief Retrieves the index of the specified value
	 * @param Value The value to retrieve the index for
	 * @return The 0-based index of the specified value
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Object Selector")
	int32 GetIndexForValue(UObject* Value) const;

	/**
	 * @brief Retrieves the widget associated with the specified value
	 * @param Value The value to retrieve the widget for
	 * @return The widget associated with the specified value
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Object Selector")
	UUserWidget* GetWidgetForValue(UObject* Value) const;
};
