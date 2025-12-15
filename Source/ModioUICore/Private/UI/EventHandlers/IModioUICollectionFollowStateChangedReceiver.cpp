/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUICollectionFollowStateChangedReceiver.h"

void IModioUICollectionFollowStateChangedReceiver::CollectionFollowStateChangedHandler(
	FModioModCollectionID CollectionID, bool bNewFollowstate)
{
	bRoutedCollectionFollowedChanged = false;
	TRACE_CPUPROFILER_EVENT_SCOPE_TEXT(
		*(Cast<UObject>(this)->GetFName().ToString().Append("_CollectionFollowStateChangedHandler")));
	NativeOnCollectionFollowStateChanged(CollectionID, bNewFollowstate);
	checkf(bRoutedCollectionFollowedChanged,
		   TEXT("Please call IModioUICollectionFollowStateChangedReceiver::NativeOnCollectionFollowStateChanged in your derived "
				"implementation in order to route notifications to blueprint."));
}

void IModioUICollectionFollowStateChangedReceiver::CollectionFollowStateChangedHandlerK2Helper(
	FModioModCollectionID CollectionID, bool bNewFollowState, TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUICollectionFollowStateChangedReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUICollectionFollowStateChangedReceiver* ConcretePtr =
				static_cast<IModioUICollectionFollowStateChangedReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->CollectionFollowStateChangedHandler(CollectionID, bNewFollowState);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnCollectionFollowStateChanged(ImplementingObject.Get(), CollectionID, bNewFollowState);
		}
	}
}

void IModioUICollectionFollowStateChangedReceiver::NativeOnCollectionFollowStateChanged(
	FModioModCollectionID ModCollectionID, bool bNewFollowState)
{
	bRoutedCollectionFollowedChanged = true;
	Execute_OnCollectionFollowStateChanged(Cast<UObject>(this), ModCollectionID, bNewFollowState);
}

void UModioUICollectionFollowStateChangedReceiverLibrary::RegisterCollectionFollowStateChangedReceiver(
	UObject* ObjectToRegister)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUICollectionFollowStateChangedReceiver>())
	{
		IModioUICollectionFollowStateChangedReceiver::RegisterFromK2(ObjectToRegister);
	}
}

void UModioUICollectionFollowStateChangedReceiverLibrary::DeregisterCollectionFollowStateChangedReceiver(
	UObject* ObjectToDeregister)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUICollectionFollowStateChangedReceiver>())
	{
		IModioUICollectionFollowStateChangedReceiver::DeregisterFromK2(ObjectToDeregister);
	}
}
