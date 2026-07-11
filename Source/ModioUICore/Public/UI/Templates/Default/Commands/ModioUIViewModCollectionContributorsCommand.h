/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
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
#include "UI/Interfaces/IModioModCollectionInfoUIDetails.h"
#include "UI/Interfaces/IModioUICommandMenu.h"

#include "ModioUIViewModCollectionContributorsCommand.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUICORE_API UModioUIViewModCollectionContributorsCommand : public UObject,
																	 public IModioUICommandObject
{
	GENERATED_BODY()

protected:
	static inline const FString CommandID {TEXT("ViewModCollectionContributors")};

	UPROPERTY(Transient, BlueprintReadOnly, Category = "mod.io|UI|Commands|Display Mod Collection Contributors",
			  meta = (BlueprintProtected))
	TObjectPtr<UObject> DataSource;

	virtual void SetDataSource_Implementation(UObject* InDataSource) override
	{
		DataSource = InDataSource;
	}

	virtual FText GetCommandDisplayString_Implementation() override
	{
		bool bUnused;
		return UModioUICommandLibrary::GetCommandText(CommandID, "ViewModCollectionContributorsAction", bUnused);
	}

	virtual FSlateBrush GetCommandBrush_Implementation() override
	{
		bool bUnused;
		return UModioUICommandLibrary::GetCommandBrush(CommandID, "ViewModCollectionContributorsAction", bUnused);
	}

	virtual bool CanExecute_Implementation(UObject* Context) override
	{
		return (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModCollectionInfoUIDetails::StaticClass()));
	}

	virtual void Execute_Implementation(UObject* Context) override
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModCollectionInfoUIDetails::StaticClass()))
			{
				UISubsystem->RequestShowDialog(EModioUIDialogType::ModCollectionContributors, DataSource);
			}
		}
	}
};
