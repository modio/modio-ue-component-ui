/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIDialogDisplayEventReceiver.h"
#include "Engine/Engine.h"
#include "ModioUISubsystem.h"

void IModioUIDialogDisplayEventReceiver::DialogDisplayEventHandler(EModioUIDialogType DialogRequested,
																   UObject* DataSource)
{
	bRoutedDialogDisplayEvent = false;
	NativeOnDialogDisplayEvent(DialogRequested, DataSource);
	checkf(bRoutedDialogDisplayEvent,
		   TEXT("Please call IModioUIDialogDisplayEventReceiver::NativeOnDialogDisplayEvent in your derived "
				"implementation in order to route notifications to blueprint."));
}

void IModioUIDialogDisplayEventReceiver::DialogDisplayEventHandlerK2Helper(EModioUIDialogType DialogRequested,
																		   UObject* DataSource,
																		   TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIDialogDisplayEventReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIDialogDisplayEventReceiver* ConcretePtr =
				static_cast<IModioUIDialogDisplayEventReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->DialogDisplayEventHandler(DialogRequested, DataSource);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnDialogDisplayEvent(ImplementingObject.Get(), DialogRequested, DataSource);
		}
	}
}

void IModioUIDialogDisplayEventReceiver::NativeOnDialogDisplayEvent(EModioUIDialogType DialogRequested,
																	UObject* DataSource)
{
	bRoutedDialogDisplayEvent = true;
	Execute_OnDialogDisplayEvent(Cast<UObject>(this), DialogRequested, DataSource);
}

void UModioUIDialogDisplayEventReceiverLibrary::RegisterDialogDisplayEventReceiver(UObject* ObjectToRegister)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIDialogDisplayEventReceiver>())
	{
		IModioUIDialogDisplayEventReceiver::RegisterFromK2(ObjectToRegister);
	}
}

void UModioUIDialogDisplayEventReceiverLibrary::DeregisterDialogDisplayEventReceiver(UObject* ObjectToDeregister)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIDialogDisplayEventReceiver>())
	{
		IModioUIDialogDisplayEventReceiver::DeregisterFromK2(ObjectToDeregister);
	}
}