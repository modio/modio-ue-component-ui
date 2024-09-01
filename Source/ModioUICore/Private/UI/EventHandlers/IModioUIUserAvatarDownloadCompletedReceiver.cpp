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

void IModioUIUserAvatarDownloadCompletedReceiver::UserAvatarDownloadCompletedHandlerK2Helper(
	FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image, TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIUserAvatarDownloadCompletedReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIUserAvatarDownloadCompletedReceiver* ConcretePtr =
				static_cast<IModioUIUserAvatarDownloadCompletedReceiver*>(RawInterfacePtr);
			ConcretePtr->UserAvatarDownloadCompletedHandler(ErrorCode, Image);
		}
		else
		{
			Execute_OnUserAvatarDownloadCompleted(ImplementingObject.Get(), ErrorCode, FModioOptionalImage {Image});
		}
	}
}

void UModioUIUserAvatarDownloadCompletedReceiverLibrary::RegisterUserAvatarDownloadCompletedReceiver(
	UObject* ObjectToRegister)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIUserAvatarDownloadCompletedReceiver>())
	{
		IModioUIUserAvatarDownloadCompletedReceiver::RegisterFromK2(ObjectToRegister);
	}
}

void UModioUIUserAvatarDownloadCompletedReceiverLibrary::DeregisterUserAvatarDownloadCompletedReceiver(
	UObject* ObjectToDeregister)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIUserAvatarDownloadCompletedReceiver>())
	{
		IModioUIUserAvatarDownloadCompletedReceiver::DeregisterFromK2(ObjectToDeregister);
	}
}