/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Templates/Default/Commands/ModioUIToggleModCollectionFollowedCommand.h"

void UModioUIToggleModCollectionFollowedCommand::NativeOnCollectionFollowStateChanged(
	FModioModCollectionID ModCollectionID, bool bNewFollowState)
{
	IModioUICollectionFollowStateChangedReceiver::NativeOnCollectionFollowStateChanged(ModCollectionID,
																					   bNewFollowState);

	if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModCollectionInfoUIDetails::StaticClass()) &&
		IModioModCollectionInfoUIDetails::Execute_GetModCollectionID(DataSource) == ModCollectionID)
	{
		bIsFollowed = bNewFollowState;
	}
}

void UModioUIToggleModCollectionFollowedCommand::RegisterFollowStateChangedReciever()
{
	IModioUICollectionFollowStateChangedReceiver::Register<UModioUIToggleModCollectionFollowedCommand>();
}
