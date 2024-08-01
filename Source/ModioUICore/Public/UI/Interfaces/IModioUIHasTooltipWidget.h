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

#include "IModioUIHasTooltipWidget.generated.h"

/**
 * @brief Interface indicating the implementing widget has a configurable tooltip
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Tooltip", Meta = (DocGroup = "UI", Comment = "@brief Interface indicating the implementing widget has a configurable tooltip"))
class MODIOUICORE_API UModioUIHasTooltipWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIHasTooltipWidget : public IInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets whether a tooltip is enabled on the implementing object
	 * @param bNewEnabledState True if tooltips should be enabled
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|HasTooltip")
	void SetTooltipEnabledState(bool bNewEnabledState);

	/**
	 * @brief Configures the values for the associated tooltip for the implementing object
	 * @param TitleText The title of the tooltip
	 * @param InfoText The contents of the tooltip
	 * @param TagText The tagline of the tooltip
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|HasTooltip")
	void ConfigureTooltip(const FText& TitleText, const FText& InfoText, const FText& TagText);
};
