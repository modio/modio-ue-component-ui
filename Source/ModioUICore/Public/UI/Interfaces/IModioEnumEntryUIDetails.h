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

#include "IModioEnumEntryUIDetails.generated.h"

/**
 * @brief Data binding interface indicating that the implementing object can be queried for an associated enum value and display string
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding|Enum", Meta = (DocGroup = "Data Binding Interfaces"))
class MODIOUICORE_API UModioEnumEntryUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioEnumEntryUIDetails : public IInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Queries the implementing object for it's associated enum value and display text for that value
	 * @param EnumValue the raw enum value the object represents
	 * @param EnumValueDisplayText the display string to show for this object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Enum")
	void GetEnumEntryDetails(uint8& EnumValue, FText& EnumValueDisplayText);
};
