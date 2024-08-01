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
#include "UI/Interfaces/IModioUserUIDetails.h"

#include "ModioUserUI.generated.h"

/**
 * @brief Wrapper around FModioUser when consumer needs UObject-representable data (e.g. for UMG as a list item or data source)
 */
UCLASS(BlueprintType, Category = "mod.io|UI|ModioUserUI")
class MODIOUICORE_API UModioUserUI : public UObject, public IModioUserUIDetails
{
	GENERATED_BODY()
protected:
	//~ Begin IModioUserUIDetails Interface
	virtual FModioUser GetFullUser_Implementation() override
	{
		return Underlying;
	}
	virtual FModioUserID GetUserID_Implementation() override
	{
		return Underlying.UserId;
	}
	//~ End IModioUserUIDetails Interface

public:
	/**
	 * @brief Underlying user info
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioUserUI")
	FModioUser Underlying;
};
