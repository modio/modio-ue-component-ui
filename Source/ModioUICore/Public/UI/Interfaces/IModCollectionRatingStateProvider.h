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

#include "Delegates/Delegate.h"
#include "UObject/Interface.h"
#include "Types/ModioRating.h"

#include "IModCollectionRatingStateProvider.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FModCollectionRatingChangedChangeHandler, int64, RawID, EModioRating, NewRating);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModCollectionRatingChangeMulticastDelegate, int64, RawID, EModioRating,
                                             NewRating);

/**
 * Interface for querying or setting the Rating of mod Collection.
 */
UINTERFACE(BlueprintType)
class MODIOUICORE_API UModCollectionRatingStateProvider : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModCollectionRatingStateProvider : public IInterface
{
	GENERATED_BODY()

protected:
	virtual EModioRating NativeQueryModCollectionRating(int64 ModCollectionID)
	{
		return EModioRating::Neutral;
	}

	virtual bool NativeRequestModCollectionRatingChange(int64 CollectionID, EModioRating NewRating)
	{
		return true;
	}

	EModioRating QueryModCollectionRating_Implementation(int64 ModCollectionID)
	{
		return NativeQueryModCollectionRating(ModCollectionID);
	}

	bool RequestModCollectionRatingChange_Implementation(int64 CollectionID, EModioRating NewRating)
	{
		return NativeRequestModCollectionRatingChange(CollectionID, NewRating);
	}

public:
	/**
	 * Queries for a mod collection's current rating
	 * @param ModCollectionID the raw ID for the mod Collection to query
	 * @return Rating of the mod Collection
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UGC|Mod Collection Rating Provider")
	EModioRating QueryModCollectionRating(int64 ModCollectionID);

	/**
	 * Requests that a mod's rating be changed
	 * @param CollectionID The raw ID for the mod Collection to request the rating change for
	 * @param NewRating the new rating for the mod collection
	 * @return true if the mod's rating was successfully changed, else false
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UGC|Mod Collection Rating Provider")
	bool RequestModCollectionRatingChange(int64 CollectionID, EModioRating NewRating);
};