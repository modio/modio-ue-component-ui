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
#include "Types/ModioTokenPack.h"
#include "UObject/Interface.h"
#include "ModioTokenPackUIDetails.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding|Mod Info", Meta = (DocGroup = "Data Binding Interfaces"))
class MODIOUICORE_API UModioTokenPackUIDetails : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MODIOUICORE_API IModioTokenPackUIDetails
{
	GENERATED_BODY()

protected:
	virtual FModioTokenPack GetFullTokenPack_Implementation()
	{
		return FModioTokenPack();
	}

	virtual FModioTokenPackID GetTokenPackID_Implementation()
	{
		return FModioTokenPackID();
	}

public:
	/**
	 * Queries the implementing object for the Token Pack that the object represents
	 * @return Token Pack struct
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	FModioTokenPack GetFullTokenPack();

	/**
	 * Queries the implementing object for the Token Pack ID that the object represents
	 * @return Token Pack ID for the relevant TokenPack
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	FModioTokenPackID GetTokenPackID();
};
