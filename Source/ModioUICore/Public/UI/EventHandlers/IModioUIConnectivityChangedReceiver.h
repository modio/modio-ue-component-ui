/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "ModioUISubsystem.h"
#include "UObject/Interface.h"

#include "IModioUIConnectivityChangedReceiver.generated.h"

/**
 * @brief Interface for classes that wish to be informed when there's a connectivity state change
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Events|Connectivity", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIConnectivityChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIConnectivityChangedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedConnectivityChanged = false;

	void ConnectivityChangedHandler(bool bNewConnectivityState);

	friend class UModioUIConnectivityChangedReceiverLibrary;

	static void ConnectivityChangedHandlerK2Helper(bool bNewConnectivityState,
												   TWeakObjectPtr<UObject> ImplementingObject);

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMap;

	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->RegisterEventHandler<IModioUIConnectivityChangedReceiver>(
				Subsystem->OnConnectivityChanged, &IModioUIConnectivityChangedReceiver::ConnectivityChangedHandler,
				*Cast<ImplementingClass>(this));

			// Update with the "current" connectivity state
			ConnectivityChangedHandler(Subsystem->bCurrentConnectivityState);
		}
	}

	static void RegisterFromK2(UObject* ObjectToRegister)
	{
		if (ObjectToRegister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->RegisterEventHandlerFromK2(
					Subsystem->OnConnectivityChanged,
					&IModioUIConnectivityChangedReceiver::ConnectivityChangedHandlerK2Helper, RegistrationMap,
					TWeakObjectPtr<>(ObjectToRegister));

				// Update with the "current" connectivity state
				ConnectivityChangedHandlerK2Helper(Subsystem->bCurrentConnectivityState,
												   TWeakObjectPtr<UObject>(ObjectToRegister));
			}
		}
	}

	static void DeregisterFromK2(UObject* ObjectToDeregister)
	{
		if (ObjectToDeregister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->DeregisterEventHandlerFromK2(
					Subsystem->OnConnectivityChanged,
					&IModioUIConnectivityChangedReceiver::ConnectivityChangedHandlerK2Helper, RegistrationMap,
					TWeakObjectPtr<>(ObjectToDeregister));
			}
		}
	}

	virtual void NativeOnConnectivityChanged(bool bNewConnectivityState);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Connectivity", meta = (BlueprintProtected))
	void OnConnectivityChanged(bool bNewConnectivityState);
};

UCLASS()
class MODIOUICORE_API UModioUIConnectivityChangedReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for connectivity changed events provided it implements
	/// UModioUIConnectivityChangedReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterConnectivityChangedReceiver(UObject* ObjectToRegister);

	/// @brief Deregisters a UObject as a receiver for connectivity changed events provided it implements
	/// UModioUIConnectivityChangedReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterConnectivityChangedReceiver(UObject* ObjectToDeregister);
};
