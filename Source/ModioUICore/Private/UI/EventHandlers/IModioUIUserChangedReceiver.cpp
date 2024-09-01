/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "Engine/Engine.h"
#include "Types/ModioUser.h"

void IModioUIUserChangedReceiver::UserChangedHandler(TOptional<FModioUser> NewUser)
{
	bRoutedUserChanged = false;
	NativeUserChanged(NewUser);
	checkf(bRoutedUserChanged, TEXT("Please call IModioUIUserChangedReceiver::NativeUserChanged in your derived "
									"implementation in order to route notifications to blueprint."));
}

void IModioUIUserChangedReceiver::UserChangedHandlerK2Helper(TOptional<FModioUser> NewUser,
															 TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIUserChangedReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIUserChangedReceiver* ConcretePtr = static_cast<IModioUIUserChangedReceiver*>(RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->UserChangedHandler(NewUser);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnUserChanged(ImplementingObject.Get(), FModioOptionalUser {NewUser});
		}
	}
}

void IModioUIUserChangedReceiver::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	bRoutedUserChanged = true;
	Execute_OnUserChanged(Cast<UObject>(this), FModioOptionalUser {NewUser});
}

void UModioUIUserChangedReceiverLibrary::RegisterUserChangedReceiver(UObject* ObjectToRegister)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIUserChangedReceiver>())
	{
		IModioUIUserChangedReceiver::RegisterFromK2(ObjectToRegister);
	}
}

void UModioUIUserChangedReceiverLibrary::DeregisterUserChangedReceiver(UObject* ObjectToDeregister)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIUserChangedReceiver>())
	{
		IModioUIUserChangedReceiver::DeregisterFromK2(ObjectToDeregister);
	}
}