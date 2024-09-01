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
#include "Types/ModioErrorCode.h"
#include "Types/ModioImageWrapper.h"
#include "Types/ModioUser.h"
#include "UObject/Interface.h"

#include "IModioUIUserAvatarDownloadCompletedReceiver.generated.h"

/**
 * @brief Interface for classes that want to receive events when a user's avatar has been downloaded
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Event Handlers", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIUserAvatarDownloadCompletedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIUserAvatarDownloadCompletedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedAvatarDownloaded = false;
	void UserAvatarDownloadCompletedHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image);
	static void UserAvatarDownloadCompletedHandlerK2Helper(FModioErrorCode ErrorCode,
														   TOptional<FModioImageWrapper> Image,
														   TWeakObjectPtr<UObject> ImplementingObject);
	friend class UModioUIUserAvatarDownloadCompletedReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMap;

	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->OnUserAvatarDownloadCompleted.AddUObject(
				Cast<ImplementingClass>(this),
				&IModioUIUserAvatarDownloadCompletedReceiver::UserAvatarDownloadCompletedHandler);
		}
	}

	template<typename ImplementingClass>
	void Deregister()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->DeregisterEventHandler<IModioUIUserAvatarDownloadCompletedReceiver>(
				Subsystem->OnUserAvatarDownloadCompleted, *Cast<ImplementingClass>(this));
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
					Subsystem->OnUserAvatarDownloadCompleted,
					&IModioUIUserAvatarDownloadCompletedReceiver::UserAvatarDownloadCompletedHandlerK2Helper,
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
				Subsystem->DeregisterEventHandlerFromK2(
					Subsystem->OnUserAvatarDownloadCompleted,
					&IModioUIUserAvatarDownloadCompletedReceiver::UserAvatarDownloadCompletedHandlerK2Helper,
					RegistrationMap, TWeakObjectPtr<>(ObjectToDeregister));
			}
		}
	}

	virtual void NativeOnUserAvatarDownloadCompleted(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Avatar Download", meta = (BlueprintProtected))
	void OnUserAvatarDownloadCompleted(FModioErrorCode ErrorCode, FModioOptionalImage Image);
};

UCLASS()
class MODIOUICORE_API UModioUIUserAvatarDownloadCompletedReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for user avatar download events provided it implements
	/// UModioUIUserAvatarDownloadCompletedReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterUserAvatarDownloadCompletedReceiver(UObject* ObjectToRegister);

	/// @brief Deregisters a UObject as a receiver for user avatar download events provided it implements
	/// UModioUIUserAvatarDownloadCompletedReceiver
	/// @param ObjectToDeregister UObject to deregister that implements the required interface
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterUserAvatarDownloadCompletedReceiver(UObject* ObjectToDeregister);
};