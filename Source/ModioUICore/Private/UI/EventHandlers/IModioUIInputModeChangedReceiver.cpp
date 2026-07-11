/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIInputModeChangedReceiver.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUIInputModeChangedReceiver)

void IModioUIInputModeChangedReceiver::InputModeChangedHandler(EModioUIInputMode NewInputModeState)
{
	bRoutedInputModeChanged = false;
	NativeOnInputModeChanged(NewInputModeState);
	checkf(bRoutedInputModeChanged,
		   TEXT("NativeOnListAllModsRequestCompleted should call Super::NativeOnListAllModsRequestCompleted "
				"to route events to blueprint"));
}

void IModioUIInputModeChangedReceiver::InputModeChangedHandlerK2Helper(EModioUIInputMode NewInputModeState,
																			 TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIInputModeChangedReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIInputModeChangedReceiver* ConcretePtr =
				static_cast<IModioUIInputModeChangedReceiver*>(RawInterfacePtr);
			ConcretePtr->InputModeChangedHandler(NewInputModeState);
		}
		else
		{
			Execute_OnInputModeChanged(ImplementingObject.Get(), NewInputModeState);
		}
	}
}

void IModioUIInputModeChangedReceiver::NativeOnInputModeChanged(EModioUIInputMode NewInputModeState)
{
	bRoutedInputModeChanged = true;
	Execute_OnInputModeChanged(Cast<UObject>(this), NewInputModeState);
}

void UModioUIInputModeChangedReceiverLibrary::RegisterInputModeChangedReceiver(UObject* ObjectToRegister)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIInputModeChangedReceiver>())
	{
		IModioUIInputModeChangedReceiver::RegisterFromK2(ObjectToRegister);
	}
}

void UModioUIInputModeChangedReceiverLibrary::DeregisterInputModeChangedReceiver(UObject* ObjectToDeregister)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIInputModeChangedReceiver>())
	{
		IModioUIInputModeChangedReceiver::DeregisterFromK2(ObjectToDeregister);
	}
}