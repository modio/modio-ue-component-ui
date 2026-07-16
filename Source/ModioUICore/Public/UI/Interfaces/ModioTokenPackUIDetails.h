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

UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding|Mod Info", Meta = (DocGroup = "Data Binding Interfaces"))
class MODIOUICORE_API UModioTokenPackUIDetails : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief Data binding interface indicating that the implementing object can be queried for data associated with a Token
 * Pack used for purchases with mod.io
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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack",
			  meta = (DeprecatedFunction, DeprecationMessage = "Please use the accessor methods for individual fields"))
	FModioTokenPack GetFullTokenPack();

	/**
	 * @docpublic
	 * @brief Get the title of the token pack
	 * @return The title text of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	FText GetTitle() const;
	/**
	 * @docpublic
	 * @brief Get the description of the token pack
	 * @return The description text of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	FText GetDescription() const;

	/**
	 * @docpublic
	 * @brief Get the long description of the token pack
	 * @return The long description text of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	FText GetLongDescription() const;

	/**
	 * @docpublic
	 * @brief Get the regular price of the token pack
	 * @return The regular price text of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	FText GetRegularPrice() const;

	/**
	 * Queries the implementing object for a textual representation of the current price of the token pack that the
	 * object represents
	 * @return FText containing display string for the pack's current price
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	FText GetTokenPackDisplayPrice() const;

	/**
	 * @docpublic
	 * @brief Get the numeric price of the token pack
	 * @return The numeric price of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	int64 GetNumericPrice() const;

	/**
	 * @docpublic
	 * @brief Get the currency code of the token pack
	 * @return The currency code of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	FString GetCurrencyCode() const;

	/**
	 * @docpublic
	 * @brief Get the release date of the token pack
	 * @return The release date of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	FDateTime GetReleaseDate() const;

	/**
	 * @docpublic
	 * @brief Get the expiration date of the token pack
	 * @return The expiration date of the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	FDateTime GetExpirationDate() const;

	/**
	 * @docpublic
	 * @brief Check if the token pack is purchasable
	 * @return True if the token pack is purchasable, false otherwise
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	bool IsPurchasable() const;

	/**
	 * @docpublic
	 * @brief Get any dynamic fields associated with the token pack
	 * @return A map of dynamic fields for the token pack
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	TMap<FString, FString> GetFields() const;

	/**
	 * Queries the implementing object for the Token Pack ID that the object represents
	 * @return Token Pack ID for the relevant TokenPack
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Token Pack")
	FModioTokenPackID GetModioId() const;
};
