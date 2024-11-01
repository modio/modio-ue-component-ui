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
#include "IModioToggleableWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FModioOnToggleStateChanged, UObject*, Context, bool, bIsOpen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioOnToggleStateChangedMulticast, UObject*, Context, bool, bIsOpen);

/**
 * @brief Interface for widgets that can be toggled open or closed
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Toggleable",
	Meta = (DocGroup = "UI", Comment =
		"@brief Interface for widgets that can be toggled open or closed"))
class UModioToggleableWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioToggleableWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Toggles the open state of the widget
	 * @param bForce Whether to force the widget to open (e.g. it may take ShouldOpenDueToClick into account when the implementing widget is derived from UMenuAnchor)
	 * @param bFocusMenu Whether to focus the menu when opening
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Toggleable")
	void Open(bool bForce, bool bFocusMenu);

	/**
	 * @brief Toggles the close state of the widget
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Toggleable")
	void Close();
	
	/**
	 * @brief Get the current open/close state of the widget
	 * @return True if the widget is open, false otherwise
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Toggleable")
	bool IsOpen() const;

	/**
	 * @brief Registers a delegate to receive callbacks when the toggle state changes
	 * @param Handler Delegate to invoke on toggle state change
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Toggleable")
	void AddToggleStateChangedHandler(const FModioOnToggleStateChanged& Handler);

	/**
	 * @brief Unregisters a delegate from receiving callbacks when the toggle state changes
	 * @param Handler Delegate to remove from toggle state change
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Toggleable")
	void RemoveToggleStateChangedHandler(const FModioOnToggleStateChanged& Handler);
};