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

#include "IModioUserUIDetails.generated.h"

/**
 * @brief Data binding interface indicating that the implementing object can be queried for an associated FModioUser
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding|User", Meta = (DocGroup = "Data Binding Interfaces"))
class MODIOUICORE_API UModioUserUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUserUIDetails : public IInterface
{
	GENERATED_BODY()

protected:
	virtual FModioUser GetFullUser_Implementation()
	{
		return FModioUser();
	}

	virtual FModioUserID GetUserID_Implementation()
	{
		return FModioUserID();
	}

public:
	/**
	 * @brief Queries the implementing object for the user that the object represents
	 * @return FModioUser for the associated user
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|User")
	FModioUser GetFullUser();

	/**
	 * @brief Queries the implementing object for the User ID for the user that the object represents
	 * @return User ID for the associated user
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|User")
	FModioUserID GetUserID();
};
