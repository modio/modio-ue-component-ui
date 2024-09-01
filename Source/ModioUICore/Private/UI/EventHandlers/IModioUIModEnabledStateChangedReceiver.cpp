/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIModEnabledStateChangedReceiver.h"

void IModioUIModEnabledStateChangedReceiver::ModEnabledStateChangedHandler(FModioModID ModID, bool bNewEnabledState)
{
	bRoutedUIModEnabledStateChanged = false;
	NativeModEnabledStateChanged(ModID, bNewEnabledState);
	checkf(bRoutedUIModEnabledStateChanged,
		   TEXT("ModEnabledStateChangedHandler should call Super::ModEnabledStateChangedHandler"
				"to route events to blueprint"));
}

void IModioUIModEnabledStateChangedReceiver::ModEnabledStateChangedHandlerK2Helper(
	FModioModID ModID, bool bNewEnabledState, TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIModEnabledStateChangedReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIModEnabledStateChangedReceiver* ConcretePtr =
				static_cast<IModioUIModEnabledStateChangedReceiver*>(RawInterfacePtr);
			ConcretePtr->ModEnabledStateChangedHandler(ModID, bNewEnabledState);
		}
		else
		{
			Execute_OnModEnabledStateChanged(ImplementingObject.Get(), ModID, bNewEnabledState);
		}
	}
}

void IModioUIModEnabledStateChangedReceiver::NativeModEnabledStateChanged(FModioModID ModID, bool bNewEnabledState)
{
	bRoutedUIModEnabledStateChanged = true;
	Execute_OnModEnabledStateChanged(Cast<UObject>(this), ModID, bNewEnabledState);
}

void UModioUIModEnabledStateChangedReceiverLibrary::RegisterModEnabledStateChangedReceiver(UObject* ObjectToRegister)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIModEnabledStateChangedReceiver>())
	{
		IModioUIModEnabledStateChangedReceiver::RegisterFromK2(ObjectToRegister);
	}
}

void UModioUIModEnabledStateChangedReceiverLibrary::DeregisterModEnabledStateChangedReceiver(
	UObject* ObjectToDeregister)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIModEnabledStateChangedReceiver>())
	{
		IModioUIModEnabledStateChangedReceiver::DeregisterFromK2(ObjectToDeregister);
	}
}