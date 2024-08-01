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

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Types/ModioModTag.h"
#include "Types/ModioModTagInfo.h"
#include "Types/ModioModTagOptions.h"
#include "UObject/Interface.h"

#include "IModioUIModTagSelector.generated.h"

DECLARE_DYNAMIC_DELEGATE(FModioOnTagSelectionChanged);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FModioOnTagSelectionChangedMulticast);

/**
 * @brief Interface class for any widget that displays a set of mod tags for a user to select
 */
UINTERFACE(Meta = (DocGroup = "UI", Comment = "@brief Interface class for any widget that displays a set of mod tags for a user to select"))
class UModioUIModTagSelector : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIModTagSelector
{
	GENERATED_BODY()

public:
	/**
	 * @brief Populates the tag selector with the tags specified in the provided tag options struct
	 * @param InOptions Tag data to use to populate the selector
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|TagSelector")
	void SetAvailableTagsFromModTagOptions(const FModioModTagOptions& InOptions);

	/**
	 * @brief Populates the tag selector using the pre-bound tag options
	 * @param InOptions Tag data to use to populate the selector
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|TagSelector")
	void SetAvailableTagsFromBoundModTagOptions(const TScriptInterface<UModioModTagOptionsUIDetails>& InOptions);

	/**
	 * @brief Retrieves the selected tags from the implementing object
	 * @return Array containing the selected tags' raw unlocalized values
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|TagSelector")
	TArray<FString> GetSelectedTags();

	/**
	 * @brief Clears the selection state of the selector
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|TagSelector")
	void ClearSelectedTags();

	/**
	 * @brief Registers a delegate to receive callbacks when the set of selected tags changes
	 * @param Handler Delegate to invoke on a selection changed event
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|TagSelector")
	void AddTagSelectionChangedHandler(const FModioOnTagSelectionChanged& Handler);

	/**
	 * @brief Unregisters a delegate so it no longer receives callbacks
	 * @param Handler Delegate that will no longer receive selection changed events
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|TagSelector")
	void RemoveTagSelectionChangedHandler(const FModioOnTagSelectionChanged& Handler);
};
