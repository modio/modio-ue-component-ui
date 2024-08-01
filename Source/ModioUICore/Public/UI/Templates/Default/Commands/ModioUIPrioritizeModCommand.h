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

#include "CoreMinimal.h"
#include "ModioUISubsystem.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Interfaces/IModioUICommandMenu.h"
#include "UObject/NoExportTypes.h"

#include "ModioUIPrioritizeModCommand.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioUIPrioritizeModCommand : public UObject, public IModioUICommandObject
{
	GENERATED_BODY()
protected:
	static inline const FString CommandID {TEXT("PrioritizeMod")};

	UPROPERTY(Transient, BlueprintReadOnly, Category = "mod.io|UI|Commands|Prioritize Mod", meta = (BlueprintProtected))
	UObject* DataSource;

	virtual void SetDataSource_Implementation(UObject* InDataSource) override
	{
		DataSource = InDataSource;
	}

	virtual FText GetCommandDisplayString_Implementation() override
	{
		bool bUnused;
		return UModioUICommandLibrary::GetCommandText(CommandID, "PrioritizeModAction", bUnused);
	}

	virtual FSlateBrush GetCommandBrush_Implementation() override
	{
		bool bUnused;
		return UModioUICommandLibrary::GetCommandBrush(CommandID, "PrioritizeModAction", bUnused);
	}

	virtual void Execute_Implementation(UObject* Context) override
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
			{
				Subsystem->PrioritizeTransferForMod(IModioModInfoUIDetails::Execute_GetModID(DataSource));
			}
		}
	}

	virtual bool CanExecute_Implementation(UObject* Context) override
	{
		return (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()));
	}
};
