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

UCLASS(Config = Game, DefaultConfig)
class MODIOUICORE_API UModioUISettings : public UObject
{
	GENERATED_BODY()

public:
	UModioUISettings();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UDataTable> ColorsTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UDataTable> SoundsTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UDataTable> ForceFeedbackTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UDataTable> CommandsTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UMaterial> UnderlineMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UMaterial> FakeCaretMaterial;

protected:

	/**
	 * Named sets of filter parameters for use in the default mod browser when the marketplace is disabled
	 */
	UPROPERTY(EditDefaultsOnly, config, Category = "UI Configuration")
	TArray<FModioPresetFilterParams> PresetNamedFilters;

	/**
	 * Named sets of filter parameters for use in the default mod browser when the marketplace is enabled
	 */
	UPROPERTY(EditDefaultsOnly, config, Category = "UI Configuration")
	TArray<FModioPresetFilterParams> MarketplaceEnabledPresetNamedFilters;

public:

	/**
	 * Get the correct filters based on whether the marketplace is enabled or not
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI Configuration")
	static const TArray<FModioPresetFilterParams>& GetPresetNamedFilters();

	/**
	 * Get the correct filters based on whether the marketplace is enabled or not
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI Configuration")
	static const FModioPresetFilterParams& GetPresetFilterByName(const FText& PresetFilterName, bool& bSuccess);

	/**
	 * Named sets of filter parameters for use in defining preset collections search
	 */
	UPROPERTY(EditDefaultsOnly, config, Category = "UI Configuration")
	FModioPresetFilterParams DefaultCollectionsFilter;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "UI Configuration")
	static FModioPresetFilterParams GetDefaultCollectionsFilter();
};
