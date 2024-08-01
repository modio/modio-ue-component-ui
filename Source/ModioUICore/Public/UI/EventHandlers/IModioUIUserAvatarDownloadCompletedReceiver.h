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

#include "ModioUISubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioImageWrapper.h"
#include "Types/ModioUser.h"
#include "UObject/Interface.h"

#include "IModioUIUserAvatarDownloadCompletedReceiver.generated.h"

/**
 * @brief Interface for classes that want to receive events when a user's avatar has been downloaded
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Event Handlers", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIUserAvatarDownloadCompletedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIUserAvatarDownloadCompletedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedAvatarDownloaded = false;
	void UserAvatarDownloadCompletedHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image);

protected:
	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->OnUserAvatarDownloadCompleted.AddUObject(
				Cast<ImplementingClass>(this),
				&IModioUIUserAvatarDownloadCompletedReceiver::UserAvatarDownloadCompletedHandler);
		}
	}

	virtual void NativeOnUserAvatarDownloadCompleted(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Avatar Download", meta = (BlueprintProtected))
	void OnUserAvatarDownloadCompleted(FModioErrorCode ErrorCode, FModioOptionalImage Image);
};
