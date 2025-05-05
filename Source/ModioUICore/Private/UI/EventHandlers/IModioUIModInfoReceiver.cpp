/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIModInfoReceiver.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUIModInfoReceiver)

void IModioUIModInfoReceiver::ModInfoRequestHandler(FModioModID ModID, FModioErrorCode ErrorCode,
													TOptional<FModioModInfo> Info)
{
	bRoutedUIModInfoReceiver = false;
	NativeOnModInfoRequestCompleted(ModID, ErrorCode, Info);
	checkf(bRoutedUIModInfoReceiver,
		   TEXT("NativeOnModInfoRequestCompleted should call Super::NativeOnModInfoRequestCompleted "
				"to route events to blueprint"));
}

void IModioUIModInfoReceiver::ListAllModsRequestHandler(FString RequestIdentifier, FModioErrorCode ErrorCode,
														TOptional<FModioModInfoList> List)
{
	bRoutedUIModInfoReceiver = false;
	NativeOnListAllModsRequestCompleted(RequestIdentifier, ErrorCode, List);
	checkf(bRoutedUIModInfoReceiver,
		   TEXT("NativeOnListAllModsRequestCompleted should call Super::NativeOnListAllModsRequestCompleted "
				"to route events to blueprint"));
}

void IModioUIModInfoReceiver::ModInfoRequestHandlerK2Helper(FModioModID ModID, FModioErrorCode ErrorCode,
															TOptional<FModioModInfo> Info,
															TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr = ImplementingObject->GetNativeInterfaceAddress(UModioUIModInfoReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIModInfoReceiver* ConcretePtr = static_cast<IModioUIModInfoReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->ModInfoRequestHandler(ModID, ErrorCode, Info);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnModInfoRequestCompleted(ImplementingObject.Get(), ModID, ErrorCode, FModioOptionalModInfo {Info});
		}
	}
}

void IModioUIModInfoReceiver::ListAllModsRequestHandlerK2Helper(FString RequestIdentifier, FModioErrorCode ErrorCode,
																TOptional<FModioModInfoList> List,
																TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		void* RawInterfacePtr = ImplementingObject->GetNativeInterfaceAddress(UModioUIModInfoReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIModInfoReceiver* ConcretePtr = static_cast<IModioUIModInfoReceiver*>(RawInterfacePtr);
			ConcretePtr->ListAllModsRequestHandler(RequestIdentifier, ErrorCode, List);
		}
		else
		{
			Execute_OnListAllModsRequestCompleted(ImplementingObject.Get(), RequestIdentifier, ErrorCode,
												  FModioOptionalModInfoList(MoveTemp(List)));
		}
	}
}

void IModioUIModInfoReceiver::NativeOnModInfoRequestCompleted(FModioModID ModID, FModioErrorCode ErrorCode,
															  TOptional<FModioModInfo> Info)
{
	bRoutedUIModInfoReceiver = true;
	Execute_OnModInfoRequestCompleted(Cast<UObject>(this), ModID, ErrorCode, FModioOptionalModInfo {Info});
}
void IModioUIModInfoReceiver::NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ErrorCode,
																  TOptional<FModioModInfoList> List)
{
	bRoutedUIModInfoReceiver = true;
	Execute_OnListAllModsRequestCompleted(Cast<UObject>(this), RequestIdentifier, ErrorCode,
										  FModioOptionalModInfoList(MoveTemp(List)));
}

void UModioUIModInfoReceiverLibrary::RegisterModInfoReceiver(UObject* ObjectToRegister,
															 EModioUIModInfoEventType EventType)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIModInfoReceiver>())
	{
		IModioUIModInfoReceiver::RegisterFromK2(ObjectToRegister, EventType);
	}
}

void UModioUIModInfoReceiverLibrary::DeregisterModInfoReceiver(UObject* ObjectToDeregister,
															   EModioUIModInfoEventType EventType)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIModInfoReceiver>())
	{
		IModioUIModInfoReceiver::DeregisterFromK2(ObjectToDeregister, EventType);
	}
}