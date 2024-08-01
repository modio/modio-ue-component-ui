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
#include "Core/ModioCommonButtonParams.h"
#include "Delegates/DelegateCombinations.h"
#include "UI/Interfaces/IModioUIBoundActionWidget.h"
#include "UObject/Interface.h"

#include "IModioUICommonButtonWidget.generated.h"

/**
 * @brief Interface indicating the implementing widget can be configured like a CommonButton
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Common Button Widget", Meta = (DocGroup = "UI", Comment = "@brief Interface indicating the implementing widget can be configured like a CommonButton"))
class MODIOUICORE_API UModioUICommonButtonWidget : public UModioUIBoundActionWidget
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUICommonButtonWidget : public IModioUIBoundActionWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief Configures the implementing object as a CommonButton
	 * @param Params The parameters to configure the button with
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Common Button Widget")
	void ConfigureCommonButton(FModioCommonButtonParams Params);
};
