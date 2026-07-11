/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "IModioUICollectionFollowStateChangedReceiver.generated.h"


/**
 * @brief Interface for objects that want to receive events when a mod collection is followed or unfollowed
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Event Handlers", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUICollectionFollowStateChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUICollectionFollowStateChangedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedCollectionFollowedChanged = false;
	void CollectionFollowStateChangedHandler(FModioModCollectionID CollectionID, bool bNewFollowstate);

	static void CollectionFollowStateChangedHandlerK2Helper(FModioModCollectionID CollectionID, bool bNewFollowState,
	                                                        TWeakObjectPtr<UObject> ImplementingObject);

	friend class UModioUICollectionFollowStateChangedReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMap;

	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->RegisterEventHandler<IModioUICollectionFollowStateChangedReceiver>(
				Subsystem->OnModCollectionFollowStateChanged,
				&IModioUICollectionFollowStateChangedReceiver::CollectionFollowStateChangedHandler,
				*Cast<ImplementingClass>(this));
		}
	}

	template<typename ImplementingClass>
	void Deregister()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->DeregisterEventHandler<IModioUICollectionFollowStateChangedReceiver>(
				Subsystem->OnModCollectionFollowStateChanged, *Cast<ImplementingClass>(this));
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
					Subsystem->OnModCollectionFollowStateChanged,
					&IModioUICollectionFollowStateChangedReceiver::CollectionFollowStateChangedHandlerK2Helper,
					RegistrationMap,
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
					Subsystem->OnModCollectionFollowStateChanged,
					&IModioUICollectionFollowStateChangedReceiver::CollectionFollowStateChangedHandlerK2Helper,
					RegistrationMap,
					TWeakObjectPtr<>(ObjectToDeregister));
			}
		}
	}

	virtual void NativeOnCollectionFollowStateChanged(FModioModCollectionID ModCollectionID, bool bNewFollowState);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Collection Follow State Changed",
		meta = (BlueprintProtected))
	void OnCollectionFollowStateChanged(FModioModCollectionID ModCollectionID, bool bNewFollowState);
};

/**
 * 
 */
UCLASS()
class MODIOUICORE_API UModioUICollectionFollowStateChangedReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for collection follow state changed UI events provided it implements
	/// UModioUICollectionFollowStateChangedReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterCollectionFollowStateChangedReceiver(UObject* ObjectToRegister);

	/// @brief UnRegisters a UObject as a receiver for collection follow state changed UI events provided it implements
	/// UModioUICollectionFollowStateChangedReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterCollectionFollowStateChangedReceiver(UObject* ObjectToDeregister);
};