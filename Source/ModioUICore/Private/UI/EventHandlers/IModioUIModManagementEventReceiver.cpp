/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"

void IModioUIModManagementEventReceiver::ModManagementEventHandler(FModioModManagementEvent Event)
{
	bRoutedModManagementEvent = false;
	NativeOnModManagementEvent(Event);
	checkf(bRoutedModManagementEvent,
		   TEXT("Please call IModioUIModManagementEventReceiver::NativeOnModManagementEvent in your derived "
				"implementation in order to route notifications to blueprint"));
}


void IModioUIModManagementEventReceiver::ModManagementEventHandlerK2Helper(FModioModManagementEvent Event, TWeakObjectPtr<UObject> ImplementingObject) 
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIModManagementEventReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIModManagementEventReceiver* ConcretePtr =
				static_cast<IModioUIModManagementEventReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->ModManagementEventHandler(Event);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnModManagementEvent(ImplementingObject.Get(), Event);
		}
	}
}

void IModioUIModManagementEventReceiver::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	bRoutedModManagementEvent = true;
	Execute_OnModManagementEvent(Cast<UObject>(this), Event);
}

void UModioModManagementEventReceiverLibrary::RegisterModManagementEventReceiver(UObject* ObjectToRegister)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIModManagementEventReceiver>())
	{
		IModioUIModManagementEventReceiver::RegisterFromK2(ObjectToRegister);
	}
}

void UModioModManagementEventReceiverLibrary::DeregisterModManagementEventReceiver(UObject* ObjectToDeregister)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIModManagementEventReceiver>())
	{
		IModioUIModManagementEventReceiver::DeregisterFromK2(ObjectToDeregister);
	}
}