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
#include "Types/ModioUser.h"
#include "UObject/Interface.h"

#include "IModioUIUserChangedReceiver.generated.h"

/**
 * @brief Interface for classes that want to receive events when the user changes
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Event Handlers", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIUserChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIUserChangedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedUserChanged = false;
	void UserChangedHandler(TOptional<FModioUser> NewUser);

	static void UserChangedHandlerK2Helper(TOptional<FModioUser> NewUser, TWeakObjectPtr<UObject> ImplementingObject);

	friend class UModioUIUserChangedReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMap;

	template<typename ImplementingClass>
	void Register()
	{
		if (UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			Subsystem->RegisterEventHandler<IModioUIUserChangedReceiver>(
				Subsystem->OnUserChanged, &IModioUIUserChangedReceiver::UserChangedHandler,
				*Cast<ImplementingClass>(this));
		}

		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			UserChangedHandler(Subsystem->QueryUserProfile());
		}
	}

	template<typename ImplementingClass>
	void Deregister()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->DeregisterEventHandler<IModioUIUserChangedReceiver>(Subsystem->OnUserChanged,
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
				Subsystem->RegisterEventHandlerFromK2(Subsystem->OnUserChanged,
													  &IModioUIUserChangedReceiver::UserChangedHandlerK2Helper,
													  RegistrationMap, TWeakObjectPtr<>(ObjectToRegister));
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
				Subsystem->DeregisterEventHandlerFromK2(Subsystem->OnUserChanged,
														&IModioUIUserChangedReceiver::UserChangedHandlerK2Helper,
														RegistrationMap, TWeakObjectPtr<>(ObjectToDeregister));
			}
		}
	}

	virtual void NativeUserChanged(TOptional<FModioUser> NewUser);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|User Changed", meta = (BlueprintProtected))
	void OnUserChanged(FModioOptionalUser NewUser);
};

UCLASS()
class MODIOUICORE_API UModioUIUserChangedReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for user changed events provided it implements
	/// UModioUIUserChangedReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterUserChangedReceiver(UObject* ObjectToRegister);

	/// @brief Deregisters a UObject as a receiver for user changed events provided it implements
	/// UModioUIUserChangedReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterUserChangedReceiver(UObject* ObjectToDeregister);
};