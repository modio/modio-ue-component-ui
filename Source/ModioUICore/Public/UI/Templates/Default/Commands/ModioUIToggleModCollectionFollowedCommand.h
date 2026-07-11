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
#include "UI/EventHandlers/IModioUICollectionFollowStateChangedReceiver.h"

#include "ModioUIToggleModCollectionFollowedCommand.generated.h"

/**
 * @nodoc
 */
UCLASS()
class MODIOUICORE_API UModioUIToggleModCollectionFollowedCommand : public UObject,
																   public IModioUICommandObject,
																   public IModioUICollectionFollowStateChangedReceiver
{
	GENERATED_BODY()

	// IModioUICollectionFollowStateChangedReceiver interface
	virtual void NativeOnCollectionFollowStateChanged(FModioModCollectionID ModCollectionID, bool bNewFollowState) override;
	// end interface

public:

	void RegisterFollowStateChangedReciever();

protected:
	static inline const FString CommandID {TEXT("FollowModCollection")};
	bool bIsFollowed = false;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "mod.io|UI|Commands|View Mod Collection Contributors",
			  meta = (BlueprintProtected))
	TObjectPtr<UObject> DataSource;

	virtual void SetDataSource_Implementation(UObject* InDataSource) override
	{
		DataSource = InDataSource;
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			if (DataSource &&
				DataSource->GetClass()->ImplementsInterface(UModioModCollectionInfoUIDetails::StaticClass()))
			{
				FModioModCollectionID CurrentCollectionID =
					IModioModCollectionInfoUIDetails::Execute_GetModCollectionID(DataSource);
				Subsystem->ListUserFollowedModCollectionsAsync(
					{}, FOnListFollowedModCollectionsDelegateFast::CreateLambda(
							[&](FModioErrorCode ec, TOptional<FModioModCollectionInfoList> FollowedCollections) {
								if (ec)
								{
									UE_LOG(ModioUICore, Error, TEXT("Failed to query collection follow state: \"%s\""),
										   *ec.GetErrorMessage());
									bIsFollowed = false;
								}
								else
								{
									bIsFollowed =
										FollowedCollections.GetValue().GetRawList().Contains(CurrentCollectionID);
								}
							}));
			}
		}
	}

	virtual FText GetCommandDisplayString_Implementation() override
	{
		bool bUnused;
		if (bIsFollowed)
		{
			return UModioUICommandLibrary::GetCommandText(CommandID, "UnfollowCollectionAction", bUnused);
		}
		else
		{
			return UModioUICommandLibrary::GetCommandText(CommandID, "FollowCollectionAction", bUnused);
		}
	}

	virtual FSlateBrush GetCommandBrush_Implementation() override
	{
		bool bUnused;
		return UModioUICommandLibrary::GetCommandBrush(CommandID, "FollowAction", bUnused);
	}

	virtual bool CanExecute_Implementation(UObject* Context) override
	{
		return (DataSource &&
				DataSource->GetClass()->ImplementsInterface(UModioModCollectionInfoUIDetails::StaticClass()));
	}

	virtual void Execute_Implementation(UObject* Context) override
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
			{
				if (DataSource &&
					DataSource->GetClass()->ImplementsInterface(UModioModCollectionInfoUIDetails::StaticClass()))
				{
					FModioModCollectionID CurrentCollectionID =
						IModioModCollectionInfoUIDetails::Execute_GetModCollectionID(DataSource);

					if (bIsFollowed)
					{
						UISubsystem->RequestUnfollowModCollection(CurrentCollectionID);
					}
					else
					{
						UISubsystem->RequestFollowModCollection(CurrentCollectionID);
					}
				}
			}
		}
	}
	
};
