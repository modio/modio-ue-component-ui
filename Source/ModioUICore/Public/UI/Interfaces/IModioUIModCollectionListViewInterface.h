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
#include "Types/ModioModCollection.h"
#include "UObject/Interface.h"
#include "IModioUIModCollectionListViewInterface.generated.h"

class UModioModCollectionInfoUI;

/**
 * @brief Interface for any widget that can visualise a list of mods
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Mod Collection List View",
	Meta = (DocGroup = "UI", Comment = "@brief Interface for any widget that can visualise a list of mod collections"))
class UModioUIModCollectionListViewInterface : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIModCollectionListViewInterface
{
	GENERATED_BODY()

protected:
	/// The default native implementation uses UModioModCollectionInfoUI for data binding, which implements
	/// IModioModCollectionInfoUIDetails so you can use the same implementing object for handling data sources

	/**
	 * @brief Default implementation which wraps each mod info in InList in a UModioModCollectionInfoUI for data binding
	 */
	virtual void SetModCollectionFromModCollectionInfoList_Implementation(const FModioModCollectionInfoList& InList,
	                                                                      bool bAddToExisting);

	/**
	 * @brief Default implementation which wraps each mod info in InArray in a UModioModCollectionInfo for data binding
	 */
	virtual void SetModCollectionFromModCollectionInfoArray_Implementation(
		const TArray<FModioModCollectionInfo>& InArray, bool bAddToExisting);

	/**
	 * @brief Implement this in derived native classes to set the list items from the provided array of data sources
	 * @param InListItems The new array of UObjects to use as your ListItems in your implementing object
	 * @param bAddToExisting Should the new array be appended or replace the existing one
	 */
	virtual void NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting) {}

	virtual void NativeSetModCollectionSelectionByID(FModioModCollectionID ModCollectionID) {}

	virtual void SetModCollectionSelectionByID_Implementation(FModioModCollectionID ModCollectionID);

public:
	/**
	 * @brief Sets the selected mod in the list based on the passed-in Mod ID
	 * @param ModCollectionID The mod collection ID to select
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Collection List View")
	void SetModCollectionSelectionByID(FModioModCollectionID ModCollectionID);

	/**
	 * @brief Populates the list based on the mod collections contained in the provided ModCollectionInfoList
	 * @param InList The set of mod collections to populate with
	 * @param bAddToExisting Should the new set be merged with the existing mod collections or replace them?
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Collection List View")
	void SetModCollectionFromModCollectionInfoList(const FModioModCollectionInfoList& InList, bool bAddToExisting);

	/**
	 * @brief Populates the list based on the provided raw array of ModCollectionInfo objects
	 * @param InArray The set of mod collections to populate with
	 * @param bAddToExisting Should the new set be merged with the existing mod corrections or replace them?
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Collection List View")
	void SetModCollectionFromModCollectionInfoArray(const TArray<FModioModCollectionInfo>& InArray,
	                                                bool bAddToExisting);
};