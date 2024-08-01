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

#include "Types/ModioModInfo.h"
#include "UObject/Interface.h"

#include "IModioModInfoUIDetails.generated.h"

/**
 * @brief Data binding interface indicating that the implementing object can be queried for an associated FModioModInfo
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding|Mod Info", Meta = (DocGroup = "Data Binding Interfaces"))
class MODIOUICORE_API UModioModInfoUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioModInfoUIDetails : public IInterface
{
	GENERATED_BODY()

protected:
	virtual FModioModInfo GetFullModInfo_Implementation()
	{
		return FModioModInfo();
	}
	
	virtual FModioModID GetModID_Implementation()
	{
		return FModioModID();
	}

public:
	/**
	 * Queries the implementing object for the Mod Info for the mod that the object represents
	 * @return ModInfo for the associated mod
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Info")
	FModioModInfo GetFullModInfo();

	/**
	 * Queries the implementing object for the Mod ID for the mod that the object represents
	 * @return Mod ID for the associated mod
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Info")
	FModioModID GetModID();
};
