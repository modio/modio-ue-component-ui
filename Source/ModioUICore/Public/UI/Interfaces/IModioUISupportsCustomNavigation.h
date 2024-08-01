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

#include "Blueprint/WidgetNavigation.h"
#include "UObject/Interface.h"

#include "IModioUISupportsCustomNavigation.generated.h"


/**
 * @brief Interface indicating that the implementing widget can query a custom delegate to provide navigation logic
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Navigation", Meta = (DocGroup = "UI", Comment = "@brief Interface indicating that the implementing widget can query a custom delegate to provide navigation logic"))
class MODIOUICORE_API UModioUISupportsCustomNavigation : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUISupportsCustomNavigation : public IInterface
{
	GENERATED_BODY()
public:
	/**
	 * @brief Sets a custom delegate to determine what should be focused during navigation
	 * @param ContentNavigationDelegate
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|IModioUISupportsCustomNavigation")
	void SetNavigationDelegate(uint8 Context, const FCustomWidgetNavigationDelegate& ContentNavigationDelegate);
};
