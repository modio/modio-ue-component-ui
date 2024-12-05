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
#include "Types/ModioTokenPack.h"
#include "Types/ModioTokenPackList.h"
#include "UObject/Interface.h"
#include "IModioUITokenPackReceiver.generated.h"

/**
 * @brief Enumerator of the information envents
 */
UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EModioUITokenPackEventType : uint8
{
	/** Event to list all token packs **/
	ListAllTokenPacks,

	/** Event to retrieve the token pack information **/
	GetTokenPack
};

ENUM_CLASS_FLAGS(EModioUITokenPackEventType);


UINTERFACE(BlueprintType, Category = "mod.io|UI|Event Handlers", Meta = (DocGroup = "Event Handler Interfaces"))
class UModioUITokenPackReceiver : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MODIOUICORE_API IModioUITokenPackReceiver
{
	GENERATED_BODY()

	bool bRoutedUITokenPackReceiver = false;

	void TokenPackRequestHandler(FModioTokenPackID TokenPackID, FModioErrorCode ErrorCode,
								 TOptional<FModioTokenPack> Pack);
	void ListAllTokenPacksRequestHandler(FModioErrorCode ErrorCode, TOptional<FModioTokenPackList> List);

	// Helper methods to allow a blueprintCallable Register method that does not rely on raw pointers
	static void TokenPackRequestHandlerK2Helper(FModioTokenPackID TokenPackID, FModioErrorCode ErrorCode,
												TOptional<FModioTokenPack> Pack,
											  TWeakObjectPtr<UObject> ImplementingObject);
	static void ListAllTokenPacksRequestHandlerK2Helper(FModioErrorCode ErrorCode,
												  TOptional<FModioTokenPackList> List,
												  TWeakObjectPtr<UObject> ImplementingObject);
	friend class UModioUITokenPackReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapListAllTokenPacks;
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapGetTokenPack;

	template<typename ImplementingClass>
	void Register(EModioUITokenPackEventType EventType)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			if (EnumHasAllFlags(EventType, EModioUITokenPackEventType::ListAllTokenPacks))
			{
				Subsystem->RegisterEventHandler<IModioUITokenPackReceiver>(
					Subsystem->OnListAllTokenPacksRequestCompleted,
					&IModioUITokenPackReceiver::ListAllTokenPacksRequestHandler,
					*Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(EventType, EModioUITokenPackEventType::GetTokenPack))
			{
				Subsystem->RegisterEventHandler<IModioUITokenPackReceiver>(
					Subsystem->OnTokenPackRequestCompleted, &IModioUITokenPackReceiver::TokenPackRequestHandler,
					*Cast<ImplementingClass>(this));
			}
		}
	}

	template<typename ImplementingClass>
	void Deregister(EModioUITokenPackEventType EventType)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			if (EnumHasAllFlags(EventType, EModioUITokenPackEventType::ListAllTokenPacks))
			{
				Subsystem->DeregisterEventHandler<IModioUITokenPackReceiver>(Subsystem->OnListAllTokenPacksRequestCompleted,
																		   *Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(EventType, EModioUITokenPackEventType::GetTokenPack))
			{
				Subsystem->DeregisterEventHandler<IModioUITokenPackReceiver>(Subsystem->OnTokenPackRequestCompleted,
																		   *Cast<ImplementingClass>(this));
			}
		}
	}

	static void RegisterFromK2(UObject* ObjectToRegister, EModioUITokenPackEventType EventType)
	{
		if (ObjectToRegister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				if (EnumHasAllFlags(EventType, EModioUITokenPackEventType::ListAllTokenPacks))
				{
					Subsystem->RegisterEventHandlerFromK2(Subsystem->OnListAllTokenPacksRequestCompleted,
														  &IModioUITokenPackReceiver::ListAllTokenPacksRequestHandlerK2Helper,
														  RegistrationMapListAllTokenPacks,
														  TWeakObjectPtr<>(ObjectToRegister));
				}
				if (EnumHasAllFlags(EventType, EModioUITokenPackEventType::GetTokenPack))
				{
					Subsystem->RegisterEventHandlerFromK2(
						Subsystem->OnTokenPackRequestCompleted, &IModioUITokenPackReceiver::TokenPackRequestHandlerK2Helper,
						IModioUITokenPackReceiver::RegistrationMapGetTokenPack, TWeakObjectPtr<>(ObjectToRegister));
				}
			}
		}
	}

	static void DeregisterFromK2(UObject* ObjectToDeregister, EModioUITokenPackEventType EventType)
	{
		if (ObjectToDeregister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				if (EnumHasAllFlags(EventType, EModioUITokenPackEventType::ListAllTokenPacks))
				{
					Subsystem->DeregisterEventHandlerFromK2(Subsystem->OnListAllTokenPacksRequestCompleted,
															&IModioUITokenPackReceiver::ListAllTokenPacksRequestHandlerK2Helper,
															RegistrationMapListAllTokenPacks,
															TWeakObjectPtr<>(ObjectToDeregister));
				}
				if (EnumHasAllFlags(EventType, EModioUITokenPackEventType::GetTokenPack))
				{
					Subsystem->DeregisterEventHandlerFromK2(
						Subsystem->OnTokenPackRequestCompleted, &IModioUITokenPackReceiver::TokenPackRequestHandlerK2Helper,
						RegistrationMapGetTokenPack, TWeakObjectPtr<>(ObjectToDeregister));
				}
			}
		}
	}

	virtual void NativeOnTokenPackRequestCompleted(FModioTokenPackID TokenPackID, FModioErrorCode ErrorCode,
												 TOptional<FModioTokenPack> Pack);

	virtual void NativeOnListAllTokenPacksRequestCompleted(FModioErrorCode ErrorCode,
													 TOptional<FModioTokenPackList> List);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Token Pack", meta = (BlueprintProtected))
	void OnTokenPackRequestCompleted(FModioTokenPackID TokenPackID, FModioErrorCode ErrorCode, FModioOptionalTokenPack Info);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Token Pack", meta = (BlueprintProtected))
	void OnListAllTokenPacksRequestCompleted(FModioErrorCode ErrorCode, FModioOptionalTokenPackList List);
};

UCLASS()
class MODIOUICORE_API UModioUITokenPackReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for ListAllTokenPacks/GetTokenPack UI events provided it implements
	/// UModioUITokenPackReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	/// @param EventType Which type of event to register for
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterTokenPackReceiver(UObject* ObjectToRegister, EModioUITokenPackEventType EventType);

	/// @brief Deregisters a UObject as a receiver for ListAllTokenPacks/GetTokenPack UI events provided it implements
	/// UModioUIModInfoReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	/// @param EventType Which type of event to deregister for
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterTokenPackReceiver(UObject* ObjectToDeregister, EModioUITokenPackEventType EventType);
};