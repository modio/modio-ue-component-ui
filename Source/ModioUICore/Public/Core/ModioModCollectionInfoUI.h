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
#include "UI/Interfaces/IModioModCollectionInfoUIDetails.h"
#include "UObject/Object.h"
#include "ModioModCollectionInfoUI.generated.h"

/**
 * @brief Wrapper around FModioModCollectionInfo when consumer needs UObject-representable data (e.g. for UMG as a list item or data source)
 */
UCLASS(BlueprintType, Category = "mod.io|UI|ModioModCollectionInfoUI")
class MODIOUICORE_API UModioModCollectionInfoUI : public UObject, public IModioModCollectionInfoUIDetails
{
	GENERATED_BODY()

protected:
	//~ Begin IModioModInfoUIDetails Interface
	virtual FModioModCollectionInfo GetFullModCollectionInfo_Implementation() override
	{
		return Underlying;
	}

	virtual FModioModCollectionID GetModCollectionID_Implementation() override
	{
		return Underlying.Id;
	}

	//~ End IModioModInfoUIDetails Interface

public:
	/**
	 * @brief Underlying mod info
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioModInfoUI")
	FModioModCollectionInfo Underlying;
};