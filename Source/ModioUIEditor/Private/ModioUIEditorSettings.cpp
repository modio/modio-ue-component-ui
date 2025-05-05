/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "ModioUIEditorSettings.h"

#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "ModioUISettings.h"
#include "Core/ModioAssetPaths.h"
#include "Objects/ModioOpenEditorWidgetAction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioUIEditorSettings)

#define LOCTEXT_NAMESPACE "FModioUIEditorSettings"

UModioUIEditorSettings::UModioUIEditorSettings(const FObjectInitializer& Initializer)
	: Super(Initializer)
{
	//Maybe move this to some other class for neatness, in case we need to add more later.
	GettingStartedEntries.Add(FModioGettingStartedEntry(
		true, LOCTEXT("MigrationEntryName", "Component UI Migration"),
		LOCTEXT("MigrationEntryDescription", "The Component UI framework is designed with the expectation that you will migrate (copy) our framework for customization."),
		TSoftObjectPtr<UTexture2D>(FSoftObjectPath("/Modio/GettingStarted/T_modbrowser_D.T_modbrowser_D")),
		UModioOpenEditorWidgetAction::StaticClass(), "/Script/Blutility.EditorUtilityWidgetBlueprint'/ModioComponentUI/UI/Editor/ComponentUiOnboarding.ComponentUiOnboarding'"));
}

FString UModioUIEditorSettings::OpenDestinationSelector()
{
	FString Out;

	IDesktopPlatform* const DesktopPlatform = FDesktopPlatformModule::Get();
	check(DesktopPlatform);

	const FString DesiredRootPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	const FText DialogTitle = LOCTEXT("AutoCopyDestinationSelectionTitle", "Select Directory to Copy Components to");
	DesktopPlatform->OpenDirectoryDialog(nullptr, DialogTitle.ToString(), DesiredRootPath, Out);
	return FPaths::Combine(Out, "/");
}

void UModioUIEditorSettings::DoComponentCopy(const bool bIncludeTemplates, const bool bIncludeComponents, const bool bIncludeDeps)
{
	FString TargetFolder = FPaths::ProjectContentDir() / FString();
	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	TArray<FAssetData> CompUiAssetData;
	TArray<FName> PackageNames;
	TArray<FString> Includes;

	Includes.Add("/ModioComponentUI/UI/Data");
	if (bIncludeTemplates)
		Includes.Add("/ModioComponentUI/UI/Templates");
	if (bIncludeComponents)
		Includes.Add("/ModioComponentUI/UI/Components");

	for (const FString& IncludePath : Includes)
	{
		TArray<FAssetData> NewAssets = TArray<FAssetData>();
		AssetRegistryModule.Get().GetAssetsByPath(*IncludePath, NewAssets, true);
		CompUiAssetData.Append(NewAssets);
	}

	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");

	if (bIncludeDeps)
	{
		for (const FAssetData& Asset : CompUiAssetData)
		{
			PackageNames.Add(Asset.PackageName);
		}

		// Include the two materials used as globals
		PackageNames.Add({"/ModioComponentUI/UI/Materials/M_UI_Underline"});
		PackageNames.Add({"/ModioComponentUI/UI/Materials/M_UI_FakeCaret"});
		PackageNames.Add({"/ModioComponentUI/UI/Data/DT_UIColors"});
		PackageNames.Add({"/ModioComponentUI/UI/Data/DT_UICommands"});

		AssetToolsModule.Get().MigratePackages(PackageNames, TargetFolder);

		// Now update the module settings to point to the new values
		// Not a great solution but it works fine.
		GetMutableDefault<UModioUISettings>()->ColorsTable =
			TSoftObjectPtr<UDataTable>(FSoftObjectPath("/Game/UI/Data/DT_UIColors.DT_UIColors"));
		GetMutableDefault<UModioUISettings>()->CommandsTable =
			TSoftObjectPtr<UDataTable>(FSoftObjectPath("/Game/UI/Data/DT_UICommands.DT_UICommands"));
		GetMutableDefault<UModioUISettings>()->UnderlineMaterial =
			TSoftObjectPtr<UMaterial>(FSoftObjectPath("/Game/UI/Materials/M_UI_Underline.M_UI_Underline"));
		GetMutableDefault<UModioUISettings>()->FakeCaretMaterial =
			TSoftObjectPtr<UMaterial>(FSoftObjectPath("/Game/UI/Materials/M_UI_FakeCaret.M_UI_FakeCaret"));
		GetMutableDefault<UModioUISettings>()->SaveConfig();
	}
	else
	{
		// Not including dependencies so no need for any data tables or config changes
		for (const FString& IncludePath : Includes)
		{
			PackageNames.Add(FName(*IncludePath));
		}
		AssetToolsModule.Get().BeginAdvancedCopyPackages(PackageNames, "/Game/UI/");
	}
}

#undef LOCTEXT_NAMESPACE
