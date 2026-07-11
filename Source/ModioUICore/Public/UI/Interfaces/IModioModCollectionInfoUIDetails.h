/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Types/ModioModCollection.h"
#include "UObject/Interface.h"
#include "IModioModCollectionInfoUIDetails.generated.h"

/**
 * @brief Data binding interface indicating that the implementing object can be queried for an associated
 * FModioModCollectionInfo
 */
UINTERFACE()
class UModioModCollectionInfoUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioModCollectionInfoUIDetails
{
	GENERATED_BODY()

protected:
	virtual FModioModCollectionInfo GetFullModCollectionInfo_Implementation()
	{
		return FModioModCollectionInfo();
	}

	virtual FModioModCollectionID GetModCollectionID_Implementation()
	{
		return FModioModCollectionID();
	}

public:
	/**
	 * Queries the implementing object for the Mod Collection Info for the mod that the object represents
	 * @return ModCollectionInfo for the associated mod collection
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Collection Info")
	FModioModCollectionInfo GetFullModCollectionInfo();

	/**
	 * Queries the implementing object for the Mod Collection ID for the mod that the object represents
	 * @return Mod Collection ID for the associated mod collection
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Collection Info")
	FModioModCollectionID GetModCollectionID();
};