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
#include "Types/ModioCommonTypes.h"
#include "UObject/Interface.h"

#include "IModioUISubscriptionsChangedReceiver.generated.h"

/**
 * @brief Interface for objects that want to receive events when a mod is enabled or disabled
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Event Handlers", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUISubscriptionsChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUISubscriptionsChangedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedSubscriptionsChanged = false;
	void SubscriptionsChangedHandler(FModioModID ModID, bool bNewSubscriptionState);

	static void SubscriptionsChangedHandlerK2Helper(FModioModID ModID, bool bNewSubscriptionState,
													TWeakObjectPtr<UObject> ImplementingObject);

	friend class UModioUISubscriptionsChangedReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMap;

	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->RegisterEventHandler<IModioUISubscriptionsChangedReceiver>(
				Subsystem->OnSubscriptionStatusChanged,
				&IModioUISubscriptionsChangedReceiver::SubscriptionsChangedHandler, *Cast<ImplementingClass>(this));
		}
	}

	template<typename ImplementingClass>
	void Deregister()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->DeregisterEventHandler<IModioUISubscriptionsChangedReceiver>(
				Subsystem->OnSubscriptionStatusChanged, *Cast<ImplementingClass>(this));
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
					Subsystem->OnSubscriptionStatusChanged,
					&IModioUISubscriptionsChangedReceiver::SubscriptionsChangedHandlerK2Helper, RegistrationMap,
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
					Subsystem->OnSubscriptionStatusChanged,
					&IModioUISubscriptionsChangedReceiver::SubscriptionsChangedHandlerK2Helper, RegistrationMap,
					TWeakObjectPtr<>(ObjectToDeregister));
			}
		}
	}

	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Subscriptions Changed",
			  meta = (BlueprintProtected))
	void OnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState);
};

UCLASS()
class MODIOUICORE_API UModioUISubscriptionsChangedReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for subscription changed UI events provided it implements
	/// UModioUISubscriptionsChangedReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterSubscriptionsChangedReceiver(UObject* ObjectToRegister);

	/// @brief UnRegisters a UObject as a receiver for subscription changed UI events provided it implements
	/// UModioUISubscriptionsChangedReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterSubscriptionsChangedReceiver(UObject* ObjectToDeregister);
};