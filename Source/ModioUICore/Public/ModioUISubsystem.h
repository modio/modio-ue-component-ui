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

#include "ModioUISubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSubscriptionCompleted, FModioErrorCode, FModioModID);

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnModSubscriptionStatusChanged, FModioModID, bool);

DECLARE_MULTICAST_DELEGATE_FourParams(FOnModLogoDownloadCompleted, FModioModID, FModioErrorCode,
									  TOptional<FModioImageWrapper>, EModioLogoSize);

DECLARE_MULTICAST_DELEGATE_FourParams(FOnModGalleryImageDownloadCompleted, FModioModID, FModioErrorCode, int32,
									  TOptional<FModioImageWrapper>);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUserAvatarDownloadCompleted, FModioErrorCode, TOptional<FModioImageWrapper>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnModCreatorAvatarDownloadCompleted, FModioModID, FModioErrorCode,
									   TOptional<FModioImageWrapper>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnModInfoRequestCompleted, FModioModID, FModioErrorCode,
									   TOptional<FModioModInfo>);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnListAllModsRequestCompleted, FString, FModioErrorCode,
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


UENUM(BlueprintType)
enum class EModioUIFeatureFlags : uint8
{
	ModEnableDisable, 
	Monetization,
	ModDownvote,
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
UCLASS() class MODIOUICORE_API UModioUISubsystem : public UEngineSubsystem, public IModRatingStateProvider
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

#if WITH_EDITOR
	// These test widgets are friends so they can manually trigger subsystem delegates to emit mock events for in-editor
	// testing

	friend class SModioUIInstallationStatusTestWidget;

#endif

	FOnModEnabledChanged OnModEnabledStateChanged;

	FOnDisplayDialogRequest OnDialogDisplayEvent;
	FOnEntitlementRefreshRequest OnEntitlementRefreshEvent;

	UPROPERTY(Transient)
	UObject* ModEnabledStateDataProvider;

	UPROPERTY(Transient)
	UObject* ModRatingStateProvider;

	UFUNCTION()
	void OnModEnabledChanged(int64 RawModID, bool bNewEnabledState);

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

	FOnConnectivityChanged OnConnectivityChanged;
	// The implementation currently assumes connectivity unless informed otherwise
	bool bCurrentConnectivityState = true;

	FOnAuthenticatedUserChanged OnUserChanged;
	FOnAuthenticationChangeStarted OnAuthenticationChangeStarted;

	void OnAuthenticationComplete(FModioErrorCode ec);

	FOnModInfoRequestCompleted OnModInfoRequestCompleted;
	void ModInfoRequestCompletedHandler(FModioErrorCode ErrorCode, TOptional<FModioModInfoList> ModInfos,
										TArray<FModioModID> IDs);

	FOnListAllModsRequestCompleted OnListAllModsRequestCompleted;
	void ListAllModsCompletedHandler(FModioErrorCode ErrorCode, TOptional<FModioModInfoList> ModInfos,
									 FString RequestIdentifier);

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

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void SetModEnabledStateDataProvider(TScriptInterface<IModioUIModEnabledStateProvider> InModEnabledStateDataProvider);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void SetModRatingStateDataProvider(TScriptInterface<IModRatingStateProvider> InModRatingStateProvider);

