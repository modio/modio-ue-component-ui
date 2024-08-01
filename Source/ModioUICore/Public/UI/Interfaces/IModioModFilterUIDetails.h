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

#include "Types/ModioFilterParams.h"
#include "UObject/Interface.h"

#include "IModioModFilterUIDetails.generated.h"

/**
 * @brief Data binding interface indicating that the implementing object can be queried for an associated FModioFilterParams
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding|Mod Filter", Meta = (DocGroup = "Data Binding Interfaces"))
class MODIOUICORE_API UModioModFilterUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioModFilterUIDetails : public IInterface
{
	GENERATED_BODY()

protected:
	virtual FModioFilterParams GetFilterParams_Implementation()
	{
		return FModioFilterParams();
	}

	virtual bool GetFilterName_Implementation(FText& Name)
	{
		Name = {};
		return false;
	}

public:
	/**
	 * @brief Queries the implementing object for the mod filter parameters that the object represents
	 * @return ModFilterParams associated with the object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Filter")
	FModioFilterParams GetFilterParams();

	/**
	 * @brief Queries the implementing object for a name that may be associated with the filter
	 * @param Name The name for the filter if there was one present
	 * @return true if the implementing object provides a name for the filter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Filter")
	bool GetFilterName(UPARAM(ref) FText& Name);
	
};
