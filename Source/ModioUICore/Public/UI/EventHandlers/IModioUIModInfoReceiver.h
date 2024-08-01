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

#include "IModioUIModInfoReceiver.generated.h"

/**
 * @brief Enumerator of the information envents
 */
UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EModioUIModInfoEventType : uint8
{
	/** Event to list all mods **/
	ListAllMods,

	/** Event to retrieve the mod information **/
	GetModInfo
};

ENUM_CLASS_FLAGS(EModioUIModInfoEventType);

/**
 * @brief Interface for objects that want to receive mod information events (ListAllMods/GetModInfo)
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Event Handlers", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIModInfoReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIModInfoReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedUIModInfoReceiver = false;

	void ModInfoRequestHandler(FModioModID ModID, FModioErrorCode ErrorCode, TOptional<FModioModInfo> Info);
	void ListAllModsRequestHandler(FString RequestIdentifier, FModioErrorCode ErrorCode,
								   TOptional<FModioModInfoList> List);

	// Helper methods to allow a blueprintCallable Register method that does not rely on raw pointers
	static void ModInfoRequestHandlerK2Helper(FModioModID ModID, FModioErrorCode ErrorCode,
											  TOptional<FModioModInfo> Info,
											  TWeakObjectPtr<UObject> ImplementingObject);
	static void ListAllModsRequestHandlerK2Helper(FString RequestIdentifier, FModioErrorCode ErrorCode,
												  TOptional<FModioModInfoList> List,
												  TWeakObjectPtr<UObject> ImplementingObject);
	friend class UModioUIModInfoReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapListAllMods;
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapGetModInfo;

	template<typename ImplementingClass>
	void Register(EModioUIModInfoEventType EventType)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			if (EnumHasAllFlags(EventType, EModioUIModInfoEventType::ListAllMods))
			{
				Subsystem->RegisterEventHandler<IModioUIModInfoReceiver>(
					Subsystem->OnListAllModsRequestCompleted, &IModioUIModInfoReceiver::ListAllModsRequestHandler,
					*Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(EventType, EModioUIModInfoEventType::GetModInfo))
			{
				Subsystem->RegisterEventHandler<IModioUIModInfoReceiver>(
					Subsystem->OnModInfoRequestCompleted, &IModioUIModInfoReceiver::ModInfoRequestHandler,
					*Cast<ImplementingClass>(this));
			}
		}
	}

	static void RegisterFromK2(UObject* ObjectToRegister, EModioUIModInfoEventType EventType)
	{
		if (ObjectToRegister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				if (EnumHasAllFlags(EventType, EModioUIModInfoEventType::ListAllMods))
				{
					Subsystem->RegisterEventHandlerFromK2(Subsystem->OnListAllModsRequestCompleted,
														  &IModioUIModInfoReceiver::ListAllModsRequestHandlerK2Helper,
														  RegistrationMapListAllMods,
														  TWeakObjectPtr<>(ObjectToRegister));
				}
				if (EnumHasAllFlags(EventType, EModioUIModInfoEventType::GetModInfo))
				{
					Subsystem->RegisterEventHandlerFromK2(
						Subsystem->OnModInfoRequestCompleted, &IModioUIModInfoReceiver::ModInfoRequestHandlerK2Helper,
						RegistrationMapGetModInfo, TWeakObjectPtr<>(ObjectToRegister));
				}
			}
		}
	}

	static void DeregisterFromK2(UObject* ObjectToDeregister, EModioUIModInfoEventType EventType)
	{
		if (ObjectToDeregister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				if (EnumHasAllFlags(EventType, EModioUIModInfoEventType::ListAllMods))
				{
					Subsystem->DeregisterEventHandlerFromK2(Subsystem->OnListAllModsRequestCompleted,
														  &IModioUIModInfoReceiver::ListAllModsRequestHandlerK2Helper,
														  RegistrationMapListAllMods,
														  TWeakObjectPtr<>(ObjectToDeregister));
				}
				if (EnumHasAllFlags(EventType, EModioUIModInfoEventType::GetModInfo))
				{
					Subsystem->DeregisterEventHandlerFromK2(
						Subsystem->OnModInfoRequestCompleted, &IModioUIModInfoReceiver::ModInfoRequestHandlerK2Helper,
						RegistrationMapGetModInfo, TWeakObjectPtr<>(ObjectToDeregister));
				}
			}
		}
	}

	virtual void NativeOnModInfoRequestCompleted(FModioModID ModID, FModioErrorCode ErrorCode,
												 TOptional<FModioModInfo> Info);

	virtual void NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ErrorCode,
													 TOptional<FModioModInfoList> List);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Mod Info", meta = (BlueprintProtected))
	void OnModInfoRequestCompleted(FModioModID ModID, FModioErrorCode ErrorCode, FModioOptionalModInfo Info);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Mod Info", meta = (BlueprintProtected))
	void OnListAllModsRequestCompleted(const FString& RequestIdentifier, FModioErrorCode ErrorCode,
									   FModioOptionalModInfoList List);
};

UCLASS()
class MODIOUICORE_API UModioUIModInfoReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for ListAllMods/GetModInfo UI events provided it implements
	/// UModioUIModInfoReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	/// @param EventType Which type of event to register for
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterModInfoReceiver(UObject* ObjectToRegister, EModioUIModInfoEventType EventType);

	/// @brief Deregisters a UObject as a receiver for ListAllMods/GetModInfo UI events provided it implements
	/// UModioUIModInfoReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	/// @param EventType Which type of event to deregister for
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterModInfoReceiver(UObject* ObjectToDeregister, EModioUIModInfoEventType EventType);
};
