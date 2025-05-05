/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIConnectivityChangedReceiver.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUIConnectivityChangedReceiver)

void IModioUIConnectivityChangedReceiver::ConnectivityChangedHandler(bool bNewConnectivityState)
{
	bRoutedConnectivityChanged = false;
	NativeOnConnectivityChanged(bNewConnectivityState);
	checkf(bRoutedConnectivityChanged,
		   TEXT("NativeOnListAllModsRequestCompleted should call Super::NativeOnListAllModsRequestCompleted "
				"to route events to blueprint"));
}

void IModioUIConnectivityChangedReceiver::ConnectivityChangedHandlerK2Helper(bool bNewConnectivityState,
																			 TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIConnectivityChangedReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIConnectivityChangedReceiver* ConcretePtr =
				static_cast<IModioUIConnectivityChangedReceiver*>(RawInterfacePtr);
			ConcretePtr->ConnectivityChangedHandler(bNewConnectivityState);
		}
		else
		{
			Execute_OnConnectivityChanged(ImplementingObject.Get(), bNewConnectivityState);
		}
	}
}

void IModioUIConnectivityChangedReceiver::NativeOnConnectivityChanged(bool bNewConnectivityState)
{
	bRoutedConnectivityChanged = true;
	Execute_OnConnectivityChanged(Cast<UObject>(this), bNewConnectivityState);
}

void UModioUIConnectivityChangedReceiverLibrary::RegisterConnectivityChangedReceiver(UObject* ObjectToRegister)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIConnectivityChangedReceiver>())
	{
		IModioUIConnectivityChangedReceiver::RegisterFromK2(ObjectToRegister);
	}
}

void UModioUIConnectivityChangedReceiverLibrary::DeregisterConnectivityChangedReceiver(UObject* ObjectToDeregister)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIConnectivityChangedReceiver>())
	{
		IModioUIConnectivityChangedReceiver::DeregisterFromK2(ObjectToDeregister);
	}
}