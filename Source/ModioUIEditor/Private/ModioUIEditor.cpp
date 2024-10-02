/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "ModioUIEditor.h"

#include "ModioEditorUtilityFunctions.h"
#include "ModioUIEditorSettings.h"

#if WITH_EDITOR
	#include "ISettingsModule.h"
	#include "Modules/ModuleManager.h"
	#include "EditorUtilityWidgetBlueprint.h"
	#include "Editor.h"
	#include "Interfaces/IMainFrameModule.h"
	#include "EditorUtilitySubsystem.h"
#endif

DEFINE_LOG_CATEGORY(ModioUIEditor);

#define LOCTEXT_NAMESPACE "FModioUIEditor"

void FModioUIEditor::StartupModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project", "Plugins", "mod.io UI Editor", LOCTEXT("UIEditorSettingsName", "mod.io UI Editor"),
			LOCTEXT("UIEditorSettingsDescription", "Configure the mod.io Component UI plugin's editor-specific behaviour"),
			GetMutableDefault<UModioUIEditorSettings>());
	}

	UModioEditorUtilityFunctions::AddGettingStartedWidgetEntries(
		GetMutableDefault<UModioUIEditorSettings>()->GettingStartedEntries);
}

void FModioUIEditor::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "mod.io UI Editor");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioUIEditor, ModioUIEditor)