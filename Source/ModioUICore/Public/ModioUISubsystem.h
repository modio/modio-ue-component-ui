/*
 *  Copyright (C) 2024-2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Engine/Engine.h"
#include "Misc/EngineVersionComparison.h"
#include "ModioSubsystem.h"
#include "ModioUICore.h"
#include "Subsystems/EngineSubsystem.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioErrorCode.h"
#include "Types/ModioModInfo.h"
#include "Types/ModioModTagOptions.h"
#include "Types/ModioOpenStoreResult.h"
#include "Types/ModioTokenPack.h"
#include "Types/ModioTokenPackList.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Interfaces/IModioUIDialog.h"
#include "UI/Interfaces/IModioUIModEnabledStateProvider.h"
#include "UI/Interfaces/IModRatingStateProvider.h"
#include "UI/Interfaces/IModCollectionRatingStateProvider.h"

#include "ModioUISubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSubscriptionCompleted, FModioErrorCode, FModioModID);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnModCollectionFollowCompleted, FModioErrorCode, FModioModCollectionID);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnModSubscriptionStatusChanged, FModioModID, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnModCollectionFollowStateChanged, FModioModCollectionID, bool);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnQueryFollowedModCollectionCompleted, FModioErrorCode, ErrorCode, bool,
                                   bIsCollectionFollowed);

DECLARE_DELEGATE_TwoParams(FOnQueryFollowedModCollectionCompletedFast, FModioErrorCode, bool);

DECLARE_MULTICAST_DELEGATE_FourParams(FOnModLogoDownloadCompleted, FModioModID, FModioErrorCode,
                                      TOptional<FModioImageWrapper>, EModioLogoSize);

DECLARE_MULTICAST_DELEGATE_FourParams(FOnModGalleryImageDownloadCompleted, FModioModID, FModioErrorCode, int32,
                                      TOptional<FModioImageWrapper>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUserAvatarDownloadCompleted, FModioErrorCode, TOptional<FModioImageWrapper>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnModCreatorAvatarDownloadCompleted, FModioModID, FModioErrorCode,
                                       TOptional<FModioImageWrapper>);

DECLARE_MULTICAST_DELEGATE_FourParams(FOnModCollectionLogoDownloadCompleted, FModioModCollectionID, FModioErrorCode,
                                      TOptional<FModioImageWrapper>, EModioLogoSize);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnModCollectionCuratorAvatarDownloadCompleted, FModioModCollectionID,
                                       FModioErrorCode,
                                       TOptional<FModioImageWrapper>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnModInfoRequestCompleted, FModioModID, FModioErrorCode,
                                       TOptional<FModioModInfo>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnModCollectionInfoRequestCompleted, FModioModCollectionID, FModioErrorCode,
                                       TOptional<FModioModCollectionInfo>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnListAllModsRequestCompleted, FString, FModioErrorCode,
                                       TOptional<FModioModInfoList>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnListModCollectionsRequestCompleted, FString, FModioErrorCode,
                                       TOptional<FModioModCollectionInfoList>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnGetModCollectionModsRequestCompleted, FModioModCollectionID,
                                       FModioErrorCode,
                                       TOptional<FModioModInfoList>);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAuthenticatedUserChanged, TOptional<FModioUser>);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnModManagementEventUI, FModioModManagementEvent);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGetUserWalletBalanceRequestCompleted, uint64);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPurchaseRequestCompleted, FModioErrorCode, TOptional<FModioTransactionRecord>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnDisplayErrorManualParams, const FText&, const FText&, bool);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDisplayModDetails, TScriptInterface<IModioModInfoUIDetails>&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDisplayModDetailsForID, const FModioModID&);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnDisplayDialogRequest, EModioUIDialogType, UObject*);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnModEnabledChanged, FModioModID, bool);

DECLARE_DYNAMIC_DELEGATE(FOnModBrowserClosed);

DECLARE_MULTICAST_DELEGATE(FOnAuthenticationChangeStarted);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnConnectivityChanged, bool);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnShowTokenPurchaseUIResult, bool, bResult, FString, Message);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnTokenPackRequestCompleted, FModioTokenPackID, FModioErrorCode,
                                       TOptional<FModioTokenPack>);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListAllTokenPacksRequestCompleted, FModioErrorCode,
                                     TOptional<FModioTokenPackList>);

DECLARE_DELEGATE_TwoParams(FOnListAllTokenPacksDelegateFast, FModioErrorCode, TOptional<FModioTokenPackList>);
DECLARE_DELEGATE_TwoParams(FOnGetTokenPackDelegateFast, FModioErrorCode, TOptional<FModioTokenPack>);

DECLARE_MULTICAST_DELEGATE(FOnEntitlementRefreshRequest);

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FOnPreUninstallDelegate, FModioModID, ModID);


UENUM(BlueprintType)
enum class EModioUIFeatureFlags : uint8
{
	ModEnableDisable,
	Monetization,
	ModDownvote,
	ModCollections
};


/**
 * Default do-nothing implementation of IModioUIModEnabledStateProvider.
 */
