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

#include "Types/ModioCommonTypes.h"
#include "UObject/Interface.h"

#include "IModioUIModEnabledStateProvider.generated.h"

/**
 * @brief Interface for objects that can provide or store the enabled/disabled state for a mod
 */
UINTERFACE(
	BlueprintType, Category = "mod.io|UI|ModState",
	Meta = (DocGroup = "UI",
			Comment = "@brief Interface for objects that can provide or store the enabled/disabled state for a mod"))
class MODIOUICORE_API UModioUIModEnabledStateProvider : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIModEnabledStateProvider : public IInterface
{
	GENERATED_BODY()

protected:
	virtual bool NativeQueryIsModEnabled(FModioModID ModID)
	{
		return true;
	}
	virtual bool NativeRequestModEnabledStateChange(FModioModID ID, bool bNewEnabledState)
	{
		return true;
	}
	bool QueryIsModEnabled_Implementation(FModioModID ModID)
	{
		return NativeQueryIsModEnabled(ModID);
	}
	bool RequestModEnabledStateChange_Implementation(FModioModID ID, bool bNewEnabledState)
	{
		return NativeRequestModEnabledStateChange(ID, bNewEnabledState);
	}

public:
	/**
	 * @brief Queries the implementing object for the enabled/disabled state of a mod
	 * @param ModID The ID of the mod to query
	 * @return True if the mod is enabled, false if it is disabled
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ModState")
	bool QueryIsModEnabled(FModioModID ModID);

	/**
	 * @brief Requests that the implementing object change the enabled/disabled state of a mod
	 * @param ID The ID of the mod to change the state of
	 * @param bNewEnabledState The new enabled state to set
	 * @return True if the request was successful, false if it was not
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ModState")
	bool RequestModEnabledStateChange(FModioModID ID, bool bNewEnabledState);
};
