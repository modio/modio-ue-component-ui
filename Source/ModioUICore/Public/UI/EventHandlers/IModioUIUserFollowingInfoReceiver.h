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

#include "Misc/EnumClassFlags.h"
#include "ModioUISubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioUser.h"
#include "Types/ModioUserList.h"
#include "UObject/Interface.h"

#include "IModioUIUserFollowingInfoReceiver.generated.h"

/**
 * @brief Enumerator of the information envents
 */
UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EModioUIUserFollowingEventType : uint8
{
	/** Event to list user following **/
	ListUserFollowing = 1 << 0,

	/** Event when a user is followed **/
	FollowUser = 1 << 1,

	/** Event when a user is unfollowed **/
	UnfollowUser = 1 << 2
};

ENUM_CLASS_FLAGS(EModioUIUserFollowingEventType);

/**
 * @brief Interface for objects that want to receive user following list events (FollowUser/UnfollowUser/GetUserFollowing)
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Event Handlers", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIUserFollowingInfoReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIUserFollowingInfoReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedUIUserFollowingInfoReceiver = false;

	void ListUserFollowingRequestHandler(FModioErrorCode ErrorCode, TOptional<FModioUserList> FollowingList);
	void FollowUserRequestHandler(FModioErrorCode ErrorCode);
	void UnfollowUserRequestHandler(FModioErrorCode ErrorCode);

	static void ListUserFollowingRequestK2Helper(FModioErrorCode ErrorCode, TOptional<FModioUserList> FollowingList,
												  TWeakObjectPtr<UObject> ImplementingObject);
	static void FollowUserRequestK2Helper(FModioErrorCode ErrorCode, TWeakObjectPtr<UObject> ImplementingObject);
	static void UnfollowUserRequestK2Helper(FModioErrorCode ErrorCode, TWeakObjectPtr<UObject> ImplementingObject);

	friend class UModioUIUserFollowingInfoReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapListUserFollowing;
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapFollowUser;
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapUnfollowUser;

	template<typename ImplementingClass>
	void Register(EModioUIUserFollowingEventType EventType)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			if (EnumHasAllFlags(EventType, EModioUIUserFollowingEventType::ListUserFollowing))
			{
				Subsystem->RegisterEventHandler<IModioUIUserFollowingInfoReceiver>(
					Subsystem->OnListUserFollowingRequestComplete,
					&IModioUIUserFollowingInfoReceiver::ListUserFollowingRequestHandler,
					*Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(EventType, EModioUIUserFollowingEventType::FollowUser))
			{
				Subsystem->RegisterEventHandler<IModioUIUserFollowingInfoReceiver>(
					Subsystem->OnFollowUserRequestCompleted,
					&IModioUIUserFollowingInfoReceiver::FollowUserRequestHandler,
					*Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(EventType, EModioUIUserFollowingEventType::UnfollowUser))
			{
				Subsystem->RegisterEventHandler<IModioUIUserFollowingInfoReceiver>(
					Subsystem->OnUnfollowUserRequestCompleted,
					&IModioUIUserFollowingInfoReceiver::UnfollowUserRequestHandler,
					*Cast<ImplementingClass>(this));
			}
		}
	}

	template<typename ImplementingClass>
	void Deregister(EModioUIUserFollowingEventType EventType)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			if (EnumHasAllFlags(EventType, EModioUIUserFollowingEventType::ListUserFollowing))
			{
				Subsystem->DeregisterEventHandler<IModioUIUserFollowingInfoReceiver>(
					Subsystem->OnListUserFollowingRequestComplete, *Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(EventType, EModioUIUserFollowingEventType::FollowUser))
			{
				Subsystem->DeregisterEventHandler<IModioUIUserFollowingInfoReceiver>(
					Subsystem->OnFollowUserRequestCompleted, *Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(EventType, EModioUIUserFollowingEventType::UnfollowUser))
			{
				Subsystem->DeregisterEventHandler<IModioUIUserFollowingInfoReceiver>(
					Subsystem->OnUnfollowUserRequestCompleted, *Cast<ImplementingClass>(this));
			}
		}
	}

	static void RegisterFromK2(UObject* ObjectToRegister, EModioUIUserFollowingEventType EventType)
	{
		if (ObjectToRegister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				if (EnumHasAllFlags(EventType, EModioUIUserFollowingEventType::ListUserFollowing))
				{
					Subsystem->RegisterEventHandlerFromK2(Subsystem->OnListUserFollowingRequestComplete,
						&IModioUIUserFollowingInfoReceiver::ListUserFollowingRequestK2Helper,
						RegistrationMapListUserFollowing, TWeakObjectPtr<>(ObjectToRegister));
				}
				if (EnumHasAllFlags(EventType, EModioUIUserFollowingEventType::FollowUser))
				{
					Subsystem->RegisterEventHandlerFromK2(
						Subsystem->OnFollowUserRequestCompleted,
						&IModioUIUserFollowingInfoReceiver::FollowUserRequestK2Helper,
						RegistrationMapFollowUser, TWeakObjectPtr<>(ObjectToRegister));
				}
				if (EnumHasAllFlags(EventType, EModioUIUserFollowingEventType::UnfollowUser))
				{
					Subsystem->RegisterEventHandlerFromK2(
						Subsystem->OnUnfollowUserRequestCompleted,
						&IModioUIUserFollowingInfoReceiver::UnfollowUserRequestK2Helper,
						RegistrationMapUnfollowUser, TWeakObjectPtr<>(ObjectToRegister));
				}
			}
		}
	}

	static void DeregisterFromK2(UObject* ObjectToDeregister, EModioUIUserFollowingEventType EventType)
	{
		if (ObjectToDeregister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				if (EnumHasAllFlags(EventType, EModioUIUserFollowingEventType::ListUserFollowing))
				{
					Subsystem->DeregisterEventHandlerFromK2(Subsystem->OnListUserFollowingRequestComplete,
						&IModioUIUserFollowingInfoReceiver::ListUserFollowingRequestK2Helper,
						RegistrationMapListUserFollowing, TWeakObjectPtr<>(ObjectToDeregister));
				}
				if (EnumHasAllFlags(EventType, EModioUIUserFollowingEventType::FollowUser))
				{
					Subsystem->DeregisterEventHandlerFromK2(
						Subsystem->OnFollowUserRequestCompleted,
						&IModioUIUserFollowingInfoReceiver::FollowUserRequestK2Helper,
						RegistrationMapFollowUser, TWeakObjectPtr<>(ObjectToDeregister));
				}
				if (EnumHasAllFlags(EventType, EModioUIUserFollowingEventType::UnfollowUser))
				{
					Subsystem->DeregisterEventHandlerFromK2(
						Subsystem->OnUnfollowUserRequestCompleted,
						&IModioUIUserFollowingInfoReceiver::UnfollowUserRequestK2Helper,
						RegistrationMapUnfollowUser, TWeakObjectPtr<>(ObjectToDeregister));
				}
			}
		}
	}

	virtual void NativeOnListUserFollowingRequestCompleted(FModioErrorCode ErrorCode,
														   TOptional<FModioUserList> FollowingList);
	virtual void NativeOnFollowUserRequestCompleted(FModioErrorCode ErrorCode);
	virtual void NativeOnUnfollowUserRequestCompleted(FModioErrorCode ErrorCode);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|User Following", meta = (BlueprintProtected))
	void OnListUserFollowingRequestCompleted(FModioErrorCode ErrorCode, FModioOptionalUserList FollowingList);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|User Following", meta = (BlueprintProtected))
	void OnFollowUserRequestCompleted(FModioErrorCode ErrorCode);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|User Following", meta = (BlueprintProtected))
	void OnUnfollowUserRequestCompleted(FModioErrorCode ErrorCode);
};

UCLASS()
class MODIOUICORE_API UModioUIUserFollowingInfoReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for ListUserFollowing/FollowUser/UnfollowUser UI events provided it implements
	/// UModioUIUserFollowingInfoReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	/// @param EventType Which type of event to register for
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterUserFollowingInfoReceiver(UObject* ObjectToRegister, EModioUIUserFollowingEventType EventType);

	/// @brief Deregisters a UObject as a receiver for ListAllMods/GetModInfo UI events provided it implements
	/// UModioUIUserFollowingInfoReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	/// @param EventType Which type of event to deregister for
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterUserFollowingInfoReceiver(UObject* ObjectToDeregister,
													EModioUIUserFollowingEventType EventType);
};
