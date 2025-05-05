/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "Engine/Engine.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUISubscriptionsChangedReceiver)

void IModioUISubscriptionsChangedReceiver::SubscriptionsChangedHandler(FModioModID ModID, bool bNewSubscriptionState)
{
	bRoutedSubscriptionsChanged = false;
	TRACE_CPUPROFILER_EVENT_SCOPE_TEXT(
		*(Cast<UObject>(this)->GetFName().ToString().Append("_SubscriptionsChangedHandler")));
	NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
	checkf(bRoutedSubscriptionsChanged,
		   TEXT("Please call IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged in your derived "
				"implementation in order to route notifications to blueprint."));
}

void IModioUISubscriptionsChangedReceiver::SubscriptionsChangedHandlerK2Helper(
	FModioModID ModID, bool bNewSubscriptionState, TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUISubscriptionsChangedReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUISubscriptionsChangedReceiver* ConcretePtr =
				static_cast<IModioUISubscriptionsChangedReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->SubscriptionsChangedHandler(ModID, bNewSubscriptionState);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnSubscriptionsChanged(ImplementingObject.Get(), ModID, bNewSubscriptionState);
		}
	}
}

void IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	bRoutedSubscriptionsChanged = true;
	Execute_OnSubscriptionsChanged(Cast<UObject>(this), ModID, bNewSubscriptionState);
}

void UModioUISubscriptionsChangedReceiverLibrary::RegisterSubscriptionsChangedReceiver(UObject* ObjectToRegister)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUISubscriptionsChangedReceiver>())
	{
		IModioUISubscriptionsChangedReceiver::RegisterFromK2(ObjectToRegister);
	}
}

void UModioUISubscriptionsChangedReceiverLibrary::DeregisterSubscriptionsChangedReceiver(UObject* ObjectToDeregister)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUISubscriptionsChangedReceiver>())
	{
		IModioUISubscriptionsChangedReceiver::DeregisterFromK2(ObjectToDeregister);
	}
}