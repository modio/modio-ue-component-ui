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

#include "IModioUIHoverableWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FModioHoverableOnHoverStateChanged, UObject*, HoverContext, bool, bHoveredState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioHoverableOnHoverStateChangedMulticast, UObject*, HoverContext, bool, bHoveredState);


/**
 * @brief Interface indicating the implementing widget can emit hover state change events
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Hoverable", Meta = (DocGroup = "UI", Comment = "@brief Interface indicating the implementing widget can emit hover state change events"))
class MODIOUICORE_API UModioUIHoverableWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIHoverableWidget : public IInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Register a delegate to receive callbacks when the hover state of the implementing object changes
	 * @param Handler Delegate to invoke on hover state change
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Hoverable")
	void AddHoverStateChangedHandler(const FModioHoverableOnHoverStateChanged& Handler);

	/**
	 * @brief Unregisters a delegate, preventing it from receiving hover state change callbacks
	 * @param Handler Delegate to unregister
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Hoverable")
	void RemoveHoverStateChangedHandler(const FModioHoverableOnHoverStateChanged& Handler);

	/**
	 * @brief Enable emitting hover state changed events on the implementing object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Hoverable")
	void EnableHoverEvents();

	/**
	 * @brief Disable emitting hover state changed events on the implementing object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Hoverable")
	void DisableHoverEvents();
};
