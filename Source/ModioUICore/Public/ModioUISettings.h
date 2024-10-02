/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Engine/DataTable.h"
#include "Materials/Material.h"

#include "ModioUISettings.generated.h"

UCLASS(Config = Editor, defaultconfig)
class MODIOUICORE_API UModioUISettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UDataTable> ColorsTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UDataTable> CommandsTable;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UMaterial> UnderlineMaterial;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI")
	TSoftObjectPtr<UMaterial> FakeCaretMaterial;
};
