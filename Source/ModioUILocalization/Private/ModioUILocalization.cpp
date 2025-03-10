/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "ModioUILocalization.h"

#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/ConfigCacheIni.h"

DEFINE_LOG_CATEGORY(ModioUILocalization);

#define LOCTEXT_NAMESPACE "FModioUILocalization"

void FModioUILocalization::StartupModule()
{
	// Localization path configuration was moved from runtime to build-time (ModioUILocalization.build.cs)
	// This change was necessary because runtime configuration required EarliestPossible loading phase,
	// which caused issues such as with commandlets. The module now uses Default loading phase and relies on build-time configuration
	/*
#if WITH_EDITOR
	if (GConfig && GConfig->IsReadyForUse())
	{
		TSharedPtr<IPlugin> ModioComponentUIPlugin = IPluginManager::Get().FindPlugin(TEXT("ModioComponentUI"));
		if (!ModioComponentUIPlugin.IsValid())
		{
			UE_LOG(ModioUILocalization, Error, TEXT("Failed to find ModioComponentUI plugin"));
			return;
		}
		
		// NOTE: Alternate localization path logic based on plugin location
		// const FString ModioLocalizationPath = [ModioComponentUIPlugin]() {
		//	 switch (ModioComponentUIPlugin->GetLoadedFrom())
		//	 {
		//		 case EPluginLoadedFrom::Engine:
		//		 {
		//			 FString ContentDir = ModioComponentUIPlugin->GetContentDir();
		//			 if (ContentDir.Contains(TEXT("Marketplace")))
		//			 {
		//				 return FString::Printf(TEXT("%%ENGINEDIR%%Plugins/Marketplace/%s/Content/Localization/ModioUI"), *ModioComponentUIPlugin->GetName());
		//			 }
		//			 return FString::Printf(TEXT("%%ENGINEDIR%%Plugins/%s/Content/Localization/ModioUI"), *ModioComponentUIPlugin->GetName());
		//		 }
		//		 case EPluginLoadedFrom::Project:
		//		 default:
		//			 return FString::Printf(TEXT("%%GAMEDIR%%Plugins/%s/Content/Localization/ModioUI"), *ModioComponentUIPlugin->GetName());
		//	 }
		// }();

		const FString ModioLocalizationPath = FString::Printf(TEXT("%s/Localization/ModioUI"), *ModioComponentUIPlugin->GetContentDir());

		if (!FPaths::DirectoryExists(ModioLocalizationPath))
		{
			UE_LOG(ModioUILocalization, Error, TEXT("Localization path '%s' does not exist. Localization for mod.io UI will not be available."), *ModioLocalizationPath);
			return;
		}

		TArray<FString> GameLocalizationPaths;
		GConfig->GetArray(TEXT("Internationalization"), TEXT("LocalizationPaths"), GameLocalizationPaths, GGameIni);
		if (!GameLocalizationPaths.Contains(ModioLocalizationPath))
		{
			GameLocalizationPaths.Add(ModioLocalizationPath);
			GConfig->SetArray(TEXT("Internationalization"), TEXT("LocalizationPaths"), GameLocalizationPaths, GGameIni);
			GConfig->Flush(false, GGameIni);
		}
	}
#endif
	*/
}

void FModioUILocalization::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioUILocalization, ModioUILocalization)
