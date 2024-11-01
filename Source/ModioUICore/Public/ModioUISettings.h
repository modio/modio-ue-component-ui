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

#include "Engine/DataTable.h"
#include "Materials/Material.h"
#include "Types/ModioFilterParams.h"

#include "ModioUISettings.generated.h"

UCLASS(Config = Editor, defaultconfig)
class MODIOUICORE_API UModioUISettings : public UObject
{
	GENERATED_BODY()

public:
	UModioUISettings();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UDataTable> ColorsTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UDataTable> CommandsTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UMaterial> UnderlineMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UMaterial> FakeCaretMaterial;

	/**
	 * Named sets of filter parameters for use in the default PresetFilterSelector UI component
	 */
	UPROPERTY(EditDefaultsOnly, config, Category = "UI Configuration")
	TArray<FModioPresetFilterParams> PresetNamedFilters;
};
