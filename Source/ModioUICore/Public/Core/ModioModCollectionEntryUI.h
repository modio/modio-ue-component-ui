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
#include "Types/ModioModCollectionEntry.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"

#include "ModioModCollectionEntryUI.generated.h"

/**
 * @brief Wrapper around FModioModCollectionEntry when consumer needs UObject-representable data (e.g. for UMG as a list item or data source)
 */
UCLASS(BlueprintType, Category = "mod.io|UI|ModioModCollectionEntryUI")
class MODIOUICORE_API UModioModCollectionEntryUI : public UObject, public IModioModInfoUIDetails
{
	GENERATED_BODY()

public:
	/**
	 * @brief Underlying mod collection entry
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Transient, Category = "mod.io|UI|ModioModCollectionEntryUI")
	FModioModCollectionEntry Underlying;

	/**
	 * @brief Cached subscription status for this mod. This can be used to avoid unnecessary API calls
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "mod.io|UI|ModioModCollectionEntryUI")
	bool bCachedSubscriptionStatus = false;

protected:
	//~ Begin IModioModInfoUIDetails Interface
	virtual FModioModInfo GetFullModInfo_Implementation() override
	{
		return Underlying.GetModProfile();
	}
	virtual FModioModID GetModID_Implementation() override
	{
		return Underlying.GetID();
	}
	//~ End IModioModInfoUIDetails Interface
};
