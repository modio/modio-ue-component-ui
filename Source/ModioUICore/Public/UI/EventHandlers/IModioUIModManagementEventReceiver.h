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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModioUISubsystem.h"
#include "Types/ModioModManagementEvent.h"
#include "UObject/Interface.h"

#include "IModioUIModManagementEventReceiver.generated.h"

/**
 * @brief Interface for classes that want to receive mod management events (install, uninstall, etc)
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Event Handlers", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIModManagementEventReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIModManagementEventReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedModManagementEvent = false;
	void ModManagementEventHandler(FModioModManagementEvent Event);
	static void ModManagementEventHandlerK2Helper(FModioModManagementEvent Event,
												  TWeakObjectPtr<UObject> ImplementingObject);
	friend class UModioModManagementEventReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<UObject>, FDelegateHandle> RegistrationMap;

	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->RegisterEventHandler<IModioUIModManagementEventReceiver>(
				Subsystem->OnModManagementEvent, &IModioUIModManagementEventReceiver::ModManagementEventHandler,
				*Cast<ImplementingClass>(this));
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
					Subsystem->OnModManagementEvent,
					&IModioUIModManagementEventReceiver::ModManagementEventHandlerK2Helper, RegistrationMap,
					TWeakObjectPtr<>(ObjectToRegister));
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
					Subsystem->OnModManagementEvent,
					&IModioUIModManagementEventReceiver::ModManagementEventHandlerK2Helper, RegistrationMap,
					TWeakObjectPtr<>(ObjectToDeregister));
			}
		}
	}

	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Mod Management", meta = (BlueprintProtected))
	void OnModManagementEvent(FModioModManagementEvent Event);
};

UCLASS()
class MODIOUICORE_API UModioModManagementEventReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for mod management events provided it implements
	/// UModioUIModManagementEventReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterModManagementEventReceiver(UObject* ObjectToRegister);

	/// @brief Deregisters a UObject as a receiver for mod management events provided it implements
	/// UModioUIModManagementEventReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterModManagementEventReceiver(UObject* ObjectToDeregister);
};
