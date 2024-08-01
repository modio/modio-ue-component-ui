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

#include "Types/ModioModDependencyList.h"
#include "UObject/Interface.h"

#include "IModioModDependencyUIDetails.generated.h"

/**
 * @brief Data binding interface indicating that the implementing object can be queried for an associated FModioModDependency
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding|Mod Dependency", Meta = (DocGroup = "Data Binding Interfaces"))
class MODIOUICORE_API UModioModDependencyUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioModDependencyUIDetails : public IInterface
{
	GENERATED_BODY()

protected:
	virtual FModioModDependency GetModDependency_Implementation()
	{
		return FModioModDependency();
	}

public:
	/**
	 * Queries the implementing object for the Mod Dependency that the object represents
	 * @return ModioModDependency for the associated mod
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Dependency")
	FModioModDependency GetModDependency();

	/**
	 * Queries the implementing object for the Mod ID for the mod that the object represents
	 * @return Mod ID for the associated mod
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Dependency")
	FModioModID GetModID();
};
