/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
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

#include "IModioUIInputModeChangedReceiver.generated.h"

/**
 * @brief Interface for classes that wish to be informed when there's an InputMode state change
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Events|InputMode", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIInputModeChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIInputModeChangedReceiver : public IInterface
{
	GENERATED_BODY()

	friend class UModioUIInputModeChangedReceiverLibrary;

	bool bRoutedInputModeChanged = false;

	void InputModeChangedHandler(EModioUIInputMode NewInputModeState);

	static void InputModeChangedHandlerK2Helper(EModioUIInputMode NewInputModeState,
												TWeakObjectPtr<UObject> ImplementingObject);

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMap;

	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->RegisterEventHandler<IModioUIInputModeChangedReceiver>(
				Subsystem->OnInputModeChanged, &IModioUIInputModeChangedReceiver::InputModeChangedHandler,
				*Cast<ImplementingClass>(this));

			// Update with the "current" InputMode state
			InputModeChangedHandler(Subsystem->CurrentInputModeState);
		}
	}

	template<typename ImplementingClass>
	void Deregister()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->DeregisterEventHandler<IModioUIInputModeChangedReceiver>(Subsystem->OnInputModeChanged,
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
					Subsystem->OnInputModeChanged, &IModioUIInputModeChangedReceiver::InputModeChangedHandlerK2Helper,
					RegistrationMap, TWeakObjectPtr<>(ObjectToRegister));

				// Update with the "current" InputMode state
				InputModeChangedHandlerK2Helper(Subsystem->CurrentInputModeState,
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
					Subsystem->OnInputModeChanged, &IModioUIInputModeChangedReceiver::InputModeChangedHandlerK2Helper,
					RegistrationMap, TWeakObjectPtr<>(ObjectToDeregister));
			}
		}
	}

	virtual void NativeOnInputModeChanged(EModioUIInputMode NewInputModeState);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|InputMode", meta = (BlueprintProtected))
	void OnInputModeChanged(EModioUIInputMode NewInputModeState);
};

UCLASS()
class MODIOUICORE_API UModioUIInputModeChangedReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for InputMode changed events provided it implements
	/// UModioUIInputModeChangedReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterInputModeChangedReceiver(UObject* ObjectToRegister);

	/// @brief Deregisters a UObject as a receiver for InputMode changed events provided it implements
	/// UModioUIInputModeChangedReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterInputModeChangedReceiver(UObject* ObjectToDeregister);
};