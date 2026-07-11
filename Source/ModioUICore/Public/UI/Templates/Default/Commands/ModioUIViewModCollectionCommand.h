/*
* Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
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
#include "UI/Interfaces/IModioUICommandMenu.h"
#include "UI/Interfaces/IModioModCollectionInfoUIDetails.h"
#include "UObject/Object.h"
#include "ModioUIViewModCollectionCommand.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUICORE_API UModioUIViewModCollectionCommand : public UObject, public IModioUICommandObject
{
	GENERATED_BODY()

protected:
	static inline const FString CommandID{TEXT("ViewModCollection")};

	UPROPERTY(Transient, BlueprintReadOnly, Category = "mod.io|UI|Commands|View Mod Collection",
		meta = (BlueprintProtected))
	TObjectPtr<UObject> DataSource;

	virtual void SetDataSource_Implementation(UObject* InDataSource) override
	{
		DataSource = InDataSource;
	}

	virtual FText GetCommandDisplayString_Implementation() override
	{
		bool bUnused;
		return UModioUICommandLibrary::GetCommandText(CommandID, "ViewModCollectionAction", bUnused);
	}

	virtual FSlateBrush GetCommandBrush_Implementation() override
	{
		bool bUnused;
		return UModioUICommandLibrary::GetCommandBrush(CommandID, "ViewModCollectionAction", bUnused);
	}

	virtual void Execute_Implementation(UObject* Context) override
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (DataSource && DataSource->GetClass()->ImplementsInterface(
				    UModioModCollectionInfoUIDetails::StaticClass()))
			{
				UISubsystem->RequestShowDialog(EModioUIDialogType::ModCollectionDetails, DataSource);
			}
		}
	}

	virtual bool CanExecute_Implementation(UObject* Context) override
	{
		return (DataSource && DataSource->GetClass()->ImplementsInterface(
			        UModioModCollectionInfoUIDetails::StaticClass()));
	}
};