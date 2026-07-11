/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIMediaDownloadCompletedReceiver.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUIMediaDownloadCompletedReceiver)

void IModioUIMediaDownloadCompletedReceiver::GalleryImageDownloadHandler(FModioModID ModID, FModioErrorCode ErrorCode,
                                                                         int32 Index,
                                                                         TOptional<FModioImageWrapper> Image)
{
	bRoutedMediaDownloadCompleted = false;
	NativeOnModGalleryImageDownloadCompleted(ModID, ErrorCode, Index, Image);
	checkf(bRoutedMediaDownloadCompleted,
	       TEXT("NativeOnModGalleryImageDownloadCompleted should call Super::NativeOnModGalleryImageDownloadCompleted "
		       "to route events to blueprint"));
}

void IModioUIMediaDownloadCompletedReceiver::ModLogoDownloadHandler(FModioModID ModID, FModioErrorCode ErrorCode,
                                                                    TOptional<FModioImageWrapper> Image,
                                                                    EModioLogoSize LogoSize)
{
	bRoutedMediaDownloadCompleted = false;
	NativeOnModLogoDownloadCompleted(ModID, ErrorCode, Image, LogoSize);
	checkf(bRoutedMediaDownloadCompleted, TEXT("NativeOnModLogoDownloadCompleted should call "
		       "Super::NativeOnModLogoDownloadCompleted to route events to blueprint"));
}

void IModioUIMediaDownloadCompletedReceiver::CreatorAvatarDownloadHandler(FModioModID ModID, FModioErrorCode ErrorCode,
                                                                          TOptional<FModioImageWrapper> Image)
{
	bRoutedMediaDownloadCompleted = false;
	NativeOnModCreatorAvatarDownloadCompleted(ModID, ErrorCode, Image);
	checkf(bRoutedMediaDownloadCompleted,
	       TEXT("NativeOnModCreatorAvatarDownloadCompleted should call "
		       "Super::NativeOnModCreatorAvatarDownloadCompleted to route events to blueprint"));
}

void IModioUIMediaDownloadCompletedReceiver::ModCollectionLogoDownloadHandler(FModioModCollectionID ID,
                                                                              FModioErrorCode ErrorCode,
                                                                              TOptional<FModioImageWrapper> Image,
                                                                              EModioLogoSize LogoSize)
{
	bRoutedMediaDownloadCompleted = false;
	NativeOnModCollectionLogoDownloadCompleted(ID, ErrorCode, Image, LogoSize);
	checkf(bRoutedMediaDownloadCompleted,
	       TEXT("NativeOnModCreatorAvatarDownloadCompleted should call "
		       "Super::NativeOnModCreatorAvatarDownloadCompleted to route events to blueprint"));
}

void IModioUIMediaDownloadCompletedReceiver::ModCollectionCuratorAvatarDownloadHandler(FModioModCollectionID ID,
	FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image)
{
	bRoutedMediaDownloadCompleted = false;
	NativeOnModCollectionCuratorAvatarDownloadCompleted(ID, ErrorCode, Image);
	checkf(bRoutedMediaDownloadCompleted,
	       TEXT("NativeOnModCreatorAvatarDownloadCompleted should call "
		       "Super::NativeOnModCreatorAvatarDownloadCompleted to route events to blueprint"));
}

void IModioUIMediaDownloadCompletedReceiver::GalleryImageDownloadHandlerK2Helper(
	FModioModID ModID, FModioErrorCode ErrorCode, int32 Index, TOptional<FModioImageWrapper> Image,
	TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIMediaDownloadCompletedReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIMediaDownloadCompletedReceiver* ConcretePtr =
				static_cast<IModioUIMediaDownloadCompletedReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->GalleryImageDownloadHandler(ModID, ErrorCode, Index, Image);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnModGalleryImageDownloadCompleted(ImplementingObject.Get(), ModID, ErrorCode, Index,
			                                           FModioOptionalImage{Image});
		}
	}
}

void IModioUIMediaDownloadCompletedReceiver::ModLogoDownloadHandlerK2Helper(FModioModID ModID,
                                                                            FModioErrorCode ErrorCode,
                                                                            TOptional<FModioImageWrapper> Image,
                                                                            EModioLogoSize LogoSize,
                                                                            TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIMediaDownloadCompletedReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIMediaDownloadCompletedReceiver* ConcretePtr =
				static_cast<IModioUIMediaDownloadCompletedReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->ModLogoDownloadHandler(ModID, ErrorCode, Image, LogoSize);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnModLogoDownloadCompleted(ImplementingObject.Get(), ModID, ErrorCode, FModioOptionalImage{Image},
			                                   LogoSize);
		}
	}
}

