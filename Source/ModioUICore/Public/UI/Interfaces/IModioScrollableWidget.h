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
};