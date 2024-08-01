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
#include "Loc/ModioEnumLocalizationHelpers.h"
#include "UI/Interfaces/IModioEnumEntryUIDetails.h"
#include "UObject/NoExportTypes.h"

#include "ModioEnumEntryUI.generated.h"

/**
 * @brief Class wrapping a specific entry from an enum, suitable for use as a data source
 */
UCLASS(BlueprintType)
class MODIOUICORE_API UModioEnumEntryUI : public UObject, public IModioEnumEntryUIDetails
{
	GENERATED_BODY()

public:
	/**
	 * @brief Underlying enum value for this entry
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "mod.io|UI|EnumEntry")
	uint8 UnderlyingEnumValue;

	/**
	 * @brief Display text for this entry
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ExposeOnSpawn = true), Category = "mod.io|UI|EnumEntry")
	FText UnderlyingEnumValueDisplayText;

	//~ Begin IModioEnumEntryUIDetails Interface
	virtual void GetEnumEntryDetails_Implementation(uint8& EnumValue, FText& EnumValueDisplayText) override;
	//~ End IModioEnumEntryUIDetails Interface
};