void IModioUIMediaDownloadCompletedReceiver::CreatorAvatarDownloadHandlerK2Helper(
	FModioModID ModID, FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image,
	TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIMediaDownloadCompletedReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIMediaDownloadCompletedReceiver* ConcretePtr =
				static_cast<IModioUIMediaDownloadCompletedReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->CreatorAvatarDownloadHandler(ModID, ErrorCode, Image);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnModCreatorAvatarDownloadCompleted(ImplementingObject.Get(), ModID, ErrorCode,
			                                            FModioOptionalImage{Image});
		}
	}
}

void IModioUIMediaDownloadCompletedReceiver::ModCollectionLogoDownloadHandlerK2Helper(FModioModCollectionID ID,
	FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image, EModioLogoSize LogoSize,
	TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIMediaDownloadCompletedReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIMediaDownloadCompletedReceiver* ConcretePtr =
				static_cast<IModioUIMediaDownloadCompletedReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->ModCollectionLogoDownloadHandler(ID, ErrorCode, Image, LogoSize);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnModCollectionLogoDownloadCompleted(ImplementingObject.Get(), ID, ErrorCode,
			                                             FModioOptionalImage{Image},
			                                             LogoSize);
		}
	}
}

void IModioUIMediaDownloadCompletedReceiver::ModCollectionCuratorAvatarDownloadHandlerK2Helper(FModioModCollectionID ID,
	FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image, TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIMediaDownloadCompletedReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIMediaDownloadCompletedReceiver* ConcretePtr =
				static_cast<IModioUIMediaDownloadCompletedReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->ModCollectionCuratorAvatarDownloadHandler(ID, ErrorCode, Image);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnModCollectionCuratorAvatarDownloadCompleted(ImplementingObject.Get(), ID, ErrorCode,
			                                                      FModioOptionalImage{Image});
		}
	}
}

void IModioUIMediaDownloadCompletedReceiver::NativeOnModLogoDownloadCompleted(FModioModID ModID,
                                                                              FModioErrorCode ErrorCode,
                                                                              TOptional<FModioImageWrapper> Image,
                                                                              EModioLogoSize LogoSize)
{
	bRoutedMediaDownloadCompleted = true;
	Execute_OnModLogoDownloadCompleted(Cast<UObject>(this), ModID, ErrorCode, FModioOptionalImage{Image}, LogoSize);
}

void IModioUIMediaDownloadCompletedReceiver::NativeOnModGalleryImageDownloadCompleted(
	FModioModID ModID, FModioErrorCode ErrorCode, int32 ImageIndex, TOptional<FModioImageWrapper> Image)
{
	bRoutedMediaDownloadCompleted = true;
	Execute_OnModGalleryImageDownloadCompleted(Cast<UObject>(this), ModID, ErrorCode, ImageIndex,
	                                           FModioOptionalImage{Image});
}

void IModioUIMediaDownloadCompletedReceiver::NativeOnModCreatorAvatarDownloadCompleted(
	FModioModID ModID, FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image)
{
	bRoutedMediaDownloadCompleted = true;
	Execute_OnModCreatorAvatarDownloadCompleted(Cast<UObject>(this), ModID, ErrorCode, FModioOptionalImage{Image});
}

void IModioUIMediaDownloadCompletedReceiver::NativeOnModCollectionLogoDownloadCompleted(
	FModioModCollectionID ModCollectionID, FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image,
	EModioLogoSize LogoSize)
{
	bRoutedMediaDownloadCompleted = true;
	Execute_OnModCollectionLogoDownloadCompleted(Cast<UObject>(this), ModCollectionID, ErrorCode,
	                                             FModioOptionalImage{Image}, LogoSize);
}

void IModioUIMediaDownloadCompletedReceiver::NativeOnModCollectionCuratorAvatarDownloadCompleted(
	FModioModCollectionID ModCollectionID, FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image)
{
	bRoutedMediaDownloadCompleted = true;
	Execute_OnModCollectionCuratorAvatarDownloadCompleted(Cast<UObject>(this), ModCollectionID, ErrorCode,
	                                                      FModioOptionalImage{Image});
}

void UModioUIMediaDownloadCompletedReceiverLibrary::RegisterMediaDownloadCompletedReceiver(
	UObject* ObjectToRegister, EModioUIMediaDownloadEventType DownloadTypes)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIMediaDownloadCompletedReceiver>())
	{
		IModioUIMediaDownloadCompletedReceiver::RegisterFromK2(ObjectToRegister, DownloadTypes);
	}
}

void UModioUIMediaDownloadCompletedReceiverLibrary::DeregisterMediaDownloadCompletedReceiver(
	UObject* ObjectToDeregister, EModioUIMediaDownloadEventType DownloadTypes)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIMediaDownloadCompletedReceiver>())
	{
		IModioUIMediaDownloadCompletedReceiver::DeregisterFromK2(ObjectToDeregister, DownloadTypes);
	}
}