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
#include "UObject/Interface.h"
#include "IModioScrollableWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FModioScrollableOffsetChanged, UObject*, ScrollableContext);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioScrollableOffsetChangedMulticast, UObject*, ScrollableContext);

/**
 * @brief Interface for widgets that support scrolling functionality
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Scrollable",
	Meta = (DocGroup = "UI", Comment =
		"@brief Interface for widgets that support scrolling functionality"))
class UModioScrollableWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioScrollableWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Scroll the widget to the top
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Scrollable")
	void ScrollToTop();

	/**
	 * @brief Scroll the widget to the bottom
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Scrollable")
	void ScrollToBottom();

	/**
	 * @brief Set the scroll offset of the widget
	 * @param Offset The scroll offset to set
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Scrollable")
	void SetScrollOffset(float Offset);

	/**
	 * @brief Get the current scroll offset of the widget
	 * @return The current scroll offset
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Scrollable")
	float GetScrollOffset() const;

	/**
	 * @brief Scroll the widget by the size of the visible content area
	 * @param bBackward If true, scroll up/left; if false, scroll down/right
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Scrollable")
	void ScrollVisibleArea(bool bBackward);

	/**
	 * @brief Checks if the widget can scroll in the specified direction
	 * @param bBackward If true, check if can scroll up/left; if false check if can scroll down/right
	 * @return True if the widget can scroll in the specified direction, false otherwise
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Scrollable")
	bool CanScrollInDirection(bool bBackward) const; 
	
	/**
	 * @brief Registers a delegate that receives a callback when the scroll offset of the widget changes
	 * @param Handler Delegate to invoke on state change
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Selectable")
	void AddScrollableOffsetChangedHandler(const FModioScrollableOffsetChanged& Handler);

	/**
	 * @brief Unsubscribes a delegate from receiving state change callbacks
	 * @param Handler The delegate to remove
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Selectable")
	void RemoveScrollableOffsetChangedHandler(const FModioScrollableOffsetChanged& Handler);
};