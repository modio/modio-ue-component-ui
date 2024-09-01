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

	virtual UWidget* GetWidgetToFocus_Implementation(EUINavigation NavigationType) const
	{
		return NativeGetWidgetToFocus(NavigationType);
	}

public:
	/**
	 * @brief Queries the implementing object for the widget that should receive focus
	 * @param NavigationType Navigation type to focus, if applicable
	 * @return Widget to focus
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Focusable")
	UWidget* GetWidgetToFocus(EUINavigation NavigationType) const;
};