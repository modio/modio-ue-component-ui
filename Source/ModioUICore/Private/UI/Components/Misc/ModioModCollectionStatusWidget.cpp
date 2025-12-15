/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioModCollectionStatusWidget.h"

#include "UI/Interfaces/IModioModCollectionInfoUIDetails.h"
#include "ModioSubsystem.h"
#include "ModioUISubsystem.h"

FModioModCollectionInfo UModioModCollectionStatusWidget::GetModCollectionInfo()
{
	return IModioModCollectionInfoUIDetails::Execute_GetFullModCollectionInfo(
		IModioUIDataSourceWidget::Execute_GetDataSource(this));
}

void UModioModCollectionStatusWidget::SetModCollectionFollowState(EModioModCollectionState NewState)
{
	CurrentCollectionFollowState = NewState;
	OnCollectionFollowStateChangedEvent.Broadcast(CurrentCollectionFollowState);
}

void UModioModCollectionStatusWidget::NativePreConstruct()
{
	IModioUISubscriptionsChangedReceiver::Register<UModioModCollectionStatusWidget>();
	IModioUICollectionFollowStateChangedReceiver::Register<UModioModCollectionStatusWidget>();
}

void UModioModCollectionStatusWidget::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	if (DataSource)
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			Subsystem->ListUserFollowedModCollectionsAsync(
				{}, FOnListFollowedModCollectionsDelegateFast::CreateLambda(
						[this](FModioErrorCode ec, TOptional<FModioModCollectionInfoList> FollowedCollections) {
							if (ec)
							{
								UE_LOG(ModioUICore, Error, TEXT("Failed to Check followed collections: %s"), *ec.GetErrorMessage());
							}
							else
							{
								// This is a temporary solution until we include a syncronous method of querying follow state
								bool bFoundCollection = false;
								FModioModCollectionID ID = GetModCollectionInfo().Id;
								for (const FModioModCollectionInfo& Info : FollowedCollections.GetValue().GetRawList())
								{
									if (Info.Id == ID)
									{
										bFoundCollection = true;
										break;
									}
								}
								SetModCollectionFollowState(bFoundCollection ? EModioModCollectionState::Followed : EModioModCollectionState::NotFollowed);
							}
						}));
		}
	}
}

void UModioModCollectionStatusWidget::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
	// Handle subscription state updates here when implemented
}

void UModioModCollectionStatusWidget::NativeOnCollectionFollowStateChanged(FModioModCollectionID ModCollectionID,
																		   bool bNewFollowState)
{
	IModioUICollectionFollowStateChangedReceiver::NativeOnCollectionFollowStateChanged(ModCollectionID,
																					   bNewFollowState);
	SetModCollectionFollowState((bNewFollowState ? EModioModCollectionState::Followed : EModioModCollectionState::NotFollowed));
}
