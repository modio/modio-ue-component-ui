/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIUserFollowingInfoReceiver.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUIUserFollowingInfoReceiver)

void IModioUIUserFollowingInfoReceiver::ListUserFollowingRequestHandler(FModioErrorCode ErrorCode,
																		TOptional<FModioUserList> FollowingList)
{
	bRoutedUIUserFollowingInfoReceiver = false;
	NativeOnListUserFollowingRequestCompleted(ErrorCode, FollowingList);
	checkf(bRoutedUIUserFollowingInfoReceiver,
		   TEXT("NativeOnListUserFollowingRequestCompleted should call Super::NativeOnListUserFollowingRequestCompleted "
				"to route events to blueprint"));
}

void IModioUIUserFollowingInfoReceiver::FollowUserRequestHandler(FModioErrorCode ErrorCode)
{
	bRoutedUIUserFollowingInfoReceiver = false;
	NativeOnFollowUserRequestCompleted(ErrorCode);
	checkf(bRoutedUIUserFollowingInfoReceiver,
		TEXT("NativeOnFollowUserRequestCompleted should call Super::NativeOnFollowUserRequestCompleted "
			 "to route events to blueprint"));
}

void IModioUIUserFollowingInfoReceiver::UnfollowUserRequestHandler(FModioErrorCode ErrorCode)
{
	bRoutedUIUserFollowingInfoReceiver = false;
	NativeOnUnfollowUserRequestCompleted(ErrorCode);
	checkf(bRoutedUIUserFollowingInfoReceiver,
		   TEXT("NativeOnUnfollowUserRequestCompleted should call Super::NativeOnUnfollowUserRequestCompleted "
				"to route events to blueprint"));
}

void IModioUIUserFollowingInfoReceiver::ListUserFollowingRequestK2Helper(FModioErrorCode ErrorCode,
																		 TOptional<FModioUserList> FollowingList,
																		 TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIUserFollowingInfoReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIUserFollowingInfoReceiver* ConcretePtr =
				static_cast<IModioUIUserFollowingInfoReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->ListUserFollowingRequestHandler(ErrorCode, FollowingList);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnListUserFollowingRequestCompleted(ImplementingObject.Get(), ErrorCode,
														FModioOptionalUserList(MoveTemp(FollowingList)));
		}
	}
}

void IModioUIUserFollowingInfoReceiver::FollowUserRequestK2Helper(FModioErrorCode ErrorCode,
																  TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIUserFollowingInfoReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIUserFollowingInfoReceiver* ConcretePtr =
				static_cast<IModioUIUserFollowingInfoReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->FollowUserRequestHandler(ErrorCode);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnFollowUserRequestCompleted(ImplementingObject.Get(), ErrorCode);
		}
	}
}

void IModioUIUserFollowingInfoReceiver::UnfollowUserRequestK2Helper(FModioErrorCode ErrorCode,
																	TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIUserFollowingInfoReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIUserFollowingInfoReceiver* ConcretePtr =
				static_cast<IModioUIUserFollowingInfoReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->UnfollowUserRequestHandler(ErrorCode);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnUnfollowUserRequestCompleted(ImplementingObject.Get(), ErrorCode);
		}
	}
}

void IModioUIUserFollowingInfoReceiver::NativeOnListUserFollowingRequestCompleted(
	FModioErrorCode ErrorCode, TOptional<FModioUserList> FollowingList)
{
	bRoutedUIUserFollowingInfoReceiver = true;
	Execute_OnListUserFollowingRequestCompleted(Cast<UObject>(this), ErrorCode, FModioOptionalUserList(MoveTemp(FollowingList)));
}

void IModioUIUserFollowingInfoReceiver::NativeOnFollowUserRequestCompleted(FModioErrorCode ErrorCode)
{
	bRoutedUIUserFollowingInfoReceiver = true;
	Execute_OnFollowUserRequestCompleted(Cast<UObject>(this), ErrorCode);
}

void IModioUIUserFollowingInfoReceiver::NativeOnUnfollowUserRequestCompleted(FModioErrorCode ErrorCode)
{
	bRoutedUIUserFollowingInfoReceiver = true;
	Execute_OnUnfollowUserRequestCompleted(Cast<UObject>(this), ErrorCode);
}

void UModioUIUserFollowingInfoReceiverLibrary::RegisterUserFollowingInfoReceiver(
	UObject* ObjectToRegister, EModioUIUserFollowingEventType EventType)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIUserFollowingInfoReceiver>())
	{
		IModioUIUserFollowingInfoReceiver::RegisterFromK2(ObjectToRegister, EventType);
	}
}

void UModioUIUserFollowingInfoReceiverLibrary::DeregisterUserFollowingInfoReceiver(
	UObject* ObjectToDeregister, EModioUIUserFollowingEventType EventType)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIUserFollowingInfoReceiver>())
	{
		IModioUIUserFollowingInfoReceiver::DeregisterFromK2(ObjectToDeregister, EventType);
	}
}
