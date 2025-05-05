/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIWalletBalanceUpdatedEventReceiver.h"
#include "Engine/Engine.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUIWalletBalanceUpdatedEventReceiver)

void IModioUIWalletBalanceUpdatedEventReceiver::WalletBalanceUpdatedHandler(uint64 NewBalance)
{
	bRoutedWalletBalanceUpdated = false;
	NativeOnWalletBalanceUpdated(NewBalance);
	checkf(bRoutedWalletBalanceUpdated,
		   TEXT("Please call IModioUIWalletBalanceUpdatedEventReceiver::NativeOnWalletBalanceUpdated in your derived "
				"implementation in order to route notifications to blueprint."));
}

void IModioUIWalletBalanceUpdatedEventReceiver::WalletBalanceUpdatedHandlerK2Helper(
	uint64 NewBalance, TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIWalletBalanceUpdatedEventReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIWalletBalanceUpdatedEventReceiver* ConcretePtr =
				static_cast<IModioUIWalletBalanceUpdatedEventReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->WalletBalanceUpdatedHandler(NewBalance);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnWalletBalanceUpdated(ImplementingObject.Get(), FModioUnsigned64(NewBalance));
		}
	}
}

void IModioUIWalletBalanceUpdatedEventReceiver::NativeOnWalletBalanceUpdated(uint64 NewBalance)
{
	bRoutedWalletBalanceUpdated = true;
	Execute_OnWalletBalanceUpdated(Cast<UObject>(this), FModioUnsigned64(NewBalance));
}

void UModioUIWalletBalanceUpdatedEventReceiverLibrary::RegisterWalletBalanceUpdatedEventReceiver(
	UObject* ObjectToRegister)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIWalletBalanceUpdatedEventReceiver>())
	{
		IModioUIWalletBalanceUpdatedEventReceiver::RegisterFromK2(ObjectToRegister);
	}
}

void UModioUIWalletBalanceUpdatedEventReceiverLibrary::DeregisterWalletBalanceUpdatedEventReceiver(
	UObject* ObjectToDeregister)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIWalletBalanceUpdatedEventReceiver>())
	{
		IModioUIWalletBalanceUpdatedEventReceiver::DeregisterFromK2(ObjectToDeregister);
	}
}