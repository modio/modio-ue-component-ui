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

#include "Core/ModioModCollectionEntryUI.h"
#include "Core/ModioModInfoUI.h"
#include "CoreMinimal.h"
#include "Types/ModioModCollectionEntry.h"
#include "Types/ModioModDependencyList.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioModInfoList.h"
#include "UObject/Interface.h"
#include "UObject/StrongObjectPtr.h"

#include "IModioUIModListViewInterface.generated.h"

class UModioModInfoUI;

/**
 * @brief Interface for any widget that can visualise a list of mods
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Mod List View", Meta = (DocGroup = "UI", Comment = "@brief Interface for any widget that can visualise a list of mods"))
class UModioUIModListViewInterface : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIModListViewInterface
{
	GENERATED_BODY()

protected:
	/// The default native implementation uses UModioModInfoUI/UModioModCollectionUI for data binding, both of which
	/// implement IModioModInfoUIDetails so you can use the same implementing object for handling both types of data
	/// sources

	/**
	 * @brief Default implementation which wraps each mod info in InList in a UModioModInfoUI for data binding
	 */
	virtual void SetModsFromModInfoList_Implementation(const FModioModInfoList& InList, bool bAddToExisting);

	/**
	 * @brief Default implementation which wraps each mod info in InArray in a UModioModInfoUI for data binding
	 */
	virtual void SetModsFromModInfoArray_Implementation(const TArray<FModioModInfo>& InArray, bool bAddToExisting);

	/**
	 * @brief Default implementation which wraps each mod info in InArray in a UModioModCollectionEntryUI for data binding
	 */
	virtual void SetModsFromModCollectionEntryArray_Implementation(const TArray<FModioModCollectionEntry>& InArray, bool bAddToExisting);

	/**
	 * @brief Implement this in derived native classes to set the list items from the provided array of data sources
	 * @param InListItems The new array of UObjects to use as your ListItems in your implementing object
	 * @param bAddToExisting Should the new array be appended or replace the existing one
	 */
	virtual void NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting) {}

	virtual void NativeSetModSelectionByID(FModioModID ModID) {}

	virtual void SetModSelectionByID_Implementation(FModioModID ModID);

public:
	/**
	 * @brief Sets the selected mod in the list based on the passed-in Mod ID
	 * @param ModID The mod ID to select
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod List View")
	void SetModSelectionByID(FModioModID ModID);

	/**
	 * @brief Populates the list based on the mods contained in the provided ModInfoList
	 * @param InList The set of mods to populate with
	 * @param bAddToExisting Should the new set be merged with the existing mods or replace them?
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod List View")
	void SetModsFromModInfoList(const FModioModInfoList& InList, bool bAddToExisting);

	/**
	 * @brief Populates the list based on the provided raw array of ModInfo objects
	 * @param InArray The set of mods to populate with
	 * @param bAddToExisting Should the new set be merged with the existing mods or replace them?
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod List View")
	void SetModsFromModInfoArray(const TArray<FModioModInfo>& InArray, bool bAddToExisting);

	/**
	 * @brief Populates the list based on the provided array of ModCollectionEntry objects
	 * @param InArray The set of mods to populate with
	 * @param bAddToExisting Should the new set be merged with the existing mods or replace them?
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod List View")
	void SetModsFromModCollectionEntryArray(const TArray<FModioModCollectionEntry>& InArray, bool bAddToExisting);
};
