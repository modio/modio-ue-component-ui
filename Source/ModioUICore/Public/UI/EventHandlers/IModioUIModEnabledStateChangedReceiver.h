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

#include "Misc/EnumClassFlags.h"
#include "ModioUISubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioModInfoList.h"
#include "UObject/Interface.h"

#include "IModioUIModEnabledStateChangedReceiver.generated.h"

/**
 * @brief Interface for objects that want to receive events when a mod is enabled or disabled
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Event Handlers", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIModEnabledStateChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIModEnabledStateChangedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedUIModEnabledStateChanged = false;
	void ModEnabledStateChangedHandler(FModioModID ModID, bool bNewEnabledState);

	static void ModEnabledStateChangedHandlerK2Helper(FModioModID ModID, bool bNewEnabledState,
													  TWeakObjectPtr<UObject> ImplementingObject);

	friend class UModioUIModEnabledStateChangedReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMap;

	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->RegisterEventHandler<IModioUIModEnabledStateChangedReceiver>(
				Subsystem->OnModEnabledStateChanged,
				&IModioUIModEnabledStateChangedReceiver::ModEnabledStateChangedHandler, *Cast<ImplementingClass>(this));
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
					Subsystem->OnModEnabledStateChanged,
					&IModioUIModEnabledStateChangedReceiver::ModEnabledStateChangedHandlerK2Helper, RegistrationMap,
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
					Subsystem->OnModEnabledStateChanged,
					&IModioUIModEnabledStateChangedReceiver::ModEnabledStateChangedHandlerK2Helper, RegistrationMap,
					TWeakObjectPtr<>(ObjectToDeregister));
			}
		}
	}

	virtual void NativeModEnabledStateChanged(FModioModID ModID, bool bNewEnabledState);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Mod Enabled State", meta = (BlueprintProtected))
	void OnModEnabledStateChanged(FModioModID ModID, bool bNewEnabledState);
};

UCLASS()
class MODIOUICORE_API UModioUIModEnabledStateChangedReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for events when a mod is enabled/disabled
	/// @param ObjectToRegister UObject to register that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterModEnabledStateChangedReceiver(UObject* ObjectToRegister);

	/// @brief Deregisters a UObject as a receiver for events when a mod is enabled/disabled
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterModEnabledStateChangedReceiver(UObject* ObjectToDeregister);
};
