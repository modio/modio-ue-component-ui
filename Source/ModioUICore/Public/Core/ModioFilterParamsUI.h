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
#include "Types/ModioFilterParams.h"
#include "UI/Interfaces/IModioModFilterUIDetails.h"
#include "UObject/NoExportTypes.h"

#include "ModioFilterParamsUI.generated.h"

/**
 * @brief Wrapper around FModioFilterParams when consumer needs UObject-representable data (e.g. for UMG as a list item or data source)
 */
UCLASS(BlueprintType, Category = "mod.io|UI|ModioFilterParamsUI")
class MODIOUICORE_API UModioFilterParamsUI : public UObject, public IModioModFilterUIDetails
{
	GENERATED_BODY()
protected:
	//~ Begin IModioModFilterUIDetails Interface
	virtual FModioFilterParams GetFilterParams_Implementation() override
	{
		return Underlying;
	}
	virtual bool GetFilterName_Implementation(FText& Name) override
	{
		Name = FilterName;
		return true;
	}
	//~ End IModioModFilterUIDetails Interface

public:
	/**
	 * @brief Display name for this set of filter parameters.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioFilterParamsUI")
	FText FilterName;

	/**
	 * @brief Underlying filter parameters
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioFilterParamsUI")
	FModioFilterParams Underlying;
};
