/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUITokenPackReceiver.h"

void IModioUITokenPackReceiver::TokenPackRequestHandler(FModioTokenPackID TokenPackID, FModioErrorCode ErrorCode,
	TOptional<FModioTokenPack> Pack)
{
	bRoutedUITokenPackReceiver = false;
	NativeOnTokenPackRequestCompleted(TokenPackID, ErrorCode, Pack);
	checkf(bRoutedUITokenPackReceiver,
		   TEXT("NativeOnTokenPackRequestCompleted should call Super::NativeOnTokenPackRequestCompleted "
				"to route events to blueprint"));
}

void IModioUITokenPackReceiver::ListAllTokenPacksRequestHandler(FModioErrorCode ErrorCode,
	TOptional<FModioTokenPackList> List)
{
	bRoutedUITokenPackReceiver = false;
	NativeOnListAllTokenPacksRequestCompleted(ErrorCode, List);
	checkf(bRoutedUITokenPackReceiver,
		   TEXT("NativeOnListAllTokenPacksRequestCompleted should call Super::NativeOnListAllTokenPacksRequestCompleted "
				"to route events to blueprint"));
}

void IModioUITokenPackReceiver::TokenPackRequestHandlerK2Helper(FModioTokenPackID TokenPackID,
	FModioErrorCode ErrorCode, TOptional<FModioTokenPack> Pack, TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr = ImplementingObject->GetNativeInterfaceAddress(UModioUITokenPackReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUITokenPackReceiver* ConcretePtr = static_cast<IModioUITokenPackReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->TokenPackRequestHandler(TokenPackID, ErrorCode, Pack);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnTokenPackRequestCompleted(ImplementingObject.Get(), TokenPackID, ErrorCode, FModioOptionalTokenPack {Pack});
		}
	}
}

void IModioUITokenPackReceiver::ListAllTokenPacksRequestHandlerK2Helper(FModioErrorCode ErrorCode, TOptional<FModioTokenPackList> List,
																		TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		void* RawInterfacePtr = ImplementingObject->GetNativeInterfaceAddress(UModioUITokenPackReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUITokenPackReceiver* ConcretePtr = static_cast<IModioUITokenPackReceiver*>(RawInterfacePtr);
			ConcretePtr->ListAllTokenPacksRequestHandler(ErrorCode, List);
		}
		else
		{
			Execute_OnListAllTokenPacksRequestCompleted(ImplementingObject.Get(), ErrorCode,
												  FModioOptionalTokenPackList(MoveTemp(List)));
		}
	}
}

void IModioUITokenPackReceiver::NativeOnTokenPackRequestCompleted(FModioTokenPackID TokenPackID,
	FModioErrorCode ErrorCode, TOptional<FModioTokenPack> Pack)
{
	bRoutedUITokenPackReceiver = true;
	Execute_OnTokenPackRequestCompleted(Cast<UObject>(this), TokenPackID, ErrorCode, FModioOptionalTokenPack {Pack});
}

void IModioUITokenPackReceiver::NativeOnListAllTokenPacksRequestCompleted(FModioErrorCode ErrorCode, TOptional<FModioTokenPackList> List)
{
	bRoutedUITokenPackReceiver = true;
	Execute_OnListAllTokenPacksRequestCompleted(Cast<UObject>(this), ErrorCode,
										  FModioOptionalTokenPackList(MoveTemp(List)));
}

void UModioUITokenPackReceiverLibrary::RegisterTokenPackReceiver(UObject* ObjectToRegister,
	EModioUITokenPackEventType EventType)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUITokenPackReceiver>())
	{
		IModioUITokenPackReceiver::RegisterFromK2(ObjectToRegister, EventType);
	}
}

void UModioUITokenPackReceiverLibrary::DeregisterTokenPackReceiver(UObject* ObjectToDeregister,
	EModioUITokenPackEventType EventType)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUITokenPackReceiver>())
	{
		IModioUITokenPackReceiver::DeregisterFromK2(ObjectToDeregister, EventType);
	}
}