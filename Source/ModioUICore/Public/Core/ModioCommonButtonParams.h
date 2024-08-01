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
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Core/ModioPropertyOverrides.h"

#include "ModioCommonButtonParams.generated.h"

/**
 * @brief Struct representing either a spacer or a common button - primarily intended for use in shortcut widgets
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|CommonButtonParams")
struct MODIOUICORE_API FModioCommonButtonParams
{
	GENERATED_BODY();

	/**
	 * @brief Whether this button is a spacer or a common button
	 */
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "mod.io|UI|CommonButtonParams")
	bool bIsSpacer = false;

	/**
	 * @brief Width and height of the button - only used if a spacer
	 */
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "mod.io|UI|CommonButtonParams|Appearance", meta = (EditConditionHides, EditCondition = "bIsSpacer", ClampMin = "0"))
	int32 Width = 32;

	/**
	 * @brief Width and height of the button - only used if a spacer
	 */
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "mod.io|UI|CommonButtonParams|Appearance", meta = (EditConditionHides, EditCondition = "bIsSpacer", ClampMin = "0"))
	int32 Height = 32;

	/**
	 * @brief Description of the role of the button - only used if not a spacer, but a button
	 */
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "mod.io|UI|CommonButtonParams|Appearance", meta = (EditConditionHides, EditCondition = "!bIsSpacer"))
	FTextOverride ButtonRoleDescription;

	/**
	 * @brief Minimum width of the button - only used if not a spacer, but a button
	 */
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "mod.io|UI|CommonButtonParams|Appearance", meta = (EditConditionHides, EditCondition = "!bIsSpacer", ClampMin = "0"))
	int32 MinWidth = 32;

	/**
	 * @brief Minimum height of the button - only used if not a spacer, but a button
	 */
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "mod.io|UI|CommonButtonParams|Appearance", meta = (EditConditionHides, EditCondition = "!bIsSpacer", ClampMin = "0"))
	int32 MinHeight = 32;

	/**
	 * @brief Style to apply to the button - only used if not a spacer, but a button
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|CommonButtonParams|Appearance",
			  meta = (EditConditionHides, EditCondition = "!bIsSpacer", MetaClass = "CommonButtonStyle"))
	FClassPathOverride Style;

	/**
	 * @brief Action to apply for the button click - only used if not a spacer, but a button
	 */
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "mod.io|UI|CommonButtonParams|Input", meta = (EditConditionHides, EditCondition = "!bIsSpacer",RowType = CommonInputActionDataBase))
	FDataTableRowHandle TriggeringInputAction;

	/**
	 * @brief Whether to consume pointer input when the button is clicked - only used if not a spacer, but a button
	 */
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "mod.io|UI|CommonButtonParams|Input", meta = (EditConditionHides, EditCondition = "!bIsSpacer"))
	bool bConsumePointerInput = true;

	/**
	 * @brief Whether to use the default input action if TriggeringInputAction is not set - only used if not a spacer, but a button
	 */
	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "mod.io|UI|CommonButtonParams|Input", meta = (EditConditionHides, EditCondition = "!bIsSpacer"))
	bool bShouldUseFallbackDefaultInputAction = true;
};

