/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIUserAvatarDownloadCompletedReceiver.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"

void IModioUIUserAvatarDownloadCompletedReceiver::UserAvatarDownloadCompletedHandler(
	FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image)
{
	bRoutedAvatarDownloaded = false;
	NativeOnUserAvatarDownloadCompleted(ErrorCode, Image);
	checkf(bRoutedAvatarDownloaded,
		   TEXT("Please call IModioUIUserAvatarDownloadCompletedReceiver::NativeOnUserAvatarDownloadCompleted in your "
				"derived "
				"implementation in order to route notifications to blueprint."));
}

void IModioUIUserAvatarDownloadCompletedReceiver::NativeOnUserAvatarDownloadCompleted(
	FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image)
{
	bRoutedAvatarDownloaded = true;
	Execute_OnUserAvatarDownloadCompleted(Cast<UObject>(this), ErrorCode, FModioOptionalImage {Image});
}
