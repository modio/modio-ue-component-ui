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
#include "Types/ModioUnsigned64.h"
#include "IModioUIConnectivityChangedReceiver.h"
#include "UObject/Interface.h"

#include "IModioUIWalletBalanceUpdatedEventReceiver.generated.h"

/**
 * @brief Interface for objects that want to receive events when the user's wallet balance changes
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Event Handlers", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIWalletBalanceUpdatedEventReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIWalletBalanceUpdatedEventReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedWalletBalanceUpdated = false;
	void WalletBalanceUpdatedHandler(uint64 NewBalance);

	static void WalletBalanceUpdatedHandlerK2Helper(uint64 NewBalance, TWeakObjectPtr<UObject> ImplementingObject);

	friend class UModioUIWalletBalanceUpdatedEventReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMap;

	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->RegisterEventHandler<IModioUIWalletBalanceUpdatedEventReceiver>(
				Subsystem->OnGetUserWalletBalanceRequestCompleted,
				&IModioUIWalletBalanceUpdatedEventReceiver::WalletBalanceUpdatedHandler,
				*Cast<ImplementingClass>(this));
		}
	}

	template<typename ImplementingClass>
	void Deregister()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->DeregisterEventHandler<IModioUIConnectivityChangedReceiver>(Subsystem->OnConnectivityChanged,
																				   *Cast<ImplementingClass>(this));
		}
	}

	static void RegisterFromK2(UObject* ObjectToDeregister)
	{
		if (ObjectToDeregister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				Subsystem->DeregisterEventHandlerFromK2(
					Subsystem->OnGetUserWalletBalanceRequestCompleted,
					&IModioUIWalletBalanceUpdatedEventReceiver::WalletBalanceUpdatedHandlerK2Helper, RegistrationMap,
					TWeakObjectPtr<>(ObjectToDeregister));
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
				Subsystem->RegisterEventHandlerFromK2(
					Subsystem->OnGetUserWalletBalanceRequestCompleted,
					&IModioUIWalletBalanceUpdatedEventReceiver::WalletBalanceUpdatedHandlerK2Helper, RegistrationMap,
					TWeakObjectPtr<>(ObjectToDeregister));
			}
		}
	}

	virtual void NativeOnWalletBalanceUpdated(uint64 NewBalance);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Wallet Balance Updated",
			  meta = (BlueprintProtected))
	void OnWalletBalanceUpdated(FModioUnsigned64 WalletBalance);
};

UCLASS()
class MODIOUICORE_API UModioUIWalletBalanceUpdatedEventReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for wallet balance change UI events provided it implements
	/// UModioUIWalletBalanceUpdatedEventReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterWalletBalanceUpdatedEventReceiver(UObject* ObjectToRegister);

	/// @brief Deregisters a UObject as a receiver for wallet balance change UI events provided it implements
	/// UModioUIWalletBalanceUpdatedEventReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterWalletBalanceUpdatedEventReceiver(UObject* ObjectToDeregister);
};