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

#include "Core/ModioModInfoUI.h"
#include "CoreMinimal.h"
#include "Types/ModioReportParams.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UObject/Object.h"

#include "ModioReportInfoUI.generated.h"

/**
 * @brief UObject wrapper around an FModioReportParams for use as a data source
 */
UCLASS()
class MODIOUICORE_API UModioReportInfoUI : public UObject, public IModioModInfoUIDetails
{
	GENERATED_BODY()

public:
	/**
	 * @brief Data for the report
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioReportInfoUI")
	FModioReportParams ReportData;

	/**
	 * @brief Mod info for the report
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioReportInfoUI")
	TObjectPtr<UObject> ModInfo;

protected:
	//~ Begin IModioModInfoUIDetails Interface
	virtual FModioModInfo GetFullModInfo_Implementation() override
	{
		if (ModInfo->Implements<UModioModInfoUIDetails>())
		{
			return IModioModInfoUIDetails::Execute_GetFullModInfo(ModInfo);
		}

		return FModioModInfo();
	}

	virtual FModioModID GetModID_Implementation() override
	{
		if (ModInfo->Implements<UModioModInfoUIDetails>())
		{
			return IModioModInfoUIDetails::Execute_GetModID(ModInfo);
		}

		return FModioModID();
	}
	//~ End IModioModInfoUIDetails Interface
};
