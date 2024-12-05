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
#include "ModioUICore.h"
#include "Types/ModioCommonTypes.h"
#include "UObject/Interface.h"

#include "IModioUIEntitlementRefreshEventReceiver.generated.h"

/**
 * @brief Interface for classes that wish to know when mod.io UI elements request a dialog be displayed
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Events|Dialog Display", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIEntitlementRefreshEventReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIEntitlementRefreshEventReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedEntitlementRefreshEvent = false;
	void EntitlementRefreshEventHandler();

	static void EntitlementRefreshEventHandlerK2Helper(TWeakObjectPtr<UObject> ImplementingObject);

	friend class UModioUIEntitlementRefreshEventReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMap;

	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->RegisterEventHandler<IModioUIEntitlementRefreshEventReceiver>(
				Subsystem->OnEntitlementRefreshEvent,
				&IModioUIEntitlementRefreshEventReceiver::EntitlementRefreshEventHandler,
				*Cast<ImplementingClass>(this));
		}
	}

	template<typename ImplementingClass>
	void Deregister()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->DeregisterEventHandler<IModioUIEntitlementRefreshEventReceiver>(Subsystem->OnEntitlementRefreshEvent,
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
					Subsystem->OnEntitlementRefreshEvent,
					&IModioUIEntitlementRefreshEventReceiver::EntitlementRefreshEventHandlerK2Helper, RegistrationMap,
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
					Subsystem->OnEntitlementRefreshEvent,
					&IModioUIEntitlementRefreshEventReceiver::EntitlementRefreshEventHandlerK2Helper, RegistrationMap,
					TWeakObjectPtr<>(ObjectToDeregister));
			}
		}
	}

	virtual void NativeOnEntitlementRefreshEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Dialog Display", meta = (BlueprintProtected))
	void OnEntitlementRefreshEvent();
};

UCLASS()
class MODIOUICORE_API UModioUIEntitlementRefreshEventReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for requests to display mod.io UI dialogs provided it implements
	/// UModioUIEntitlementRefreshEventReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterEntitlementRefreshEventReceiver(UObject* ObjectToRegister);

	/// @brief Deregisters a UObject as a receiver for requests to display mod.io UI dialogs provided it implements
	/// UModioUIEntitlementRefreshEventReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterEntitlementRefreshEventReceiver(UObject* ObjectToDeregister);
};