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
#include "Types/ModioImageWrapper.h"
#include "UObject/Interface.h"

#include "IModioUIMediaDownloadCompletedReceiver.generated.h"

/**
 * @brief Enumerator with different events when an image is downloaded
 */
UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EModioUIMediaDownloadEventType : uint8
{
	/** Downloading gallery images **/
	ModGalleryImages,

	/** Downloading an avatar **/
	ModCreatorAvatarImage,

	/** Downloading a logo **/
	ModLogo,

	/* Downloading a collection logo */
	ModCollectionLogo,

	/* Downloading a collection curator avatar */
	ModCollectionCuratorAvatarImage
};

ENUM_CLASS_FLAGS(EModioUIMediaDownloadEventType);

/**
 * @brief Interface for objects that want to receive notifications when a media download operation is completed
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Events|Media", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIMediaDownloadCompletedReceiver : public UInterface
{
	GENERATED_BODY()
};

// TODO: @modio-ue make sure this interface is consistent with that used in other interfaces regarding how the native
// methods interact with the blueprintimplementable ones etc

class MODIOUICORE_API IModioUIMediaDownloadCompletedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedMediaDownloadCompleted = false;

	void GalleryImageDownloadHandler(FModioModID ModID, FModioErrorCode ErrorCode, int32 Index,
	                                 TOptional<FModioImageWrapper> Image);
	void ModLogoDownloadHandler(FModioModID ModID, FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image,
	                            EModioLogoSize LogoSize);

	void CreatorAvatarDownloadHandler(FModioModID ModID, FModioErrorCode ErrorCode,
	                                  TOptional<FModioImageWrapper> Image);

	void ModCollectionLogoDownloadHandler(FModioModCollectionID ID, FModioErrorCode ErrorCode,
	                                      TOptional<FModioImageWrapper> Image,
	                                      EModioLogoSize LogoSize);

	void ModCollectionCuratorAvatarDownloadHandler(FModioModCollectionID ID, FModioErrorCode ErrorCode,
	                                               TOptional<FModioImageWrapper> Image);

	static void GalleryImageDownloadHandlerK2Helper(FModioModID ModID, FModioErrorCode ErrorCode, int32 Index,
	                                                TOptional<FModioImageWrapper> Image,
	                                                TWeakObjectPtr<UObject> ImplementingObject);
	static void ModLogoDownloadHandlerK2Helper(FModioModID ModID, FModioErrorCode ErrorCode,
	                                           TOptional<FModioImageWrapper> Image, EModioLogoSize LogoSize,
	                                           TWeakObjectPtr<UObject> ImplementingObject);
	static void CreatorAvatarDownloadHandlerK2Helper(FModioModID ModID, FModioErrorCode ErrorCode,
	                                                 TOptional<FModioImageWrapper> Image,
	                                                 TWeakObjectPtr<UObject> ImplementingObject);
	static void ModCollectionLogoDownloadHandlerK2Helper(FModioModCollectionID ID, FModioErrorCode ErrorCode,
	                                                     TOptional<FModioImageWrapper> Image, EModioLogoSize LogoSize,
	                                                     TWeakObjectPtr<UObject> ImplementingObject);
	static void ModCollectionCuratorAvatarDownloadHandlerK2Helper(FModioModCollectionID ID, FModioErrorCode ErrorCode,
	                                                              TOptional<FModioImageWrapper> Image,
	                                                              TWeakObjectPtr<UObject> ImplementingObject);
	friend class UModioUIMediaDownloadCompletedReceiverLibrary;

protected:
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapGallery;
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapLogo;
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapAvatar;
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapCollectionLogo;
	inline static TMap<TWeakObjectPtr<>, FDelegateHandle> RegistrationMapCollectionAvatar;

	template<typename ImplementingClass>
	void Register(EModioUIMediaDownloadEventType DownloadTypes)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModGalleryImages))
			{
				Subsystem->RegisterEventHandler<IModioUIMediaDownloadCompletedReceiver>(
					Subsystem->OnModGalleryImageDownloadCompleted,
					&IModioUIMediaDownloadCompletedReceiver::GalleryImageDownloadHandler,
					*Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModLogo))
			{
				Subsystem->RegisterEventHandler<IModioUIMediaDownloadCompletedReceiver>(
					Subsystem->OnModLogoDownloadCompleted,
					&IModioUIMediaDownloadCompletedReceiver::ModLogoDownloadHandler, *Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCreatorAvatarImage))
			{
				Subsystem->RegisterEventHandler<IModioUIMediaDownloadCompletedReceiver>(
					Subsystem->OnModCreatorAvatarDownloadCompleted,
					&IModioUIMediaDownloadCompletedReceiver::CreatorAvatarDownloadHandler,
					*Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCollectionLogo))
			{
				Subsystem->RegisterEventHandler<IModioUIMediaDownloadCompletedReceiver>(
					Subsystem->OnModCollectionLogoDownloadCompleted,
					&IModioUIMediaDownloadCompletedReceiver::ModCollectionLogoDownloadHandler,
					*Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCollectionCuratorAvatarImage))
			{
				Subsystem->RegisterEventHandler<IModioUIMediaDownloadCompletedReceiver>(
					Subsystem->OnModCollectionCuratorAvatarDownloadCompleted,
					&IModioUIMediaDownloadCompletedReceiver::ModCollectionCuratorAvatarDownloadHandler,
					*Cast<ImplementingClass>(this));
			}
		}
	}

	template<typename ImplementingClass>
	void Deregister(EModioUIMediaDownloadEventType DownloadTypes)
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModGalleryImages))
			{
				Subsystem->DeregisterEventHandler<&IModioUIMediaDownloadCompletedReceiver::GalleryImageDownloadHandler>(
					Subsystem->OnModGalleryImageDownloadCompleted, *Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModLogo))
			{
				Subsystem->DeregisterEventHandler<&IModioUIMediaDownloadCompletedReceiver::GalleryImageDownloadHandler>(
					Subsystem->OnModLogoDownloadCompleted, *Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCreatorAvatarImage))
			{
				Subsystem->DeregisterEventHandler<&IModioUIMediaDownloadCompletedReceiver::GalleryImageDownloadHandler>(
					Subsystem->OnModCreatorAvatarDownloadCompleted, *Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCollectionLogo))
			{
				Subsystem->DeregisterEventHandler<&
					IModioUIMediaDownloadCompletedReceiver::ModCollectionLogoDownloadHandler>(
					Subsystem->OnModCollectionLogoDownloadCompleted, *Cast<ImplementingClass>(this));
			}
			if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCollectionCuratorAvatarImage))
			{
				Subsystem->DeregisterEventHandler<&
					IModioUIMediaDownloadCompletedReceiver::ModCollectionCuratorAvatarDownloadHandler>(
					Subsystem->OnModCollectionCuratorAvatarDownloadCompleted, *Cast<ImplementingClass>(this));
			}
		}
	}

	static void RegisterFromK2(UObject* ObjectToRegister, EModioUIMediaDownloadEventType DownloadTypes)
	{
		if (ObjectToRegister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModGalleryImages))
				{
					Subsystem->RegisterEventHandlerFromK2(
						Subsystem->OnModGalleryImageDownloadCompleted,
						&IModioUIMediaDownloadCompletedReceiver::GalleryImageDownloadHandlerK2Helper,
						RegistrationMapGallery, TWeakObjectPtr<>(ObjectToRegister));
				}
				if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModLogo))
				{
					Subsystem->RegisterEventHandlerFromK2(
						Subsystem->OnModLogoDownloadCompleted,
						&IModioUIMediaDownloadCompletedReceiver::ModLogoDownloadHandlerK2Helper, RegistrationMapLogo,
						TWeakObjectPtr<>(ObjectToRegister));
				}
				if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCreatorAvatarImage))
				{
					Subsystem->RegisterEventHandlerFromK2(
						Subsystem->OnModCreatorAvatarDownloadCompleted,
						&IModioUIMediaDownloadCompletedReceiver::CreatorAvatarDownloadHandlerK2Helper,
						RegistrationMapAvatar, TWeakObjectPtr<>(ObjectToRegister));
				}
				if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCollectionLogo))
				{
					Subsystem->RegisterEventHandlerFromK2(
						Subsystem->OnModCollectionLogoDownloadCompleted,
						&IModioUIMediaDownloadCompletedReceiver::ModCollectionLogoDownloadHandlerK2Helper,
						RegistrationMapCollectionLogo,
						TWeakObjectPtr<>(ObjectToRegister));
				}
				if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCollectionCuratorAvatarImage))
				{
					Subsystem->RegisterEventHandlerFromK2(
						Subsystem->OnModCollectionCuratorAvatarDownloadCompleted,
						&IModioUIMediaDownloadCompletedReceiver::ModCollectionCuratorAvatarDownloadHandlerK2Helper,
						RegistrationMapCollectionAvatar, TWeakObjectPtr<>(ObjectToRegister));
				}
			}
		}
	}

	static void DeregisterFromK2(UObject* ObjectToDeregister, EModioUIMediaDownloadEventType DownloadTypes)
	{
		if (ObjectToDeregister)
		{
			UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
			if (Subsystem)
			{
				if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModGalleryImages))
				{
					Subsystem->DeregisterEventHandlerFromK2(
						Subsystem->OnModGalleryImageDownloadCompleted,
						&IModioUIMediaDownloadCompletedReceiver::GalleryImageDownloadHandlerK2Helper,
						RegistrationMapGallery, TWeakObjectPtr<>(ObjectToDeregister));
				}
				if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModLogo))
				{
					Subsystem->DeregisterEventHandlerFromK2(
						Subsystem->OnModLogoDownloadCompleted,
						&IModioUIMediaDownloadCompletedReceiver::ModLogoDownloadHandlerK2Helper, RegistrationMapLogo,
						TWeakObjectPtr<>(ObjectToDeregister));
				}
				if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCreatorAvatarImage))
				{
					Subsystem->DeregisterEventHandlerFromK2(
						Subsystem->OnModCreatorAvatarDownloadCompleted,
						&IModioUIMediaDownloadCompletedReceiver::CreatorAvatarDownloadHandlerK2Helper,
						RegistrationMapAvatar, TWeakObjectPtr<>(ObjectToDeregister));
				}
				if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCollectionLogo))
				{
					Subsystem->DeregisterEventHandlerFromK2(
						Subsystem->OnModCollectionLogoDownloadCompleted,
						&IModioUIMediaDownloadCompletedReceiver::ModCollectionLogoDownloadHandlerK2Helper,
						RegistrationMapCollectionLogo,
						TWeakObjectPtr<>(ObjectToDeregister));
				}
				if (EnumHasAllFlags(DownloadTypes, EModioUIMediaDownloadEventType::ModCollectionCuratorAvatarImage))
				{
					Subsystem->DeregisterEventHandlerFromK2(
						Subsystem->OnModCollectionCuratorAvatarDownloadCompleted,
						&IModioUIMediaDownloadCompletedReceiver::ModCollectionCuratorAvatarDownloadHandlerK2Helper,
						RegistrationMapCollectionAvatar, TWeakObjectPtr<>(ObjectToDeregister));
				}
			}
		}
	}

	virtual void NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode,
	                                              TOptional<FModioImageWrapper> Image, EModioLogoSize LogoSize);
	virtual void NativeOnModGalleryImageDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode,
	                                                      int32 ImageIndex, TOptional<FModioImageWrapper> Image);
	virtual void NativeOnModCreatorAvatarDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode,
	                                                       TOptional<FModioImageWrapper> Image);
	virtual void NativeOnModCollectionLogoDownloadCompleted(FModioModCollectionID ModCollectionID,
	                                                        FModioErrorCode ErrorCode,
	                                                        TOptional<FModioImageWrapper> Image,
	                                                        EModioLogoSize LogoSize);
	virtual void NativeOnModCollectionCuratorAvatarDownloadCompleted(FModioModCollectionID ModCollectionID,
	                                                                 FModioErrorCode ErrorCode,
	                                                                 TOptional<FModioImageWrapper> Image);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Media", meta = (BlueprintProtected))
	void OnModGalleryImageDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode, int32 ImageIndex,
	                                        FModioOptionalImage Image);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Media", meta = (BlueprintProtected))
	void OnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode, FModioOptionalImage Image,
	                                EModioLogoSize LogoSize);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Media", meta = (BlueprintProtected))
	void OnModCreatorAvatarDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode, FModioOptionalImage Image);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Media", meta = (BlueprintProtected))
	void OnModCollectionLogoDownloadCompleted(FModioModCollectionID ModCollectionID, FModioErrorCode ErrorCode,
	                                          FModioOptionalImage Image, EModioLogoSize LogoSize);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Media", meta = (BlueprintProtected))
	void OnModCollectionCuratorAvatarDownloadCompleted(FModioModCollectionID ModCollectionID, FModioErrorCode ErrorCode,
	                                                   FModioOptionalImage Image);
};

UCLASS()
class MODIOUICORE_API UModioUIMediaDownloadCompletedReceiverLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/// @brief Registers a UObject as a receiver for GetModMedia UI events provided it implements
	/// UModioUIModInfoReceiver
	/// @param ObjectToRegister UObject to register that implements the required interface
	/// @param DownloadTypes Which type of event to register for
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void RegisterMediaDownloadCompletedReceiver(UObject* ObjectToRegister,
	                                                   EModioUIMediaDownloadEventType DownloadTypes);

	/// @brief Deregisters a UObject as a receiver for GetModMedia UI events provided it implements
	/// UModioUIModInfoReceiver
	/// @param ObjectToDeregister UObject to register that implements the required interface
	/// @param DownloadTypes Which type of event to register for
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Event Handlers")
	static void DeregisterMediaDownloadCompletedReceiver(UObject* ObjectToDeregister,
	                                                     EModioUIMediaDownloadEventType DownloadTypes);
};