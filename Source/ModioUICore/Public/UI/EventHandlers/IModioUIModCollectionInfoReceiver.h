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

#include "Misc/EnumClassFlags.h"
#include "ModioUISubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioModCollection.h"
#include "UObject/Interface.h"

#include "IModioUIModCollectionInfoReceiver.generated.h"

/**
 * @brief Enumerator of the information events
 */
UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EModioUIModCollectionInfoEventType : uint8
{
	/** Event to list all mod collections **/
	ListAllModCollections,

	/** Event to retrieve the mod collection information **/
	GetModCollectionInfo,

	/** Event to recieve information on the mods in a collection **/
	GetModCollectionMods
};

ENUM_CLASS_FLAGS(EModioUIModCollectionInfoEventType);

/**
 * @brief Interface for objects that want to receive mod collection information events (ListAllModCollections/GetModCollectionInfo)
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Event Handlers", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIModCollectionInfoReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIModCollectionInfoReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedUIModCollectionInfoReceiver = false;

	void ModCollectionInfoRequestHandler(FModioModCollectionID ID, FModioErrorCode ErrorCode,
	                                     TOptional<FModioModCollectionInfo> Info);
	void ListModCollectionsRequestHandler(FString RequestIdentifier, FModioErrorCode ErrorCode,
	                                      TOptional<FModioModCollectionInfoList> List);
	void GetModCollectionModsRequestHandler(FModioModCollectionID CollectionID, FModioErrorCode ErrorCode,
	                                        TOptional<FModioModInfoList> Mods);

	// Helper methods to allow a blueprintCallable Register method that does not rely on raw pointers
	static void ModCollectionInfoRequestHandlerK2Helper(FModioModCollectionID ID, FModioErrorCode ErrorCode,
	                                                    TOptional<FModioModCollectionInfo> Info,
	                                                    TWeakObjectPtr<UObject> ImplementingObject);
	static void ListModCollectionsRequestHandlerK2Helper(FString RequestIdentifier, FModioErrorCode ErrorCode,
	                                                     TOptional<FModioModCollectionInfoList> List,
	                                                     TWeakObjectPtr<UObject> ImplementingObject);
	static void GetModCollectionModsRequestHandlerK2Helper(FModioModCollectionID CollectionID,
	                                                       FModioErrorCode ErrorCode,
	                                                       TOptional<FModioModInfoList> Mods,
	                                                       TWeakObjectPtr<UObject> ImplementingObject);

	friend class UModioUIModCollectionInfoReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapListModCollections;
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapGetModCollectionInfo;
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapGetModCollectionMods;

	template<typename ImplementingClass>
	void Register(EModioUIModCollectionInfoEventType EventType)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			if (EnumHasAllFlags(EventType, EModioUIModCollectionInfoEventType::ListAllModCollections))
			{
				Subsystem->RegisterEventHandler<IModioUIModCollectionInfoReceiver>(
					Subsystem->OnListModCollectionsRequestCompleted,
					&IModioUIModCollectionInfoReceiver::ListModCollectionsRequestHandler,
					*Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(EventType, EModioUIModCollectionInfoEventType::GetModCollectionInfo))
			{
				Subsystem->RegisterEventHandler<IModioUIModCollectionInfoReceiver>(
					Subsystem->OnModCollectionInfoRequestCompleted,
					&IModioUIModCollectionInfoReceiver::ModCollectionInfoRequestHandler,
					*Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(EventType, EModioUIModCollectionInfoEventType::GetModCollectionMods))
			{
				Subsystem->RegisterEventHandler<IModioUIModCollectionInfoReceiver>(
					Subsystem->OnGetModCollectionModsRequestCompleted,
					&IModioUIModCollectionInfoReceiver::GetModCollectionModsRequestHandler,
					*Cast<ImplementingClass>(this));
			}
		}
	}

	template<typename ImplementingClass>
	void Deregister(EModioUIModCollectionInfoEventType EventType)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			if (EnumHasAllFlags(EventType, EModioUIModCollectionInfoEventType::ListAllModCollections))
			{
				Subsystem->DeregisterEventHandler<IModioUIModCollectionInfoReceiver>(
					Subsystem->OnListModCollectionsRequestCompleted,
					*Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(EventType, EModioUIModCollectionInfoEventType::GetModCollectionInfo))
			{
				Subsystem->DeregisterEventHandler<IModioUIModCollectionInfoReceiver>(
					Subsystem->OnModCollectionInfoRequestCompleted,
					*Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(EventType, EModioUIModCollectionInfoEventType::GetModCollectionMods))
			{
				Subsystem->DeregisterEventHandler<IModioUIModCollectionInfoReceiver>(
					Subsystem->OnGetModCollectionModsRequestCompleted,
					*Cast<ImplementingClass>(this));
			}
		}
	}

	static void RegisterFromK2(UObject* ObjectToRegister, EModioUIModCollectionInfoEventType EventType)
	{
		if (ObjectToRegister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				if (EnumHasAllFlags(EventType, EModioUIModCollectionInfoEventType::ListAllModCollections))
				{
					Subsystem->RegisterEventHandlerFromK2(Subsystem->OnListModCollectionsRequestCompleted,
					                                      &IModioUIModCollectionInfoReceiver::ListModCollectionsRequestHandlerK2Helper,
					                                      RegistrationMapListModCollections,
					                                      TWeakObjectPtr<>(ObjectToRegister));
				}
				if (EnumHasAllFlags(EventType, EModioUIModCollectionInfoEventType::GetModCollectionInfo))
				{
					Subsystem->RegisterEventHandlerFromK2(
						Subsystem->OnModCollectionInfoRequestCompleted,
						&IModioUIModCollectionInfoReceiver::ModCollectionInfoRequestHandlerK2Helper,
						RegistrationMapGetModCollectionInfo, TWeakObjectPtr<>(ObjectToRegister));
				}
				if (EnumHasAllFlags(EventType, EModioUIModCollectionInfoEventType::GetModCollectionMods))
				{
					Subsystem->RegisterEventHandlerFromK2(
						Subsystem->OnGetModCollectionModsRequestCompleted,
						&IModioUIModCollectionInfoReceiver::GetModCollectionModsRequestHandlerK2Helper,
						RegistrationMapGetModCollectionMods, TWeakObjectPtr<>(ObjectToRegister));
				}
			}
		}
	}

	static void DeregisterFromK2(UObject* ObjectToDeregister, EModioUIModCollectionInfoEventType EventType)
	{
		if (ObjectToDeregister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				if (EnumHasAllFlags(EventType, EModioUIModCollectionInfoEventType::ListAllModCollections))
				{
					Subsystem->DeregisterEventHandlerFromK2(Subsystem->OnListModCollectionsRequestCompleted,
					                                        &IModioUIModCollectionInfoReceiver::ListModCollectionsRequestHandlerK2Helper,
					                                        RegistrationMapListModCollections,
					                                        TWeakObjectPtr<>(ObjectToDeregister));
				}
				if (EnumHasAllFlags(EventType, EModioUIModCollectionInfoEventType::GetModCollectionInfo))
				{
					Subsystem->DeregisterEventHandlerFromK2(
						Subsystem->OnModCollectionInfoRequestCompleted,
						&IModioUIModCollectionInfoReceiver::ModCollectionInfoRequestHandlerK2Helper,
						RegistrationMapGetModCollectionInfo, TWeakObjectPtr<>(ObjectToDeregister));
				}
				if (EnumHasAllFlags(EventType, EModioUIModCollectionInfoEventType::GetModCollectionMods))
				{
					Subsystem->DeregisterEventHandlerFromK2(
						Subsystem->OnGetModCollectionModsRequestCompleted,
						&IModioUIModCollectionInfoReceiver::GetModCollectionModsRequestHandlerK2Helper,
						RegistrationMapGetModCollectionMods, TWeakObjectPtr<>(ObjectToDeregister));
				}
			}
		}
	}

	virtual void NativeOnModCollectionInfoRequestCompleted(FModioModCollectionID ID, FModioErrorCode ErrorCode,
	                                                       TOptional<FModioModCollectionInfo> Info);

	virtual void NativeOnListModCollectionsRequestCompleted(FString RequestIdentifier, FModioErrorCode ErrorCode,
	                                                        TOptional<FModioModCollectionInfoList> List);

	virtual void NativeOnModCollectionModsRequestCompleted(FModioModCollectionID CollectionID,
	                                                       FModioErrorCode ErrorCode,
	                                                       TOptional<FModioModInfoList> Mods);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Mod Collection Info",
		meta = (BlueprintProtected))
	void OnModCollectionInfoRequestCompleted(FModioModCollectionID ID, FModioErrorCode ErrorCode,
	                                         FModioOptionalModCollectionInfo Info);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Mod Collection Info",
		meta = (BlueprintProtected))
	void OnListModCollectionsRequestCompleted(const FString& RequestIdentifier, FModioErrorCode ErrorCode,
	                                          FModioOptionalModCollectionInfoList List);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Mod Collection Info",
		meta = (BlueprintProtected))
	void OnGetModCollectionModsRequestCompleted(const FModioModCollectionID& CollectionID, FModioErrorCode ErrorCode,
	                                            FModioOptionalModInfoList Mods);
};

UCLASS()
class MODIOUICORE_API UModioUIModCollectionInfoReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for ListAllModCollections/GetModCollectionInfo UI events provided it implements
	/// UModioUIModCollectionInfoReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	/// @param EventType Which type of event to register for
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterModCollectionInfoReceiver(UObject* ObjectToRegister,
	                                              EModioUIModCollectionInfoEventType EventType);

	/// @brief Deregisters a UObject as a receiver for ListAllModCollections/GetModCollectionInfo UI events provided it implements
	/// UModioUIModCollectionInfoReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	/// @param EventType Which type of event to deregister for
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterModCollectionInfoReceiver(UObject* ObjectToDeregister,
	                                                EModioUIModCollectionInfoEventType EventType);
};