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

#include "Types/ModioGettingStartedEntry.h"

#include "ModioUIEditorSettings.generated.h"

UCLASS(Config = Editor, defaultconfig)
class MODIOUIEDITOR_API UModioUIEditorSettings : public UObject
{
	GENERATED_BODY()

public:

	UModioUIEditorSettings(const FObjectInitializer& Initializer);

	UFUNCTION(BlueprintCallable, Category = "modio UI Editor")
	static FString OpenDestinationSelector();

	UFUNCTION(BlueprintCallable, Category = "modio UI Editor")
	static void DoComponentCopy(const bool bIncludeTemplates, const bool bIncludeComponents, const bool bIncludeDeps);

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Config, Category = "modio UI Editor|Getting Started ")
	TSet<FModioGettingStartedEntry> GettingStartedEntries;
};