	/**
	 * Enables mod management, installing the UI subsystem as the mod management event handler so notifications can be
	 * broadcast to UI
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	FModioErrorCode EnableModManagement();

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void DisableModManagement();

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestSubscriptionForModID(FModioModID ID, bool IncludeDependencies);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestSubscriptionForModIDWithHandler(FModioModID ID, bool IncludeDependencies, FOnErrorOnlyDelegate Callback);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestRemoveSubscriptionForModID(FModioModID ID);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestRemoveSubscriptionForModIDWithHandler(FModioModID ID, FOnErrorOnlyDelegate DedicatedCallback);

	void OnRatingSubmissionComplete(FModioErrorCode ErrorCode, EModioRating ModioRating);
	void OnExternalUpdatesFetched(FModioErrorCode ErrorCode);
	void RequestRateUpForModId(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void RequestRateDownForModId(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);
	void RequestUninstallForModID(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback);

	void OnLogoutComplete(FModioErrorCode ModioErrorCode);
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
		if(FunctionPointer == nullptr)
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

	// Delegate for the subscription success or fail
	FOnSubscriptionCompleted OnSubscriptionRequestCompleted;

	// Perhaps this should also carry the error code and a TOptional<bool> for the newly changed state?
	FOnModSubscriptionStatusChanged OnSubscriptionStatusChanged;

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestUserAvatar();

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestEmailAuthenticationWithHandler(FModioEmailAuthCode Code, const FOnErrorOnlyDelegate Callback);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestGalleryImageDownloadForModID(FModioModID ID, int32 Index,
											 EModioGallerySize ImageSize = EModioGallerySize::Original);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestLogoDownloadForModID(FModioModID ID, EModioLogoSize LogoSize = EModioLogoSize::Thumb320);

	TOptional<FModioModTagOptions> GetTagOptionsList();

	void GetTagOptionsListAsync();

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	float GetCurrentDPIScaleValue();

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestModInfoForModIDs(TArray<FModioModID> IDs);

	// Uses RequestIdentifier so requesters can tell if a set of results or an error belongs to them
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestListAllMods(FModioFilterParams Params, FString RequestIdentifier);

	/**
	 * Requests a list of all purchasable Token Packs via the currently active online portal provider
	 * The result is received by any class implementing the ModioTokenPackReceiver interface via OnListAllTokenPacksRequestCompleted
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestListAllTokenPacks();

	/**
	 * Requests the purchase of the given Token Pack via the currently active online portal provider
	 * @param TokenPackID ID of the pack to purchase
	 * @param Callback Executed upon completion of the purchase process, indicating success and any additional information
	 * @return Whether the purchase process was successfully started. Does not indicate status of the purchase itself.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	bool RequestPurchaseTokenPack(FModioTokenPackID TokenPackID, const FOnPlatformCheckoutDelegate& Callback);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	bool QueryIsModEnabled(FModioModID ID);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestWalletBalanceRefresh();

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestWalletBalanceRefreshWithHandler(const FOnGetUserWalletBalanceDelegate& Callback);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestPurchaseForModIDWithHandler(FModioModID ID, FModioUnsigned64 ExpectedPrice,
											const FOnPurchaseModDelegate& Callback);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	bool RequestModEnabledStateChange(FModioModID ID, bool bNewEnabledState);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestShowDialog(EModioUIDialogType DialogType, UObject* DataSource);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void NotifyConnectivityChange(bool bNewConnectivityState);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	bool QueryConnectivityState();

	/**
	 * Indicates whether a UGC subsystem feature is enabled
	 *
	 * @param Feature The feature to query
	 * @return bool indicating the state of the specified feature
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|ModioUISubsystem")
	bool IsUGCFeatureEnabled(EModioUIFeatureFlags Feature);

	virtual bool NativeRequestModRatingChange(int64 ID, EModioRating NewRating) override;
	virtual EModioRating NativeQueryModRating(int64 ModID) override;

	/**
	 * Attempts to invoke the store for the current platform.
	 * @return Indicates if the native store UI is supported on the current platform,
	 * i.e if the store has opened, *not* if a purchase was made.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	EModioOpenStoreResult RequestShowTokenPurchaseUI();

	/**
	 * Attempts to invoke the store for the current platform.
	 * @param Callback A callback that returns a bool indicating whether the user made a purchase in the store
	 * @return Indicates if the native store UI is supported on the current platform, i.e if the store has opened, *not* if a purchase was made.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	EModioOpenStoreResult RequestShowTokenPurchaseUIWithHandler(const FOnShowTokenPurchaseUIResult& Callback);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUISubsystem")
	void RequestRefreshEntitlements();

private:
	TMap<int64, EModioRating> ModRatingMap;
};
