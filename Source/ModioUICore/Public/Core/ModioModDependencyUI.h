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
#include "UI/Interfaces/IModioModDependencyUIDetails.h"
#include "UObject/NoExportTypes.h"

#include "ModioModDependencyUI.generated.h"

/**
 * @brief Wrapper around FModioModDependency when consumer needs UObject-representable data (e.g. for UMG as a list item or data source)
 */
UCLASS(BlueprintType, Category = "mod.io|UI|ModioModDependencyUI")
class MODIOUICORE_API UModioModDependencyUI : public UObject, public IModioModDependencyUIDetails
{
	GENERATED_BODY()
protected:
	//~ Begin IModioModDependencyUIDetails Interface
	virtual FModioModDependency GetModDependency_Implementation() override
	{
		return Underlying;
	}
	virtual FModioModID GetModID_Implementation() override
	{
		return Underlying.ModID;
	}
	//~ End IModioModDependencyUIDetails Interface

public:
	/**
	 * @brief Underlying mod dependency
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioModDependencyUI")
	FModioModDependency Underlying;
};
