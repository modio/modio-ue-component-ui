/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIEntitlementRefreshEventReceiver.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUIEntitlementRefreshEventReceiver)

void IModioUIEntitlementRefreshEventReceiver::EntitlementRefreshEventHandler()
{
	bRoutedEntitlementRefreshEvent = false;
	NativeOnEntitlementRefreshEvent();
	checkf(bRoutedEntitlementRefreshEvent,
		   TEXT("Please call IModioUIEntitlementRefreshEventReceiver::NativeOnEntitlementRefreshEvent in your derived "
				"implementation in order to route notifications to blueprint."));
}

void IModioUIEntitlementRefreshEventReceiver::EntitlementRefreshEventHandlerK2Helper(TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIEntitlementRefreshEventReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIEntitlementRefreshEventReceiver* ConcretePtr =
				static_cast<IModioUIEntitlementRefreshEventReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->EntitlementRefreshEventHandler();
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnEntitlementRefreshEvent(ImplementingObject.Get());
		}
	}
}

void IModioUIEntitlementRefreshEventReceiver::NativeOnEntitlementRefreshEvent()
{
	bRoutedEntitlementRefreshEvent = true;
	Execute_OnEntitlementRefreshEvent(Cast<UObject>(this));
}

void UModioUIEntitlementRefreshEventReceiverLibrary::RegisterEntitlementRefreshEventReceiver(UObject* ObjectToRegister)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIEntitlementRefreshEventReceiver>())
	{
		IModioUIEntitlementRefreshEventReceiver::RegisterFromK2(ObjectToRegister);
	}
}

void UModioUIEntitlementRefreshEventReceiverLibrary::DeregisterEntitlementRefreshEventReceiver(UObject* ObjectToDeregister)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIEntitlementRefreshEventReceiver>())
	{
		IModioUIEntitlementRefreshEventReceiver::DeregisterFromK2(ObjectToDeregister);
	}
}