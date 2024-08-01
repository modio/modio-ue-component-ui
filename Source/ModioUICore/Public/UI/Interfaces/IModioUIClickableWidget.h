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

#include "IModioUIClickableWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FModioClickableOnClicked, UObject*, ClickContext);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioClickableOnClickedMulticast, UObject*, ClickContext);

/**
 * @brief Interface indicating the implementing widget can emit click events that can be enabled/disabled
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Clickable", Meta = (DocGroup = "UI", Comment = "@brief Interface indicating the implementing widget can emit click events that can be enabled/disabled"))
class MODIOUICORE_API UModioUIClickableWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIClickableWidget : public IInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Registers a delegate to receive callbacks when the implementing object is clicked
	 * @param Handler Delegate to invoke on a click event
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Clickable")
	void AddClickedHandler(const FModioClickableOnClicked& Handler);

	/**
	 * @brief Unregisters a delegate so it will no longer receive callbacks when the implementing widget is clicked
	 * @param Handler Delegate to unsubscribe
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Clickable")
	void RemoveClickedHandler(const FModioClickableOnClicked& Handler);

	/**
	 * @brief Enables click events being emitted by the implementing object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Clickable")
	void EnableClick();

	/**
	 * @brief Disables click events being emitted by the implementing object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Clickable")
	void DisableClick();
};
