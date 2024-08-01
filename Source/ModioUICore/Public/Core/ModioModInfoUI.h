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
#include "Types/ModioModInfo.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UObject/NoExportTypes.h"

#include "ModioModInfoUI.generated.h"

/**
 * @brief Wrapper around FModioModInfo when consumer needs UObject-representable data (e.g. for UMG as a list item or data source)
 */
UCLASS(BlueprintType, Category = "mod.io|UI|ModioModInfoUI")
class MODIOUICORE_API UModioModInfoUI : public UObject, public IModioModInfoUIDetails
{
	GENERATED_BODY()
protected:
	//~ Begin IModioModInfoUIDetails Interface
	virtual FModioModInfo GetFullModInfo_Implementation() override
	{
		return Underlying;
	}
	virtual FModioModID GetModID_Implementation() override
	{
		return Underlying.ModId;
	}
	//~ End IModioModInfoUIDetails Interface

public:
	/**
	 * @brief Underlying mod info
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioModInfoUI")
	FModioModInfo Underlying;
};
