// Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>

#pragma once

#include "Delegates/Delegate.h"
#include "UObject/Interface.h"
#include "Types/ModioRating.h"

#include "IModRatingStateProvider.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FModRatingChangedChangeHandler, int64, RawID, EModioRating, NewRating);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnModRatingChangeMulticastDelegate, int64, RawID, EModioRating,NewRating);

/**
 * Interface for querying or setting the Rating of mod.
 */
UINTERFACE(BlueprintType)
class MODIOUICORE_API UModRatingStateProvider : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModRatingStateProvider : public IInterface
{
	GENERATED_BODY()

protected:
	virtual EModioRating NativeQueryModRating(int64 ModID)
	{
		return EModioRating::Neutral;
	}

	virtual bool NativeRequestModRatingChange(int64 ID, EModioRating NewRating)
	{
		return true;
	}

	EModioRating QueryModRating_Implementation(int64 ModID)
	{
		return NativeQueryModRating(ModID);
	}

	bool RequestModRatingChange_Implementation(int64 ID, EModioRating NewRating)
	{
		return NativeRequestModRatingChange(ID, NewRating);
	}

public:
	/**
	 * Queries if a mod is currently Rating
	 * @param ModID the raw ID for the mod to query
	 * @return Rating of the mod
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UGC|Mod Rating Provider")
	EModioRating QueryModRating(int64 ModID);

	/**
	 * Requests that a mod's rating be changed
	 * @param ID The raw ID for the mod to request the rating change for
	 * @param NewRating the new rating for the mod
	 * @return true if the mod's rating was successfully changed, else false
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UGC|Mod Rating Provider")
	bool RequestModRatingChange(int64 ID, EModioRating NewRating);
};
