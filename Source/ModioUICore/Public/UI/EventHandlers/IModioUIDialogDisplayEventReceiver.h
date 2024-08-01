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

#include "IModioUIDialogDisplayEventReceiver.generated.h"

/**
 * @brief Interface for classes that wish to know when mod.io UI elements request a dialog be displayed
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Events|Dialog Display", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIDialogDisplayEventReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIDialogDisplayEventReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedDialogDisplayEvent = false;
	void DialogDisplayEventHandler(EModioUIDialogType DialogRequested, UObject* DataSource);

	static void DialogDisplayEventHandlerK2Helper(EModioUIDialogType DialogRequested, UObject* DataSource,
												  TWeakObjectPtr<UObject> ImplementingObject);

	friend class UModioUIDialogDisplayEventReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMap;

	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->RegisterEventHandler<IModioUIDialogDisplayEventReceiver>(
				Subsystem->OnDialogDisplayEvent, &IModioUIDialogDisplayEventReceiver::DialogDisplayEventHandler,
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
					Subsystem->OnDialogDisplayEvent,
					&IModioUIDialogDisplayEventReceiver::DialogDisplayEventHandlerK2Helper, RegistrationMap,
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
					Subsystem->OnDialogDisplayEvent,
					&IModioUIDialogDisplayEventReceiver::DialogDisplayEventHandlerK2Helper, RegistrationMap,
					TWeakObjectPtr<>(ObjectToDeregister));
			}
		}
	}

	virtual void NativeOnDialogDisplayEvent(EModioUIDialogType DialogRequested, UObject* DataSource);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Dialog Display", meta = (BlueprintProtected))
	void OnDialogDisplayEvent(EModioUIDialogType DialogRequested, UObject* DataSource);
};

UCLASS()
class MODIOUICORE_API UModioUIDialogDisplayEventReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for requests to display mod.io UI dialogs provided it implements
	/// UModioUIDialogDisplayEventReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterDialogDisplayEventReceiver(UObject* ObjectToRegister);

	/// @brief Deregisters a UObject as a receiver for requests to display mod.io UI dialogs provided it implements
	/// UModioUIDialogDisplayEventReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterDialogDisplayEventReceiver(UObject* ObjectToDeregister);
};
