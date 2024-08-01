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
#include "Engine/DataTable.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UObject/Interface.h"

#include "IModioUIBoundActionWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FModioBoundActionProgressed, FDataTableRowHandle, BoundAction, float, Progress);
DECLARE_DYNAMIC_DELEGATE_OneParam(FModioBoundActionCompleted, FDataTableRowHandle, BoundAction);

/**
 * @brief Interface indicating the implementing widget has a bound action that can be clicked or held
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Bound Action Widget", Meta = (DocGroup = "UI", Comment = "@brief Interface indicating the implementing widget has a bound action that can be clicked or held"))
class MODIOUICORE_API UModioUIBoundActionWidget : public UModioUIClickableWidget
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIBoundActionWidget : public IModioUIClickableWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Registers a delegate to receive progress callbacks when the implementing object's bound action is held
	 * @param InputActionRow Delegate to invoke with hold progress
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Bound Action Widget")
	void SetBoundAction(const FDataTableRowHandle& InputActionRow);

	/**
	 * @brief Tries to get the bound action for the implementing object
	 * @param InputActionRow The bound action if it exists
	 * @return True if the bound action exists, false otherwise
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Bound Action Widget")
	bool TryGetBoundAction(FDataTableRowHandle& InputActionRow) const;

	/**
	 * @brief Clears the bound action for the implementing object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Bound Action Widget")
	void ClearBoundAction();

	/**
	 * Registers a delegate to receive progress callbacks when the implementing object's bound action is held
	 * @param Handler Delegate to invoke with hold progress
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Bound Action Widget")
	void AddActionProgressHandler(const FModioBoundActionProgressed& Handler);

	/**
	 * Unregisters a delegate so it will no longer receive callbacks when the implementing widget's bound action is held
	 * @param Handler Delegate to unsubscribe
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Bound Action Widget")
	void RemoveActionProgressHandler(const FModioBoundActionProgressed& Handler);

	/**
	 * Registers a delegate to receive callbacks when the implementing object's bound action is held for the full duration
	 * @param Handler Delegate to invoke on a hold completed event
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Bound Action Widget")
	void AddActionCompletedHandler(const FModioBoundActionCompleted& Handler);

	/**
	 * Unregisters a delegate so it will no longer receive callbacks when the implementing widget's bound action is held
	 * @param Handler Delegate to unsubscribe
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Bound Action Widget")
	void RemoveActionCompletedHandler(const FModioBoundActionCompleted& Handler);
};
