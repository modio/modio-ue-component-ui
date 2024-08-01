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
#include "Types/ModioModManagementEvent.h"
#include "UObject/NoExportTypes.h"

#include "ModioModManagementEventUI.generated.h"

/**
 * @brief Wrapper around FModioModManagementEvent when consumer needs UObject-representable data (e.g. for UMG as a list item or data source)
 */
UCLASS(BlueprintType, Category = "mod.io|UI|ModioModManagementEventUI")
class MODIOUICORE_API UModioModManagementEventUI : public UObject
{
	GENERATED_BODY()
public:

	/**
	 * @brief Underlying mod management event
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioModManagementEventUI")
	FModioModManagementEvent Underlying;
};
