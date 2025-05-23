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

#include "ModioUIReportModCommand.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioUIReportModCommand : public UObject, public IModioUICommandObject
{
	GENERATED_BODY()
protected:
	static inline const FString CommandID {TEXT("ReportMod")};

	UPROPERTY(Transient, BlueprintReadOnly, Category = "mod.io|UI|Commands|Display Filtered Mods",
			  meta = (BlueprintProtected))
	TObjectPtr<UObject> DataSource;

	virtual void SetDataSource_Implementation(UObject* InDataSource) override
	{
		DataSource = InDataSource;
	}

	virtual FText GetCommandDisplayString_Implementation() override
	{
		bool bUnused;
		return UModioUICommandLibrary::GetCommandText(CommandID, "ReportModAction", bUnused);
	}

	virtual FSlateBrush GetCommandBrush_Implementation() override
	{
		bool bUnused;
		return UModioUICommandLibrary::GetCommandBrush(CommandID, "ReportModAction", bUnused);
	}

	virtual void Execute_Implementation(UObject* Context) override
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
			{
				UISubsystem->RequestShowDialog(EModioUIDialogType::ReportMod, DataSource);
			}
		}
	}

	virtual bool CanExecute_Implementation(UObject* Context) override
	{
		return (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()));
	}
};
