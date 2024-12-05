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
#include "UI/Interfaces/ModioTokenPackUIDetails.h"
#include "UObject/NoExportTypes.h"
#include "Types/ModioTokenPack.h"

#include "ModioTokenPackUI.generated.h"

/**
 * @brief Wrapper around FModioTokenPack when consumer needs UObject-representable data (e.g. for UMG as a list item or data source)
 */
UCLASS(BlueprintType, Category = "mod.io|UI|ModioTokenPackUI")
class MODIOUICORE_API UModioTokenPackUI : public UObject, public IModioTokenPackUIDetails
{
	GENERATED_BODY()

protected:
	//~ Begin IModioTokenPackUIDetails Interface
	virtual FModioTokenPack GetFullTokenPack_Implementation()
	{
		return Underlying;
	}

	virtual FModioTokenPackID GetTokenPackID_Implementation()
	{
		return Underlying.GetId();
	}
	//~ End IModioTokenPackUIDetails Interface

public:
	/**
	 * @brief Underlying mod info
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioTokenPackUI")
	FModioTokenPack Underlying;
};
