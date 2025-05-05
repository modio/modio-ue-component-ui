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

#include "Brushes/SlateNoResource.h"
#include "CoreMinimal.h"
#include "ModioUISubsystem.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Interfaces/IModioUICommandMenu.h"
#include "UObject/NoExportTypes.h"

#include "ModioUIToggleModEnabledCommand.generated.h"

UCLASS()
class MODIOUICORE_API UModioUIToggleModEnabledCommand : public UObject, public IModioUICommandObject
{
	GENERATED_BODY()

protected:
	static inline const FString CommandID {TEXT("ToggleModEnabled")};

	UPROPERTY(Transient, BlueprintReadOnly, Category = "mod.io|UI|Commands|Toggle Mod Enabled",
			  meta = (BlueprintProtected))
	TObjectPtr<UObject> DataSource;

	virtual void SetDataSource_Implementation(UObject* InDataSource) override
	{
		DataSource = InDataSource;
	}

	virtual FText GetCommandDisplayString_Implementation() override
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
			{
				bool bWasParamsResolved = false;
				FModioModID TargetModID = IModioModInfoUIDetails::Execute_GetModID(DataSource);
				return UISubsystem->QueryIsModEnabled(TargetModID)
						   ? UModioUICommandLibrary::GetCommandText(CommandID, "DisableModAction", bWasParamsResolved)
						   : UModioUICommandLibrary::GetCommandText(CommandID, "EnableModAction", bWasParamsResolved);
			}
		}
		return FText::GetEmpty();
	}

	virtual FSlateBrush GetCommandBrush_Implementation() override
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
			{
				bool bWasParamsResolved = false;
				FModioModID TargetModID = IModioModInfoUIDetails::Execute_GetModID(DataSource);
				return UISubsystem->QueryIsModEnabled(TargetModID)
						   ? UModioUICommandLibrary::GetCommandBrush(CommandID, "DisableModAction", bWasParamsResolved)
						   : UModioUICommandLibrary::GetCommandBrush(CommandID, "EnableModAction", bWasParamsResolved);
			}
		}
		return FSlateNoResource {};
	}

	virtual void Execute_Implementation(UObject* Context) override
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
			{
				FModioModID TargetModID = IModioModInfoUIDetails::Execute_GetModID(DataSource);
				UISubsystem->RequestModEnabledStateChange(TargetModID, !UISubsystem->QueryIsModEnabled(TargetModID));
			}
		}
	}

	virtual bool CanExecute_Implementation(UObject* Context) override
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
			{
				FModioModID TargetModID = IModioModInfoUIDetails::Execute_GetModID(DataSource);
				if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
				{
					if (ModioSubsystem->QueryUserInstallations(true).Contains(TargetModID))
					{
						return true;
					}
				}
			}
		}
		return false;
	}
};
