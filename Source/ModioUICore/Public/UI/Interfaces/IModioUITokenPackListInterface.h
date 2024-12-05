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
#include "Types/ModioTokenPackList.h"
#include "UObject/Interface.h"

#include "IModioUITokenPackListInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, Category = "mod.io|UI|Token Pack List View",
		   Meta = (DocGroup = "UI", Comment = "@brief Interface for any widget that can visualise a list of token packs"))
class UModioUITokenPackListInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MODIOUICORE_API IModioUITokenPackListInterface
{
	GENERATED_BODY()

	protected:
	/// The default native implementation uses UModioTokenPackUI for data binding, which
	/// implements IModioTokenPackUIDetails so you can use the same implementing object for handling

	/**
	 * @brief Default implementation which wraps each token pack in InList in a UModioTokenPackUI for data binding
	 */
	virtual void SetTokenPacksFromTokenPackList_Implementation(const FModioTokenPackList& InList, bool bAddToExisting);

	/**
	 * @brief Default implementation which wraps each token pack in InArray in a UModioTokenPackUI for data binding
	 */
	virtual void SetTokenPacksFromTokenPackArray_Implementation(const TArray<FModioTokenPack>& InArray,
																bool bAddToExisting);

	/**
	 * @brief Implement this in derived native classes to set the list items from the provided array of data sources
	 * @param InListItems The new array of UObjects to use as your ListItems in your implementing object
	 * @param bAddToExisting Should the new array be appended or replace the existing one
	 */
	virtual void NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting) {}

	virtual void NativeSetTokenPackSelectionByID(FModioTokenPackID TokenPackID) {}

	virtual void SetTokenPackSelectionByID_Implementation(FModioTokenPackID TokenPackID);

public:
	/**
	 * @brief Sets the selected token pack in the list based on the passed-in Token Pack ID
	 * @param TokenPackID The token pack ID to select
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod List View")
	void SetTokenPackSelectionByID(FModioTokenPackID TokenPackID);

	/**
	 * @brief Populates the list based on the token packs contained in the provided TokenPackList
	 * @param InList The set of token packs to populate with
	 * @param bAddToExisting Should the new set be merged with the existing token packs or replace them?
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod List View")
	void SetTokenPacksFromTokenPackList(const FModioTokenPackList& InList, bool bAddToExisting);

	/**
	 * @brief Populates the list based on the provided raw array of Token Pack objects
	 * @param InArray The set of token packs to populate with
	 * @param bAddToExisting Should the new set be merged with the existing token packs or replace them?
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod List View")
	void SetTokenPacksFromTokenPackArray(const TArray<FModioTokenPack>& InArray, bool bAddToExisting);
};
