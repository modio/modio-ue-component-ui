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

#include "ModioUIPurchaseModCommand.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioUIPurchaseModCommand: public UObject, public IModioUICommandObject
{
	GENERATED_BODY()
protected:
	static inline const FString CommandID {TEXT("PurchaseMod")};

	UPROPERTY(Transient, BlueprintReadOnly, Category = "mod.io|UI|Commands|Purchase Mod", meta = (BlueprintProtected))
	UObject* DataSource;

	virtual void SetDataSource_Implementation(UObject* InDataSource) override
	{
		DataSource = InDataSource;
	}

	virtual FText GetCommandDisplayString_Implementation() override
	{
		bool bUnused;
		return UModioUICommandLibrary::GetCommandText(CommandID, "PurchaseModAction", bUnused);
	}

	virtual FSlateBrush GetCommandBrush_Implementation() override
	{
		bool bUnused;
		return UModioUICommandLibrary::GetCommandBrush(CommandID, "PurchaseModAction", bUnused);
	}

	virtual void Execute_Implementation(UObject* Context) override
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
			{
				UISubsystem->RequestShowDialog(EModioUIDialogType::PurchaseMod, DataSource);
			}
		}
	}

	virtual bool CanExecute_Implementation(UObject* Context) override
	{
		return (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()));
	}
};
