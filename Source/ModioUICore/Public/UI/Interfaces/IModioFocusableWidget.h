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
#include "Components/Widget.h"

#include "IModioFocusableWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FModioOnFocusPathChanged, UObject*, FocusContext, FFocusEvent, InFocusEvent, bool, bIsFocused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FModioOnFocusPathChangedMulticast, UObject*, FocusContext, FFocusEvent, InFocusEvent, bool, bIsFocused);

/**
 * @brief Interface indicating the implementing widget (or it's children) can receive focus
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Focusable",
	Meta = (DocGroup = "UI", Comment =
		"@brief Interface indicating the implementing widget (or it's children) can receive focus"))
class UModioFocusableWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioFocusableWidget
{
	GENERATED_BODY()

protected:
	virtual UWidget* NativeGetWidgetToFocus(EUINavigation NavigationType) const
	{
		return nullptr;
	}
	UWidget* GetWidgetToFocus_Implementation(EUINavigation NavigationType) const
	{
		return NativeGetWidgetToFocus(NavigationType);
	}

	virtual void NativeAddFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler) {}
	void AddFocusPathChangedHandler_Implementation(const FModioOnFocusPathChanged& Handler)
	{
		NativeAddFocusPathChangedHandler(Handler);
	}

	virtual void NativeRemoveFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler) {}
	void RemoveFocusPathChangedHandler_Implementation(const FModioOnFocusPathChanged& Handler)
	{
		NativeRemoveFocusPathChangedHandler(Handler);
	}

public:
	/**
	 * @brief Queries the implementing object for the widget that should receive focus
	 * @param NavigationType Navigation type to focus, if applicable
	 * @return Widget to focus
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Focusable")
	UWidget* GetWidgetToFocus(EUINavigation NavigationType) const;

	/**
	 * @brief Registers a delegate to receive callbacks when the focus path changes
	 * The delegate is assumed to be invoked both when the implementing widget gains focus and when children of the widget gain focus
	 * @param Handler Delegate to invoke on focus path change
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Focusable")
	void AddFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler);

	/**
	 * @brief Unregisters a delegate from receiving callbacks when the focus path changes
	 * @param Handler Delegate to remove from focus path change
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Focusable")
	void RemoveFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler);
};