UCLASS()
class MODIOUICORE_API UModioUIDefaultModEnabledStateProvider : public UObject, public IModioUIModEnabledStateProvider
{
	GENERATED_BODY()
};

/**
 * @brief The UI subsystem for mod.io
 */
UCLASS()
class MODIOUICORE_API UModioUISubsystem
	: public UEngineSubsystem,
	  public IModRatingStateProvider,
	  public IModCollectionRatingStateProvider
{
	GENERATED_BODY()

protected:
	virtual void GetPreloadDependencies(TArray<UObject*>& OutDeps) override;

	friend class IModioUIMediaDownloadCompletedReceiver;
	friend class IModioUIModInfoReceiver;
	friend class IModioUIModManagementEventReceiver;
	friend class IModioUIInputDeviceChangedReceiver;
	friend class IModioUIUserChangedReceiver;
	friend class IModioUIUserAvatarDownloadCompletedReceiver;
	friend class IModioUIAuthenticationChangedReceiver;
	friend class IModioUINotificationController;
	friend class IModioUIModDetailsDisplay;
	friend class IModioUIModEnabledStateChangedReceiver;
	friend class IModioUIDialogDisplayEventReceiver;
	friend class IModioUIConnectivityChangedReceiver;
	friend class IModioUIWalletBalanceUpdatedEventReceiver;
	friend class IModioUITokenPackReceiver;
	friend class IModioUIEntitlementRefreshEventReceiver;
	friend class IModioUISubscriptionsChangedReceiver;
	friend class IModioUIModCollectionInfoReceiver;
	friend class IModioUICollectionFollowStateChangedReceiver;

	#if WITH_EDITOR
	// These test widgets are friends so they can manually trigger subsystem delegates to emit mock events for in-editor
	// testing

	friend class SModioUIInstallationStatusTestWidget;

	#endif

	FOnModEnabledChanged OnModEnabledStateChanged;

	FOnDisplayDialogRequest OnDialogDisplayEvent;
	FOnEntitlementRefreshRequest OnEntitlementRefreshEvent;

	UPROPERTY(Transient)
	TObjectPtr<UObject> ModEnabledStateDataProvider;

	UPROPERTY(Transient)
	TObjectPtr<UObject> ModRatingStateProvider;

	UPROPERTY(Transient)
	TObjectPtr<UObject> ModCollectionRatingStateProvider;

	UFUNCTION()
	void OnModEnabledChanged(int64 RawModID, bool bNewEnabledState);

	// Perhaps this should also carry the error code and a TOptional<bool> for the newly changed state?
	FOnModSubscriptionStatusChanged OnSubscriptionStatusChanged;

	// Delegate for the subscription success or fail
	FOnSubscriptionCompleted OnSubscriptionRequestCompleted;

	FOnModCollectionFollowStateChanged OnModCollectionFollowStateChanged;
	FOnModCollectionFollowCompleted OnModCollectionFollowRequestComplete;

	FOnModCollectionFollowCompleted OnModCollectionSubscribeRequestComplete;
	FOnModCollectionFollowCompleted OnModCollectionUnsubscribeRequestComplete;

	void ModCollectionFollowHandler(FModioErrorCode ErrorCode, TOptional<FModioModCollectionInfo> CollectionInfo);
	void ModCollectionSubscribeHandler(FModioErrorCode ErrorCode, FModioModCollectionID CollectionID);

	UFUNCTION()
	void ModCollectionUnfollowHandler(FModioErrorCode ErrorCode, FModioModCollectionID CollectionID);
	void ModCollectionUnsubscribeHandler(FModioErrorCode ErrorCode, FModioModCollectionID CollectionID);

	UFUNCTION()
	void SubscriptionHandler(FModioErrorCode ErrorCode, FModioModID ID);

	void PurchaseRequestHandler(FModioErrorCode ErrorCode, TOptional<FModioTransactionRecord> Transaction);

	UFUNCTION()
	void UnsubscribeHandler(FModioErrorCode ErrorCode, FModioModID ID);

	UFUNCTION()
	void UninstallHandler(FModioErrorCode ErrorCode, FModioModID ID);

	FOnModLogoDownloadCompleted OnModLogoDownloadCompleted;
	void LogoDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image, FModioModID ID,
	                         EModioLogoSize LogoSize);

	FOnUserAvatarDownloadCompleted OnUserAvatarDownloadCompleted;
	void UserAvatarDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image);

	FOnModGalleryImageDownloadCompleted OnModGalleryImageDownloadCompleted;
	void GalleryImageDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image, FModioModID ID,
	                                 int32 Index);

	FOnModCreatorAvatarDownloadCompleted OnModCreatorAvatarDownloadCompleted;
	void CreatorAvatarDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image, FModioModID ID);

	FOnModCollectionLogoDownloadCompleted OnModCollectionLogoDownloadCompleted;
	void ModCollectionLogoDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image,
	                                      FModioModCollectionID ID, EModioLogoSize LogoSize);

	FOnModCollectionCuratorAvatarDownloadCompleted OnModCollectionCuratorAvatarDownloadCompleted;
	void ModCollectionCuratorAvatarDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image,
	                                               FModioModCollectionID ID);

	FOnConnectivityChanged OnConnectivityChanged;
	// The implementation currently assumes connectivity unless informed otherwise
	bool bCurrentConnectivityState = true;

	FOnAuthenticatedUserChanged OnUserChanged;
	FOnAuthenticationChangeStarted OnAuthenticationChangeStarted;

	UPROPERTY()
	FOnPreUninstallDelegate OnPreUninstall;

	void OnAuthenticationComplete(FModioErrorCode ErrorCode);

	FOnModInfoRequestCompleted OnModInfoRequestCompleted;
	void ModInfoRequestCompletedHandler(FModioErrorCode ErrorCode, TOptional<FModioModInfoList> ModInfos,
	                                    TArray<FModioModID> IDs);

	FOnModCollectionInfoRequestCompleted OnModCollectionInfoRequestCompleted;
	void ModCollectionInfoRequestCompletedHandler(FModioErrorCode ErrorCode,
	                                              TOptional<FModioModCollectionInfoList> ModCollectionInfos,
	                                              TArray<FModioModCollectionID> IDs);

	FOnListAllModsRequestCompleted OnListAllModsRequestCompleted;
	void ListAllModsCompletedHandler(FModioErrorCode ErrorCode, TOptional<FModioModInfoList> ModInfos,
	                                 FString RequestIdentifier);

	FOnListModCollectionsRequestCompleted OnListModCollectionsRequestCompleted;
	void ListModCollectionsCompletedHandler(FModioErrorCode ErrorCode,
	                                        TOptional<FModioModCollectionInfoList> ModCollectionInfos,
	                                        FString RequestIdentifier);

	FOnGetModCollectionModsRequestCompleted OnGetModCollectionModsRequestCompleted;
	void GetModCollectionModsCompletedHandler(FModioErrorCode ErrorCode, TOptional<FModioModInfoList> ModInfos,
	                                          FModioModCollectionID CollectionID);

	FOnTokenPackRequestCompleted OnTokenPackRequestCompleted;
	void TokenPackRequestCompletedHandler(FModioErrorCode ErrorCode, TOptional<FModioTokenPackList> TokenPacks,
	                                      TArray<FModioTokenPackID> IDs);

	FOnListAllTokenPacksRequestCompleted OnListAllTokenPacksRequestCompleted;
	void ListAllTokenPacksCompletedHandler(FModioErrorCode ErrorCode, TOptional<FModioTokenPackList> TokenPacks);

	FOnModManagementEventUI OnModManagementEvent;

	void ModManagementEventHandler(FModioModManagementEvent Event);

	void OnGetModTagOptionsComplete(FModioErrorCode ModioErrorCode, TOptional<FModioModTagOptions> ModioModTagInfos);

	FOnDisplayErrorManualParams OnDisplayManualParams;

	FOnPurchaseRequestCompleted OnPurchaseRequestCompleted;

	FOnGetUserWalletBalanceRequestCompleted OnGetUserWalletBalanceRequestCompleted;
	void WalletBalanceRequestHandler(FModioErrorCode ErrorCode, TOptional<uint64> Balance);

	FOnDisplayModDetails OnDisplayModDetails;
	FOnDisplayModDetailsForID OnDisplayModDetailsForID;

	TOptional<FModioModTagOptions> CachedModTags;

	FVector2D CachedMouseCursorLocation;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/**
	 * @docpublic
	 * @brief Sets the data provider object for handling Mod Enable/Disable actions/tracking.
	 * 
	 * @param InModEnabledStateDataProvider - The object implementing the IModioUIModEnabledStateProvider interface that will act as the data provider.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void SetModEnabledStateDataProvider(
		TScriptInterface<IModioUIModEnabledStateProvider> InModEnabledStateDataProvider);

	/**
	 * @docpublic
	 * @brief Sets the data provider object for handling Mod Rating actions/tracking.
	 *
	 * @param InModRatingStateProvider - The object implementing the IModRatingStateProvider interface that
	 * will act as the data provider.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void SetModRatingStateDataProvider(TScriptInterface<IModRatingStateProvider> InModRatingStateProvider);

	/**
	 * @docpublic
	 * @brief Sets the data provider object for handling Mod Collection Rating actions/tracking.
	 *
	 * @param InModCollectionRatingStateProvider - The object implementing the IModCollectionRatingStateProvider interface that
	 * will act as the data provider.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void SetModCollectionRatingStateDataProvider(
		TScriptInterface<IModCollectionRatingStateProvider> InModCollectionRatingStateProvider);

	/**
	 * @docpublic
	 * @brief Enables mod management, installing the UI subsystem as the mod management event handler so notifications can be broadcast to UI
	 * 
	 * @return An error code indicating success or failure of enabling mod management.  Note that this is independent of
	 * error codes for mod management events.  Inspect the `Callback` for information on each mod management event. 
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	FModioErrorCode EnableModManagement();

	/**
	 * @docpublic
	 * @brief Disables mod management.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void DisableModManagement();

	/**
	 * @docpublic
	 * @brief Subscribes the current user to the provided ModID, optionally subscribing to dependencies.
	 * 
	 * @param ID - The ModId of the Mod to be subscribed to.
	 * @param IncludeDependencies - Boolean indicating whether Mods that the given Mod depends on should also be subscribed to.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestSubscriptionForModID(FModioModID ID, bool IncludeDependencies);

	/**
	 * @docpublic
	 * @brief Subscribes the current user to the provided ModID, optionally subscribing to dependencies, executing the provided callback upon successfully subscribing.
	 *
	 * @param ID - The ModId of the Mod to be subscribed to.
	 * @param IncludeDependencies - Boolean indicating whether Mods that the given Mod depends on should also be
	 * subscribed to.
	 * @param Callback - The callback to be executed upon a successful subscription.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestSubscriptionForModIDWithHandler(FModioModID ID, bool IncludeDependencies,
	                                            FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Unsubscribes the current user from the given ModId.
	 * 
	 * @param ID - The ModId of the Mod to unsubscribe from.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestRemoveSubscriptionForModID(FModioModID ID);

	/**
	 * @docpublic
	 * @brief Unsubscribes the current user from the given ModId, executing the given callback upon completing the unsubscribing.
	 *
	 * @param ID - The ModId of the Mod to unsubscribe from.
	 * @param DedicatedCallback - The callback to be executed upon a successful unsubscription.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestRemoveSubscriptionForModIDWithHandler(FModioModID ID, FOnErrorOnlyDelegate DedicatedCallback);

	void OnRatingSubmissionComplete(FModioErrorCode ErrorCode, EModioRating ModioRating);
	void OnModCollectionRatingSubmissionComplete(FModioErrorCode ErrorCode, EModioRating ModioRating);
	void OnExternalUpdatesFetched(FModioErrorCode ErrorCode);
	void RequestRateUpForModId(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void RequestRateDownForModId(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void RequestRateUpForModCollectionId(FModioModCollectionID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void RequestRateDownForModCollectionId(FModioModCollectionID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void RequestUninstallForModID(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);

	void OnLogoutComplete(FModioErrorCode ErrorCode);
	void LogOut(FOnErrorOnlyDelegateFast DedicatedCallback);

	template<typename ClassOwner, class... Args, typename DelegateSignature, typename ImplementingClass>
	void RegisterEventHandler(TMulticastDelegate<DelegateSignature>& Callback,
	                          void (ClassOwner::*FunctionPointer)(Args...), ImplementingClass& ObjectToRegister)
	{
		if (FunctionPointer == nullptr)
		{
			return;
		}

		Callback.RemoveAll(&ObjectToRegister);
		Callback.AddUObject(&ObjectToRegister, FunctionPointer);
	}

	template<typename DelegateSignature, typename ImplementingClass>
	void DeregisterEventHandler(TMulticastDelegate<DelegateSignature>& Callback, ImplementingClass& ObjectToRegister)
	{
		Callback.RemoveAll(&ObjectToRegister);
	}

	template<typename DelegateSignature, typename Func>
	void RegisterEventHandlerFromK2(TMulticastDelegate<DelegateSignature>& Callback, Func* FunctionPointer,
	                                TMap<TWeakObjectPtr<>, FDelegateHandle>& Map,
	                                TWeakObjectPtr<UObject> ObjectToRegisterWeakPtr)
	{
		if (FunctionPointer == nullptr)
		{
			UE_LOG(ModioUICore, Warning, TEXT("Attempting to register a null function pointer to delegate"));
			return;
		}

		bool bContainsObject = false;
		// Safety clean up before adding a new one
		for (auto It = Map.CreateIterator(); It; ++It)
		{
			const auto RegisteredObject = It.Key().Get(false);
			if (!RegisteredObject)
			{
				Callback.Remove(It.Value());
				It.RemoveCurrent();
			}
			else if (RegisteredObject == ObjectToRegisterWeakPtr)
			{
				bContainsObject = true;
				break;
			}
		}

		// Ensure we don't add duplicates of the same object.
		if (!bContainsObject)
		{
			const FDelegateHandle Handle = Callback.AddStatic(FunctionPointer, ObjectToRegisterWeakPtr);
			Map.Add(ObjectToRegisterWeakPtr, Handle);
		}
	}

	template<typename DelegateSignature, typename Func>
	void DeregisterEventHandlerFromK2(TMulticastDelegate<DelegateSignature>& Callback, Func* FunctionPointer,
	                                  TMap<TWeakObjectPtr<>, FDelegateHandle>& Map,
	                                  TWeakObjectPtr<UObject> ObjectToDeregisterWeakPtr)
	{
		if (FunctionPointer == nullptr)
		{
			UE_LOG(ModioUICore, Warning, TEXT("Attempting to deregister a null function pointer from delegate"));
			return;
		}

		// Collapse delegate list, and remove our object from the delegate
		for (auto It = Map.CreateIterator(); It; ++It)
		{
			const auto RegisteredObject = It.Key().Get(false);
			if (!RegisteredObject)
			{
				Callback.Remove(It.Value());
				It.RemoveCurrent();
			}
			else if (RegisteredObject == ObjectToDeregisterWeakPtr)
			{
				Callback.Remove(It.Value());
				It.RemoveCurrent();
			}
		}
	}

	/**
	 * @docpublic
	 * @brief Registers a callback that will be invoked before mods are uninstalled.
	 *
	 * @param Callback - The Callback to invoke before uninstall. Receives the FModioModID of the mod being uninstalled, and must return a bool indicating approval.
	 */
	UFUNCTION(BlueprintCallable, Category="mod.io|UI|ModioUISubsystem")
	void RegisterPreUninstallHandler(const FOnPreUninstallDelegate& Callback)
	{
		OnPreUninstall = Callback;
	}

	/**
	 * @docpublic
	 * @brief Unregisters the currently bound pre-uninstall callback, disabling any veto logic before uninstalls.
	 */
	UFUNCTION(BlueprintCallable, Category="mod.io|UI|ModioUISubsystem")
	void UnregisterPreUninstallHandler()
	{
		OnPreUninstall.Unbind();
	}

	/**
	 * @docpublic
	 * @brief Requests download of the currently authenticated user's Avatar.
	 * Completion of the request triggers a callback in implementations of IModioUIUserAvatarDownloadCompletedReceiver
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestUserAvatar();

	/**
	 * @docpublic
	 * @brief Requests the authentication of a provided Authentication Code that has been entered by the user. Calls the callback upon completion.
	 * 
	 * @param Code - The authentication code that has been entered.
	 * @param Callback - The callback to be executed upon completion.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestEmailAuthenticationWithHandler(FModioEmailAuthCode Code, const FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Get a gallery image for the specified mod ID.
	 * Executes callbacks in implementations of IModioUIMediaDownloadCompletedReceiver
	 * 
	 * @param ID - The mod you want to retrieve an image for
	 * @param Index - The zero-based index of the image you want to retrieve
	 * @param ImageSize - Size of the image you want to retrieve
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestGalleryImageDownloadForModID(FModioModID ID, int32 Index,
	                                         EModioGallerySize ImageSize = EModioGallerySize::Original);

	/**
	 * @docpublic
	 * @brief Downloads the logo for the specified ModId.
	 * Executes callbacks in implementations of IModioUIMediaDownloadCompletedReceiver
	 * 
	 * @param ID - Mod ID for use in logo retrieval
	 * @param LogoSize - Parameter indicating the size of logo that's required
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestLogoDownloadForModID(FModioModID ID, EModioLogoSize LogoSize = EModioLogoSize::Thumb320);

	/**
	 * @docpublic
	 * @brief Downloads the logo for the specified ModCollectionId.
	 * Executes callbacks in implementations of IModioUIMediaDownloadCompletedReceiver
	 * 
	 * @param ID - Mod Collection ID for use in logo retrieval
	 * @param LogoSize - Parameter indicating the size of logo that's required
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestLogoDownloadForModCollectionID(FModioModCollectionID ID,
	                                           EModioLogoSize LogoSize = EModioLogoSize::Thumb320);

	TOptional<FModioModTagOptions> GetTagOptionsList();

	void GetTagOptionsListAsync();

	/**
	 * @docpublic
	 * @brief Gets the current DPI scale value of the UI based on the viewport size.
	 * 
	 * @return The scale of the UI.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	float GetCurrentDPIScaleValue();

	/**
	 * @docpublic
	 * @brief Requests a list of all Mods for the current game that match the given IDs.
	 * Executes callbacks in implementations of IModioUIModInfoReceiver
	 * 
	 * @param IDs - Array of ModIds to request information on.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestModInfoForModIDs(TArray<FModioModID> IDs);

	/**
	 * @docpublic
	 * @brief Requests a list of all Mods for the current game.
	 * Executes callbacks in implementations of IModioUIModInfoReceiver.
	 * 
	 * @param Params - A filter to apply to the results, returning only Mods that match it
	 * @param RequestIdentifier - For requesters to tell if a set of results or an error belongs to them
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestListAllMods(FModioFilterParams Params, FString RequestIdentifier);

	/**
	 * @docpublic
	 * @brief Requests a list of mod collections for the current game.
	 * Executes callbacks in implementations of IModioUIModCollectionInfoReceiver.
	 * 
	 * @param Filter - A filter to apply to the results, returning only Mod collections that match it
	 * @param RequestIdentifier - For requesters to tell if a set of results or an error belongs to them
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestListModCollections(const FModioFilterParams& Filter, FString RequestIdentifier);

	/**
	 * @docpublic
	 * @brief Requests The list of mods for the given Mod Collection.
	 * Executes callbacks in implementations of IModioUIModCollectionInfoReceiver.
	 *
	 * @param CollectionID - The ID of the collection to get all the mods for
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestGetModCollectionMods(FModioModCollectionID CollectionID);

	/**
	 * @docpublic
	 * @brief Requests a list of all purchasable Token Packs via the currently active online portal provider
	 * The result is received by any class implementing the ModioTokenPackReceiver interface via OnListAllTokenPacksRequestCompleted
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestListAllTokenPacks();

	/**
	 * @docpublic
	 * @brief Requests the purchase of the given Token Pack via the currently active online portal provider
	 * 
	 * @param TokenPackID - ID of the pack to purchase
	 * @param Callback - Executed upon completion of the purchase process, indicating success and any additional information
	 * @return Whether - the purchase process was successfully started. Does not indicate status of the purchase itself.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	bool RequestPurchaseTokenPack(FModioTokenPackID TokenPackID, const FOnPlatformCheckoutDelegate& Callback);

	/**
	 * @docpublic
	 * @brief Queries whether the given Mod is enabled. Not to be confused with whether a Mod is Subscribed to.
	 * 
	 * @param ID - The Id of the Mod to query the Enabled status of.
	 * @return Whether the Mod is enabled or not.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	bool QueryIsModEnabled(FModioModID ID);

	/**
	 * @docpublic
	 * @brief Requests an update of the currently authenticated user's mod.io wallet balance, creating a wallet if one does not already exist.
	 * Executes callbacks in implementations of IModioUIWalletBalanceUpdatedEventReceiver.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestWalletBalanceRefresh();

	/**
	 * @docpublic
	 * @brief Requests an update of the currently authenticated user's mod.io wallet balance, creating a wallet if one
	 * does not already exist. Also executes the given callback upon completion.
	 * Executes callbacks in implementations of IModioUIWalletBalanceUpdatedEventReceiver.
	 * 
	 * @param Callback - The callback to execute upon completion.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestWalletBalanceRefreshWithHandler(const FOnGetUserWalletBalanceDelegate& Callback);

	/**
	 * @docpublic
	 * @brief Purchases a mod for the current player, executing the callback upon completion.
	 * 
	 * @param ID - ID of the mod to purchase
	 * @param ExpectedPrice - The price the user is expected to pay for the mod, this ensures that there is consistency between the displayed price and
	 * the price in the backend. If there is a mismatch, the purchase will fail.
	 * @param Callback - Callback invoked with purchase information once the purchase is completed.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestPurchaseForModIDWithHandler(FModioModID ID, FModioUnsigned64 ExpectedPrice,
	                                        const FOnPurchaseModDelegate& Callback);

	/**
	 * @docpublic
	 * @brief Requests to change the Enabled state of the given Mod to the given State
	 * 
	 * @param ID - The ModID of the Mod to update
	 * @param bNewEnabledState - The state to apply to the given Mod.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	bool RequestModEnabledStateChange(FModioModID ID, bool bNewEnabledState);

	/**
	 * @docpublic
	 * @brief Requests the display of the given Dialog Type, providing the new dialog the given Data Source.
	 * Executes only in implementations of IModioDialogDisplayEventReceiver
	 * 
	 * @param DialogType - The type of dialog to display
	 * @param DataSource - The data to hand to the new dialog upon creation
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestShowDialog(EModioUIDialogType DialogType, UObject* DataSource);

	/**
	 * @docpublic
	 * @brief Updates the current Connectivity state, and notifies implementations of IMOdioUIConnectivityChangedReceiver *only* if the state changes.
	 * 
	 * @param bNewConnectivityState - the new Connectivity state
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void NotifyConnectivityChange(bool bNewConnectivityState);

	/**
	 * @docpublic
	 * @brief Gets the current Connectivity State
	 * 
	 * @return The current connectivity State.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	bool QueryConnectivityState();

	/**
	 * @docpublic
	 * @brief Indicates whether a UGC subsystem feature is enabled
	 *
	 * @param Feature The feature to query
	 * @return bool indicating the state of the specified feature
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|ModioUISubsystem")
	bool IsUGCFeatureEnabled(EModioUIFeatureFlags Feature);

	virtual bool NativeRequestModRatingChange(int64 ID, EModioRating NewRating) override;
	virtual EModioRating NativeQueryModRating(int64 ModID) override;

	virtual bool NativeRequestModCollectionRatingChange(int64 CollectionID, EModioRating NewRating) override;
	virtual EModioRating NativeQueryModCollectionRating(int64 ModCollectionID) override;

	/**
	 * @docpublic
	 * @brief Attempts to invoke the store for the current platform.
	 * 
	 * @return Indicates if the native store UI is supported on the current platform,
	 * i.e if the store has opened, *not* if a purchase was made.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	EModioOpenStoreResult RequestShowTokenPurchaseUI();

	/**
	 * @docpublic
	 * @brief Attempts to invoke the store for the current platform.
	 * 
	 * @param Callback A callback that returns a bool indicating whether the user made a purchase in the store
	 * @return Indicates if the native store UI is supported on the current platform, i.e if the store has opened, *not* if a purchase was made.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	EModioOpenStoreResult RequestShowTokenPurchaseUIWithHandler(const FOnShowTokenPurchaseUIResult& Callback);

	/**
	 * @docpublic
	 * @brief Requests a refreshing of the currently logged in user's entitlements (consuming them if possible).
	 * This then executes callbacks in implementations of IModioUIWalletBalanceUpdatedEventReceiver.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestRefreshEntitlements();

	/**
	 * @docpublic
	 * @brief Follows the given ModCollection ID for the current user.
	 *
	 * @param ID - The ModCollectionId of the Mod Collection to be followed.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestFollowModCollection(FModioModCollectionID ID);

	/**
	 * @docpublic
	 * @brief Follows the given Mod Collection ID for the current user, executing the
	 * provided callback upon successfully subscribing.
	 *
	 * @param ID - The ModCollectionId of the Mod to be subscribed to.
	 * @param Callback - The callback to be executed upon a successful follow.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestFollowModCollectionWithHandler(FModioModCollectionID ID,
	                                           FOnFollowModCollectionDelegate Callback);

	/**
	 * @docpublic
	 * @brief Unfollows the given Mod Collection Id for the current user.
	 *
	 * @param ID - The ModCollectionId of the Mod collection to unfollow.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestUnfollowModCollection(FModioModCollectionID ID);

	/**
	 * @docpublic
	 * @brief Unfollows the given Mod collection ID for the current user, executing the given callback upon completing the
	 * unsubscribing.
	 *
	 * @param ID - The ModCollectionId of the Mod collection to unfollow.
	 * @param DedicatedCallback - The callback to be executed upon a successful unfollow.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestUnfollowModCollectionWithHandler(FModioModCollectionID ID,
	                                             FOnErrorOnlyDelegate DedicatedCallback);

	/**
	 * @docpublic
	 * @brief Subscribes the current user to the provided ModCollectionID.
	 *
	 * @param ID - The ModCollectionId of the Mod Collection to be subscribed to.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestSubscribeToModCollection(FModioModCollectionID ID);

	/**
	 * @docpublic
	 * @brief Subscribes the current user to the provided ModCollectionID, executing the
	 * provided callback upon successfully subscribing.
	 *
	 * @param ID - The ModCollectionId of the Mod Collection to be subscribed to.
	 * @param Callback - The callback to be executed upon a successful subscription.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestSubscribeToModCollectionWithHandler(FModioModCollectionID ID,
	                                                FOnErrorOnlyDelegate Callback);

	/**
	 * @docpublic
	 * @brief Unsubscribes the current user from the given ModCollectionId.
	 *
	 * @param ID - The ModId of the Mod Collection to unsubscribe from.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestUnsubscribeFromModCollection(FModioModCollectionID ID);

	/**
	 * @docpublic
	 * @brief Unsubscribes the current user from the given ModCollectionId, executing the given callback upon completing the
	 * unsubscribing.
	 *
	 * @param ID - The ModCollectionId of the Mod Collection to unsubscribe from.
	 * @param DedicatedCallback - The callback to be executed upon a successful unsubscription.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestUnsubscribeFromModCollectionWithHandler(FModioModCollectionID ID,
	                                                    FOnErrorOnlyDelegate DedicatedCallback);

	/**
	 * @docpublic
	 * @brief Checks if the current user is following to the given ModCollectionId, returning the result in a delegate
	 *
	 * @param ID - The ModCollectionId of the Mod Collection to check is followed.
	 */
	void QueryIsUserFollowingModCollection(FModioModCollectionID ID);

	/**
	 * @docpublic
	 * @brief Checks if the current user is following to the given ModCollectionId, returning the result in a delegate
	 *
	 * @param ID - The ModCollectionId of the Mod Collection to check is followed.
	 * @param Handler - The callback to be executed upon a successful check.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void QueryIsUserFollowingModCollectionWithHandler(FModioModCollectionID ID,
	                                                  FOnQueryFollowedModCollectionCompleted Handler);

private:
	TMap<int64, EModioRating> ModRatingMap;
	TMap<int64, EModioRating> ModCollectionRatingMap;
};