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

#include "ModioUIManageModSubscriptionCommand.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioUIManageModSubscriptionCommand : public UObject, public IModioUICommandObject
{
	GENERATED_BODY()
protected:
	static inline const FString CommandID {TEXT("ManageModSubscription")};

	UPROPERTY(Transient, BlueprintReadOnly, Category = "mod.io|UI|Commands|Manage Mod Subscription",
			  meta = (BlueprintProtected))
	UObject* DataSource;

	virtual void SetDataSource_Implementation(UObject* InDataSource) override
	{
		DataSource = InDataSource;
	}

	virtual FText GetCommandDisplayString_Implementation() override
	{
		bool bUnused;
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
			{
				FModioModID CurrentModID = IModioModInfoUIDetails::Execute_GetModID(DataSource);
				if (Subsystem->QueryUserSubscriptions().Contains(CurrentModID))
				{
					return UModioUICommandLibrary::GetCommandText(CommandID, "UnsubscribeAction", bUnused);
				}
				else
				{
					return UModioUICommandLibrary::GetCommandText(CommandID, "SubscribeAction", bUnused);
				}
			}
		}
		return FText::GetEmpty();
	}

	virtual FSlateBrush GetCommandBrush_Implementation() override
	{
		bool bUnused;
		return UModioUICommandLibrary::GetCommandBrush(CommandID, "SubscribeAction", bUnused);
	}

	virtual void Execute_Implementation(UObject* Context) override
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
			{
				if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
				{
					FModioModID CurrentModID = IModioModInfoUIDetails::Execute_GetModID(DataSource);

					if (Subsystem->QueryUserSubscriptions().Contains(CurrentModID))
					{
						UISubsystem->RequestRemoveSubscriptionForModID(CurrentModID);
					}
					else
					{
						FModioModInfo CurrentModInfo = IModioModInfoUIDetails::Execute_GetFullModInfo(DataSource);
						if (CurrentModInfo.Dependencies)
						{
							UISubsystem->RequestShowDialog(EModioUIDialogType::ConfirmDependencyDownload, DataSource);
						}
						else
						{
							UISubsystem->RequestSubscriptionForModID(CurrentModID, false);
						}
					}
				}
			}
		}
	}

	virtual bool CanExecute_Implementation(UObject* Context) override
	{
		return (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()));
	}
};
