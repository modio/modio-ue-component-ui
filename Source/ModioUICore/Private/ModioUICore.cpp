/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "ModioUICore.h"

#include "Engine/Engine.h"
#include "ModioErrorCondition.h"
#include "ModioSubsystem.h"
#include "ModioUISettings.h"
#include "ModioUISubsystem.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#endif

#include "Modules/ModuleManager.h"
#include "GenericPlatform/GenericPlatformMisc.h"

DEFINE_LOG_CATEGORY(ModioUICore);

#define LOCTEXT_NAMESPACE "FModioUICore"

void FModioUICore::StartupModule()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Project", "Plugins", "mod.io UI", LOCTEXT("UISettingsName", "mod.io UI"),
			LOCTEXT("UISettingsDescription",
					"Configure the mod.io Component UI plugin's global settings."),
			GetMutableDefault<UModioUISettings>());
	}
#endif

	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Modio.RequestEmailAuthCode"),
		TEXT("Request an email authentication code be sent to the specified email address"),
		FConsoleCommandWithWorldArgsAndOutputDeviceDelegate::CreateRaw(this,
																	   &FModioUICore::RequestEmailAuthCodeForTesting),
		ECVF_Default));
	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Modio.SubmitEmailAuthCode"),
		TEXT("Authenticate using a code sent to an email previously used with RequestEmailAuthCode"),
		FConsoleCommandWithWorldArgsAndOutputDeviceDelegate::CreateRaw(this,
																	   &FModioUICore::SubmitEmailAuthCodeForTesting),
		ECVF_Default));

	ConsoleCommands.Add(IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Modio.SimulateConnectivityChange"), TEXT("Simulates a change in connectivity state"),
		FConsoleCommandWithWorldArgsAndOutputDeviceDelegate::CreateRaw(this,
																	   &FModioUICore::SimulateConnectivityChange)));
}

void FModioUICore::ShutdownModule()
{
	for (IConsoleCommand* Command : ConsoleCommands)
	{
		IConsoleManager::Get().UnregisterConsoleObject(Command);
	}

#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "mod.io UI");
	}
#endif
}

void FModioUICore::RequestEmailAuthCodeForTesting(const TArray<FString>& Args, UWorld*, FOutputDevice& OutputDevice)
{
	if (Args.Num() != 1)
	{
		OutputDevice.Log(ELogVerbosity::Warning, "Argument count mismatch: expected 1");
		return;
	}

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->RequestEmailAuthCodeAsync(
			FModioEmailAddress(Args[0]), FOnErrorOnlyDelegateFast::CreateLambda([&OutputDevice](FModioErrorCode ec) {
				// OutputDevice.Log(ec ? ELogVerbosity::Fatal : ELogVerbosity::Display, ec.GetErrorMessage());
			}));
	}
}

void FModioUICore::SubmitEmailAuthCodeForTesting(const TArray<FString>& Args, UWorld*, FOutputDevice& OutputDevice)
{
	if (Args.Num() != 1)
	{
		OutputDevice.Log(ELogVerbosity::Warning, "Argument count mismatch: expected 1");
		return;
	}

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->AuthenticateUserEmailAsync(
			FModioEmailAuthCode(Args[0]), FOnErrorOnlyDelegateFast::CreateLambda([&OutputDevice](FModioErrorCode ec) {
				// OutputDevice.Log(ec ? ELogVerbosity::Fatal : ELogVerbosity::Display, ec.GetErrorMessage());
			}));
	}
}

void FModioUICore::SimulateConnectivityChange(const TArray<FString>& Args, UWorld*, FOutputDevice& OutputDevice)
{
	if (Args.Num() != 1)
	{
		OutputDevice.Log(ELogVerbosity::Warning, "Argument count mismatch: expected 1");
		return;
	}
	if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		Subsystem->NotifyConnectivityChange(Args[0].ToBool());
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioUICore, ModioUICore)
