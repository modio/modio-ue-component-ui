/*
 *  Copyright (C) 2024-2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "ModioUISubsystem.h"

#include "Modio.h"
#include "ModioSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Core/ModioModInfoUI.h"
#include "Delegates/DelegateCombinations.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/UserInterfaceSettings.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Libraries/ModioErrorConditionLibrary.h"
#include "Loc/BeginModioLocNamespace.h"
#include "Math/IntPoint.h"
#include "ModioErrorCondition.h"
#include "ModioSettings.h"
#include "ModioSubsystem.h"
#include "ModioUICore.h"
#include "OnlineSubsystem.h"

#include "Interfaces/OnlineExternalUIInterface.h"
#include "Interfaces/OnlineStoreInterfaceV2.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "Interfaces/OnlinePurchaseInterface.h"
#include "Interfaces/OnlineEntitlementsInterface.h"
#include "Libraries/ModioPlatformHelpersLibrary.h"
#include "Libraries/ModioSDKLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioUISubsystem)

void UModioUISubsystem::GetPreloadDependencies(TArray<UObject*>& OutDeps)
{
	Super::GetPreloadDependencies(OutDeps);
}

bool UModioUISubsystem::QueryIsModEnabled(FModioModID ID)
{
	if (!ModEnabledStateDataProvider)
	{
		return true;
	}
	return IModioUIModEnabledStateProvider::Execute_QueryIsModEnabled(ModEnabledStateDataProvider, ID);
}

void UModioUISubsystem::RequestWalletBalanceRefresh()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetUserWalletBalanceAsync(
			FOnGetUserWalletBalanceDelegateFast::CreateUObject(this, &UModioUISubsystem::WalletBalanceRequestHandler));
	}
}

void UModioUISubsystem::RequestWalletBalanceRefreshWithHandler(const FOnGetUserWalletBalanceDelegate& Callback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetUserWalletBalanceAsync(FOnGetUserWalletBalanceDelegateFast::CreateLambda(
			[HookedHandler = FOnGetUserWalletBalanceDelegateFast::CreateUObject(
					this, &UModioUISubsystem::WalletBalanceRequestHandler),
				Callback](FModioErrorCode ec, TOptional<uint64> Balance) {
				Callback.ExecuteIfBound(ec, FModioOptionalUInt64{Balance});
				HookedHandler.ExecuteIfBound(ec, Balance);
			}));
	}
}

void UModioUISubsystem::RequestPurchaseForModIDWithHandler(FModioModID ID, FModioUnsigned64 ExpectedPrice,
                                                           const FOnPurchaseModDelegate& Callback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->PurchaseModAsync(ID, ExpectedPrice.Underlying,
		                            FOnPurchaseModDelegateFast::CreateLambda(
			                            [HookedHandler = FOnPurchaseModDelegateFast::CreateUObject(
					                            this, &UModioUISubsystem::PurchaseRequestHandler),
				                            Callback](FModioErrorCode ec,
				                                      TOptional<FModioTransactionRecord> Transaction) {
				                            Callback.ExecuteIfBound(ec, FModioOptionalTransactionRecord{Transaction});
				                            HookedHandler.ExecuteIfBound(ec, Transaction);
			                            }));
	}
}

bool UModioUISubsystem::RequestModEnabledStateChange(FModioModID ID, bool bNewEnabledState)
{
	if (!ModEnabledStateDataProvider)
	{
		return false;
	}

	return IModioUIModEnabledStateProvider::Execute_RequestModEnabledStateChange(
		ModEnabledStateDataProvider, ID, bNewEnabledState);
}

void UModioUISubsystem::RequestShowDialog(EModioUIDialogType DialogType, UObject* DataSource)
{
	OnDialogDisplayEvent.Broadcast(DialogType, DataSource);
}

void UModioUISubsystem::NotifyConnectivityChange(bool bNewConnectivityState)
{
	if (bCurrentConnectivityState != bNewConnectivityState)
	{
		bCurrentConnectivityState = bNewConnectivityState;
		OnConnectivityChanged.Broadcast(bCurrentConnectivityState);
	}
}

bool UModioUISubsystem::QueryConnectivityState()
{
	return bCurrentConnectivityState;
}

bool UModioUISubsystem::IsUGCFeatureEnabled(EModioUIFeatureFlags Feature)
{
	// Copy of the same function in UUGCSubsystem

	const UModioSettings* ModioConfiguration = GetDefault<UModioSettings>();
	if (ModioConfiguration)
	{
		switch (Feature)
		{
			case EModioUIFeatureFlags::ModDownvote:
				return ModioConfiguration->bEnableModDownvoteFeature;
			case EModioUIFeatureFlags::Monetization:
				return ModioConfiguration->bEnableMonetizationFeature;
			case EModioUIFeatureFlags::ModEnableDisable:
				return ModioConfiguration->bEnableModEnableDisableFeature;
			case EModioUIFeatureFlags::ModCollections:
				return ModioConfiguration->bEnableModCollectionsFeature;
			default:
				return false;
		}
	}
	return false;
}

void UModioUISubsystem::OnModEnabledChanged(int64 RawModID, bool bNewEnabledState)
{
	OnModEnabledStateChanged.Broadcast(FModioModID(RawModID), bNewEnabledState);
}

void UModioUISubsystem::ModCollectionFollowHandler(FModioErrorCode ErrorCode,
                                                   TOptional<FModioModCollectionInfo> CollectionInfo)
{
	OnModCollectionFollowRequestComplete.Broadcast(ErrorCode, CollectionInfo.GetValue().Id);
	if (!ErrorCode)
	{
		OnModCollectionFollowStateChanged.Broadcast(CollectionInfo.GetValue().Id, true);
	}
	else
	{
		UE_LOG(ModioUICore, Error, TEXT("Follow failed for mod collection %s: \"%s\""),
		       *CollectionInfo.GetValue().Id.ToString(),
		       *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::ModCollectionSubscribeHandler(FModioErrorCode ErrorCode, FModioModCollectionID CollectionID)
{
	OnModCollectionSubscribeRequestComplete.Broadcast(ErrorCode, CollectionID);
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Error, TEXT("Subscribe failed for mod collection %s: \"%s\""),
		       *CollectionID.ToString(), *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::ModCollectionUnfollowHandler(FModioErrorCode ErrorCode,
                                                     FModioModCollectionID CollectionID)
{
	if (!ErrorCode)
	{
		OnModCollectionFollowStateChanged.Broadcast(CollectionID, false);
	}
	else
	{
		UE_LOG(ModioUICore, Error, TEXT("Unfollow failed for mod collection %s: \"%s\""),
		       *CollectionID.ToString(),
		       *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::ModCollectionUnsubscribeHandler(FModioErrorCode ErrorCode, FModioModCollectionID CollectionID)
{
	OnModCollectionUnsubscribeRequestComplete.Broadcast(ErrorCode, CollectionID);
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Error, TEXT("Unsubscribe failed for mod collection %s: \"%s\""),
		       *CollectionID.ToString(), *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::SubscriptionHandler(FModioErrorCode ErrorCode, FModioModID ID)
{
	OnSubscriptionRequestCompleted.Broadcast(ErrorCode, ID);
	if (!ErrorCode)
	{
		OnSubscriptionStatusChanged.Broadcast(ID, true);
	}
	else
	{
		UE_LOG(ModioUICore, Error, TEXT("Subscription failed for mod %s: \"%s\""), *ID.ToString(),
		       *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::PurchaseRequestHandler(FModioErrorCode ErrorCode,
                                               TOptional<FModioTransactionRecord> Transaction)
{
	OnPurchaseRequestCompleted.Broadcast(ErrorCode, Transaction);
	if (!ErrorCode)
	{
		OnGetUserWalletBalanceRequestCompleted.Broadcast(Transaction.GetValue().UpdatedUserWalletBalance.Underlying);
		OnSubscriptionStatusChanged.Broadcast(Transaction.GetValue().AssociatedModID, true);
	}
	else
	{
		UE_LOG(ModioUICore, Error, TEXT("Purchase failed: \"%s\""), *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::UnsubscribeHandler(FModioErrorCode ErrorCode, FModioModID ID)
{
	if (!ErrorCode)
	{
		OnSubscriptionStatusChanged.Broadcast(ID, false);
	}
	else
	{
		UE_LOG(ModioUICore, Error, TEXT("Unsubscribe failed for mod %s: \"%s\""), *ID.ToString(),
		       *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::UninstallHandler(FModioErrorCode ErrorCode, FModioModID ID)
{
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Error, TEXT("Uninstall failed for mod %s: \"%s\""), *ID.ToString(),
		       *ErrorCode.GetErrorMessage());
	}

	// Need to create a synthetic FModioModManagementEvent to let the UI know an uninstallation has occurred.
	// UninstallHandler is used by ForceUninstallAsync, which does not emit a ModManagementEvent as it's an async
	// function with a callback, and is not used by the mod management loop.
	FModioModManagementEvent Event;
	Event.ID = ID;
	Event.Event = EModioModManagementEventType::Uninstalled;
	Event.Status = ErrorCode;
	OnModManagementEvent.Broadcast(Event);
}

void UModioUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Collection.InitializeDependency(UModioSubsystem::StaticClass());

	SetModRatingStateDataProvider(this);
	SetModCollectionRatingStateDataProvider(this);
}

void UModioUISubsystem::SetModEnabledStateDataProvider(
	TScriptInterface<IModioUIModEnabledStateProvider> InModEnabledStateDataProvider)
{
	ModEnabledStateDataProvider = InModEnabledStateDataProvider.GetObject();

	if (ModEnabledStateDataProvider)
	{
		FModioModEnabledStateChangeHandler Handler;
		Handler.BindDynamic(this, &UModioUISubsystem::OnModEnabledChanged);
		IModioUIModEnabledStateProvider::Execute_AddModEnabledStateChangeHandler(ModEnabledStateDataProvider, Handler);
	}
}

FModioErrorCode UModioUISubsystem::EnableModManagement()
{
	if (UModioSubsystem* DependencySubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		return DependencySubsystem->EnableModManagement(
			FOnModManagementDelegateFast::CreateUObject(this, &UModioUISubsystem::ModManagementEventHandler));
	}
	return FModioErrorCode::SystemError();
}

void UModioUISubsystem::DisableModManagement()
{
	if (UModioSubsystem* DependencySubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		DependencySubsystem->DisableModManagement();
	}
}

void UModioUISubsystem::RequestSubscriptionForModID(FModioModID ID, bool IncludeDependencies)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->SubscribeToModAsync(
			ID, IncludeDependencies,
			FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::SubscriptionHandler, ID));
	}
}

void UModioUISubsystem::RequestSubscriptionForModIDWithHandler(FModioModID ID, bool IncludeDependencies,
                                                               FOnErrorOnlyDelegate Callback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->SubscribeToModAsync(
			ID, IncludeDependencies,
			FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
						this, &UModioUISubsystem::SubscriptionHandler, ID),
					Callback](FModioErrorCode ec) {
					Callback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::RequestRemoveSubscriptionForModIDWithHandler(FModioModID ID,
                                                                     FOnErrorOnlyDelegate DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		// Only proceed with uninstall if the delegate returns true or is not bound
		if (OnPreUninstall.IsBound() && !OnPreUninstall.Execute(ID))
		{
			UE_LOG(ModioUICore, Warning, TEXT("Uninstall for mod %s was prevented by PreUninstall delegate"),
			       *ID.ToString());
			DedicatedCallback.ExecuteIfBound(FModioErrorCode::CancelledError());
			return;
		}

		Subsystem->UnsubscribeFromModAsync(
			ID, FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
						this, &UModioUISubsystem::UnsubscribeHandler, ID),
					DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::RequestUninstallForModID(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		// Only proceed with uninstall if the delegate returns true or is not bound
		if (OnPreUninstall.IsBound() && !OnPreUninstall.Execute(ID))
		{
			UE_LOG(ModioUICore, Warning, TEXT("Uninstall for mod %s was prevented by PreUninstall delegate"),
			       *ID.ToString());
			DedicatedCallback.ExecuteIfBound(FModioErrorCode::CancelledError());
			return;
		}

		Subsystem->ForceUninstallModAsync(
			ID, FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
						this, &UModioUISubsystem::UninstallHandler, ID),
					DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::RequestRateUpForModId(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		// Needs additional payload param so we know which type of operation was completed
		Subsystem->SubmitModRatingAsync(
			ID, EModioRating::Positive,
			FOnErrorOnlyDelegateFast::CreateLambda(
				[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
						this, &UModioUISubsystem::OnRatingSubmissionComplete, EModioRating::Positive),
					DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::RequestRateDownForModId(FModioModID ID, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		// Needs additional payload param so we know which type of operation was completed
		Subsystem->SubmitModRatingAsync(
			ID, EModioRating::Negative,
			FOnErrorOnlyDelegateFast::CreateLambda(
				[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
						this, &UModioUISubsystem::OnRatingSubmissionComplete, EModioRating::Negative),
					DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::RequestRateUpForModCollectionId(FModioModCollectionID ID,
                                                        FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		// Needs additional payload param so we know which type of operation was completed
		Subsystem->SubmitModCollectionRatingAsync(
			ID, EModioRating::Positive,
			FOnErrorOnlyDelegateFast::CreateLambda(
				[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
						this, &UModioUISubsystem::OnModCollectionRatingSubmissionComplete, EModioRating::Positive),
					DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::RequestRateDownForModCollectionId(FModioModCollectionID ID,
                                                          FOnErrorOnlyDelegateFast DedicatedCallback)
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		// Needs additional payload param so we know which type of operation was completed
		Subsystem->SubmitModCollectionRatingAsync(
			ID, EModioRating::Negative,
			FOnErrorOnlyDelegateFast::CreateLambda(
				[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
						this, &UModioUISubsystem::OnRatingSubmissionComplete, EModioRating::Negative),
					DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::OnRatingSubmissionComplete(FModioErrorCode ErrorCode, EModioRating ModioRating)
{
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to submit rating %s: \"%s\""), *UEnum::GetValueAsString(ModioRating),
		       *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::OnModCollectionRatingSubmissionComplete(FModioErrorCode ErrorCode, EModioRating ModioRating)
{
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to submit mod collection rating %s: \"%s\""),
		       *UEnum::GetValueAsString(ModioRating),
		       *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::OnExternalUpdatesFetched(FModioErrorCode ErrorCode)
{
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to fetch external updates: \"%s\""), *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::RequestRemoveSubscriptionForModID(FModioModID ID)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		// Only proceed with uninstall if the delegate returns true or is not bound
		if (OnPreUninstall.IsBound() && !OnPreUninstall.Execute(ID))
		{
			UE_LOG(ModioUICore, Warning, TEXT("Uninstall for mod %s was prevented by PreUninstall delegate"),
			       *ID.ToString());
			return;
		}

		Subsystem->UnsubscribeFromModAsync(
			ID, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::UnsubscribeHandler, ID));
	}
}

void UModioUISubsystem::RequestLogoDownloadForModID(FModioModID ID,
                                                    EModioLogoSize LogoSize /*= EModioLogoSize::Thumb320*/)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModMediaAsync(
			ID, LogoSize,
			FOnGetMediaDelegateFast::CreateUObject(this, &UModioUISubsystem::LogoDownloadHandler, ID, LogoSize));
	}
}

void UModioUISubsystem::RequestLogoDownloadForModCollectionID(FModioModCollectionID ID, EModioLogoSize LogoSize)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModCollectionMediaAsync(ID, LogoSize,
		                                      FOnGetMediaDelegateFast::CreateUObject(
			                                      this, &UModioUISubsystem::ModCollectionLogoDownloadHandler, ID,
			                                      LogoSize));
	}
}

float UModioUISubsystem::GetCurrentDPIScaleValue()
{
	FVector2D ViewportSize;

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		return GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())
			->GetDPIScaleBasedOnSize(FIntPoint(FGenericPlatformMath::FloorToInt(ViewportSize.X),
			                                   FGenericPlatformMath::FloorToInt(ViewportSize.Y)));
	}
	return 1.0f;
}

void UModioUISubsystem::RequestModInfoForModIDs(TArray<FModioModID> IDs)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		// Bind the list of IDs as an additional parameter so that if we get an error we can report the error for the
		// specific IDs that were queried
		Subsystem->ListAllModsAsync(
			FModioFilterParams().MatchingIDs(IDs),
			FOnListAllModsDelegateFast::CreateUObject(this, &UModioUISubsystem::ModInfoRequestCompletedHandler, IDs));
	}
}

void UModioUISubsystem::RequestListAllMods(FModioFilterParams Params, FString RequestIdentifier)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->ListAllModsAsync(
			Params, FOnListAllModsDelegateFast::CreateUObject(this, &UModioUISubsystem::ListAllModsCompletedHandler,
			                                                  RequestIdentifier));
	}
}

void UModioUISubsystem::RequestListModCollections(const FModioFilterParams& Filter, FString RequestIdentifier)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->ListModCollectionsAsync(
			Filter, FOnListModCollectionsDelegateFast::CreateUObject(
				this, &UModioUISubsystem::ListModCollectionsCompletedHandler,
				RequestIdentifier));
	}
}

void UModioUISubsystem::RequestGetModCollectionMods(FModioModCollectionID CollectionID)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModCollectionModsAsync(
			CollectionID, FOnGetModCollectionModsDelegateFast::CreateUObject(
				this, &UModioUISubsystem::GetModCollectionModsCompletedHandler, CollectionID));
	}
}

void UModioUISubsystem::RequestListAllTokenPacks()
{
	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::GetByPlatform())
	{
		if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			if (OnlineSubsystem->GetStoreV2Interface().IsValid())
			{
				if (OnlineSubsystem->GetIdentityInterface().IsValid())
				{
					EModioPortal CurrentPortal = ModioSubsystem->GetCurrentPortal();

					FUniqueNetIdPtr Id = OnlineSubsystem->GetIdentityInterface()->GetUniquePlayerId(0);
					FOnlineStoreFilter Filter;
					Filter.IncludeCategories.Add({});
					FOnQueryOnlineStoreOffersComplete OnQueryOffersComplete;

					OnQueryOffersComplete.BindLambda([this, Id, OnlineSubsystem](bool bWasSuccessful,
						const TArray<FUniqueOfferId>& OfferIds,
						const FString& Error) {
							if (bWasSuccessful && !OfferIds.IsEmpty())
							{
								TArray<FModioTokenPack> Offers;
								for (const FUniqueOfferId& Offer : OfferIds)
								{
									// We get the details of the offers from the cache, this is not async
									Offers.Add(
										FModioTokenPack(
											*OnlineSubsystem->GetStoreV2Interface()->GetOffer(Offer).Get()));
								}
								FModioTokenPackList OffersList = FModioTokenPackList(Offers);
								ListAllTokenPacksCompletedHandler(FModioErrorCode(), FModioTokenPackList(Offers));
							}
							else
							{
								ListAllTokenPacksCompletedHandler(FModioErrorCode::SystemError(), {});
							}
						});
					// This callback is guaranteed
					OnlineSubsystem->GetStoreV2Interface()->QueryOffersByFilter(*Id.Get(), Filter,
						OnQueryOffersComplete);
					return;
				}
			}
		}
	}

	// We failed somewhere, so call the handler with Error
	ListAllTokenPacksCompletedHandler(FModioErrorCode::SystemError(), {});
}

bool UModioUISubsystem::RequestPurchaseTokenPack(FModioTokenPackID TokenPackID,
                                                 const FOnPlatformCheckoutDelegate& Callback)
{
	if (TokenPackID.ToString().IsEmpty())
	{
		Callback.Execute(false, "Tried to purchase a token pack with an invalid ID");
		return false;
	}

	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::GetByPlatform())
	{
		if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			if (OnlineSubsystem->GetPurchaseInterface().IsValid()
			    && OnlineSubsystem->GetIdentityInterface().IsValid())
			{
				EModioPortal CurrentPortal = ModioSubsystem->GetCurrentPortal();
				FUniqueNetIdPtr Id = OnlineSubsystem->GetIdentityInterface()->GetUniquePlayerId(0);
				FPurchaseCheckoutRequest PurchaseRequest;
				PurchaseRequest.AddPurchaseOffer("", TokenPackID.ToString(), 1, true);
				FOnPurchaseCheckoutComplete CheckoutCallback;
				CheckoutCallback.BindLambda(
					[this, ModioSubsystem, OnlineSubsystem, Callback, Id](const FOnlineError& Error,
					                                                      const TSharedRef<FPurchaseReceipt>& Receipt) {
						if (!Error.WasSuccessful())
						{
							Callback.Execute(false, "Checkout failed with error: " + Error.ErrorRaw);
							return;
						}
						OnlineSubsystem->GetPurchaseInterface()->FinalizePurchase(
							*Id.Get(), Receipt.Get().TransactionId);
						Callback.Execute(true, "Successfully purchased offer.");
					});
				OnlineSubsystem->GetPurchaseInterface()->Checkout(*Id.Get(), PurchaseRequest, CheckoutCallback);
				return true;
			}
			Callback.Execute(false, "No purchase, entitlement, or identity interface");
			return false;
		}
		Callback.Execute(false, "No modio subsystem");
		return false;
	}
	Callback.Execute(false, "No online subsystem");
	return false;
}

void UModioUISubsystem::LogoDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image,
                                            FModioModID ID, EModioLogoSize LogoSize)
{
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to download logo for mod %s: \"%s\""), *ID.ToString(),
		       *ErrorCode.GetErrorMessage());
	}
	OnModLogoDownloadCompleted.Broadcast(ID, ErrorCode, Image, LogoSize);
}

void UModioUISubsystem::UserAvatarDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image)
{
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to download user avatar: \"%s\""), *ErrorCode.GetErrorMessage());
	}
	OnUserAvatarDownloadCompleted.Broadcast(ErrorCode, Image);
}

void UModioUISubsystem::RequestUserAvatar()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetUserMediaAsync(
			EModioAvatarSize::Thumb100,
			FOnGetMediaDelegateFast::CreateUObject(this, &UModioUISubsystem::UserAvatarDownloadHandler));
	}
}

void UModioUISubsystem::RequestEmailAuthenticationWithHandler(FModioEmailAuthCode Code,
                                                              const FOnErrorOnlyDelegate Callback)
{
	OnAuthenticationChangeStarted.Broadcast();

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->AuthenticateUserEmailAsync(
			Code, FOnErrorOnlyDelegateFast::CreateLambda(
				[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
					this, &UModioUISubsystem::OnAuthenticationComplete), Callback](FModioErrorCode ec) {
					Callback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::RequestGalleryImageDownloadForModID(
	FModioModID ID, int32 Index, EModioGallerySize ImageSize /*= EModioGallerySize::Original*/)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModMediaAsync(
			ID, ImageSize, Index,
			FOnGetMediaDelegateFast::CreateUObject(this, &UModioUISubsystem::GalleryImageDownloadHandler, ID, Index));
	}
}

void UModioUISubsystem::GalleryImageDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image,
                                                    FModioModID ID, int32 Index)
{
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to download gallery image for mod %s: \"%s\""), *ID.ToString(),
		       *ErrorCode.GetErrorMessage());
	}
	OnModGalleryImageDownloadCompleted.Broadcast(ID, ErrorCode, Index, Image);
}

void UModioUISubsystem::CreatorAvatarDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image,
                                                     FModioModID ID)
{
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to download creator avatar for mod %s: \"%s\""), *ID.ToString(),
		       *ErrorCode.GetErrorMessage());
	}
	OnModCreatorAvatarDownloadCompleted.Broadcast(ID, ErrorCode, Image);
}

void UModioUISubsystem::ModCollectionLogoDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image,
                                                         FModioModCollectionID ID, EModioLogoSize LogoSize)
{
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to download logo for mod collection %s: \"%s\""), *ID.ToString(),
		       *ErrorCode.GetErrorMessage());
	}
	OnModCollectionLogoDownloadCompleted.Broadcast(ID, ErrorCode, Image, LogoSize);
}

void UModioUISubsystem::ModCollectionCuratorAvatarDownloadHandler(FModioErrorCode ErrorCode,
                                                                  TOptional<FModioImageWrapper> Image,
                                                                  FModioModCollectionID ID)
{
	if (ErrorCode)
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to download curator avatar for mod collection %s: \"%s\""),
		       *ID.ToString(),
		       *ErrorCode.GetErrorMessage());
	}
	OnModCollectionCuratorAvatarDownloadCompleted.Broadcast(ID, ErrorCode, Image);
}

void UModioUISubsystem::OnAuthenticationComplete(FModioErrorCode ErrorCode)
{
	if (!ErrorCode)
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			TOptional<FModioUser> NewUser = Subsystem->QueryUserProfile();
			OnUserChanged.Broadcast(NewUser);
		}
	}
	else
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to authenticate user: \"%s\""), *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::ModInfoRequestCompletedHandler(FModioErrorCode ErrorCode, TOptional<FModioModInfoList> ModInfos,
                                                       TArray<FModioModID> IDs)
{
	if (ErrorCode)
	{
		// Got an error, notify anybody that the specified IDs got errors when requesting mod info
		for (FModioModID& ID : IDs)
		{
			OnModInfoRequestCompleted.Broadcast(ID, ErrorCode, {});
		}
	}
	else
	{
		for (FModioModInfo Info : ModInfos.GetValue().GetRawList())
		{
			OnModInfoRequestCompleted.Broadcast(Info.ModId, {}, Info);
		}
	}
}

void UModioUISubsystem::ModCollectionInfoRequestCompletedHandler(FModioErrorCode ErrorCode,
                                                                 TOptional<FModioModCollectionInfoList>
                                                                 ModCollectionInfos, TArray<FModioModCollectionID> IDs)
{
	if (ErrorCode)
	{
		// Got an error, notify anybody that the specified IDs got errors when requesting mod collection info
		for (FModioModCollectionID& ID : IDs)
		{
			OnModCollectionInfoRequestCompleted.Broadcast(ID, ErrorCode, {});
		}
	}
	else
	{
		for (FModioModCollectionInfo Info : ModCollectionInfos.GetValue().GetRawList())
		{
			OnModCollectionInfoRequestCompleted.Broadcast(Info.Id, {}, Info);
		}
	}
}

void UModioUISubsystem::ListAllModsCompletedHandler(FModioErrorCode ErrorCode, TOptional<FModioModInfoList> ModInfos,
                                                    FString RequestIdentifier)
{
	OnListAllModsRequestCompleted.Broadcast(RequestIdentifier, ErrorCode, ModInfos);
}

void UModioUISubsystem::ListModCollectionsCompletedHandler(FModioErrorCode ErrorCode,
                                                           TOptional<FModioModCollectionInfoList> ModCollectionInfos,
                                                           FString RequestIdentifier)
{
	OnListModCollectionsRequestCompleted.Broadcast(RequestIdentifier, ErrorCode, ModCollectionInfos);
}

void UModioUISubsystem::GetModCollectionModsCompletedHandler(FModioErrorCode ErrorCode,
                                                             TOptional<FModioModInfoList> ModInfos,
                                                             FModioModCollectionID CollectionID)
{
	OnGetModCollectionModsRequestCompleted.Broadcast(CollectionID, ErrorCode, ModInfos);
}

void UModioUISubsystem::TokenPackRequestCompletedHandler(FModioErrorCode ErrorCode,
                                                         TOptional<FModioTokenPackList> TokenPacks,
                                                         TArray<FModioTokenPackID> IDs)
{
	if (ErrorCode)
	{
		// Got an error, notify anybody that the specified IDs got errors when requesting mod info
		for (FModioTokenPackID& ID : IDs)
		{
			OnTokenPackRequestCompleted.Broadcast(ID, ErrorCode, {});
		}
	}
	else
	{
		for (FModioTokenPack Pack : TokenPacks.GetValue().GetRawList())
		{
			OnTokenPackRequestCompleted.Broadcast(Pack.GetId(), {}, Pack);
		}
	}
}

void UModioUISubsystem::ListAllTokenPacksCompletedHandler(FModioErrorCode ErrorCode,
                                                          TOptional<FModioTokenPackList> TokenPacks)
{
	OnListAllTokenPacksRequestCompleted.Broadcast(ErrorCode, TokenPacks);
}

void UModioUISubsystem::LogOut(FOnErrorOnlyDelegateFast DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		OnAuthenticationChangeStarted.Broadcast();

		Subsystem->ClearUserDataAsync(FOnErrorOnlyDelegateFast::CreateLambda(
			[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::OnLogoutComplete),
				DedicatedCallback](FModioErrorCode ec) {
				DedicatedCallback.ExecuteIfBound(ec);
				HookedHandler.ExecuteIfBound(ec);
			}));
	}
}

void UModioUISubsystem::OnLogoutComplete(FModioErrorCode ErrorCode)
{
	if (!ErrorCode)
	{
		OnUserChanged.Broadcast({});
	}
	else
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to log out: \"%s\""), *ErrorCode.GetErrorMessage());
	}
}

TOptional<FModioModTagOptions> UModioUISubsystem::GetTagOptionsList()
{
	return CachedModTags;
}

void UModioUISubsystem::GetTagOptionsListAsync()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->GetModTagOptionsAsync(
			FOnGetModTagOptionsDelegateFast::CreateUObject(this, &UModioUISubsystem::OnGetModTagOptionsComplete));
	}
}

void UModioUISubsystem::ModManagementEventHandler(FModioModManagementEvent Event)
{
	OnModManagementEvent.Broadcast(Event);
}

void UModioUISubsystem::OnGetModTagOptionsComplete(FModioErrorCode ErrorCode, TOptional<FModioModTagOptions> ModTags)
{
	if (!ErrorCode)
	{
		CachedModTags = ModTags;
	}
	else
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to get mod tag options: \"%s\""), *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::WalletBalanceRequestHandler(FModioErrorCode ErrorCode, TOptional<uint64> Balance)
{
	if (!ErrorCode)
	{
		OnGetUserWalletBalanceRequestCompleted.Broadcast(Balance.GetValue());
	}
	else
	{
		UE_LOG(ModioUICore, Error, TEXT("Failed to get user wallet balance: \"%s\""), *ErrorCode.GetErrorMessage());
	}
}

void UModioUISubsystem::SetModRatingStateDataProvider(
	TScriptInterface<IModRatingStateProvider> InModRatingStateProvider)
{
	ModRatingStateProvider = InModRatingStateProvider.GetObject();
}

void UModioUISubsystem::SetModCollectionRatingStateDataProvider(
	TScriptInterface<IModCollectionRatingStateProvider> InModCollectionRatingStateProvider)
{
	ModCollectionRatingStateProvider = InModCollectionRatingStateProvider.GetObject();
}

EModioRating UModioUISubsystem::NativeQueryModRating(int64 ModID)
{
	if (ModRatingMap.Contains(ModID))
	{
		return ModRatingMap[ModID];
	}

	return EModioRating::Neutral;
}

bool UModioUISubsystem::NativeRequestModCollectionRatingChange(int64 CollectionID, EModioRating NewRating)
{
	if (ModCollectionRatingMap.Contains(CollectionID))
	{
		ModCollectionRatingMap[CollectionID] = NewRating;
	}
	else
	{
		ModCollectionRatingMap.Add(CollectionID, NewRating);
	}

	return true;
}

EModioRating UModioUISubsystem::NativeQueryModCollectionRating(int64 ModCollectionID)
{
	if (ModCollectionRatingMap.Contains(ModCollectionID))
	{
		return ModCollectionRatingMap[ModCollectionID];
	}

	return EModioRating::Neutral;
}

EModioOpenStoreResult UModioUISubsystem::RequestShowTokenPurchaseUI()
{
	return RequestShowTokenPurchaseUIWithHandler({});
}

EModioOpenStoreResult UModioUISubsystem::RequestShowTokenPurchaseUIWithHandler(
	const FOnShowTokenPurchaseUIResult& Callback)
{
	if (!IsUGCFeatureEnabled(EModioUIFeatureFlags::Monetization))
	{
		Callback.Execute(false, "Tried to invoke store but Monetization is not enabled for the project.");
		return EModioOpenStoreResult::FailedInactive;
	}

	if (IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::GetByPlatform())
	{
		if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			EModioPortal CurrentPortal = ModioSubsystem->GetCurrentPortal();

			if (CurrentPortal == EModioPortal::XboxLive)
			{
				// Early out because Xbox doesn't support displaying the store from engine
				Callback.Execute(false, "Xbox Live does not support invoking store.");
				return EModioOpenStoreResult::FailedUnsupportedPlatform;
			}

			if (OnlineSubsystem->GetExternalUIInterface().IsValid())
			{
				// If the current platform is Steam, we have to show the Item Store via a website URL, as there is
				// no abstraction for showing the item store within the OSS.
				if (CurrentPortal == EModioPortal::Steam)
				{
					FString StoreUrl = "https://store.steampowered.com/itemstore/" +
					                   UModioSDKLibrary::GetMonetizationPurchaseCategory(CurrentPortal) +
					                   "/?beta=1";
					FShowWebUrlParams WebParams;
					FOnShowWebUrlClosedDelegate OnWebUrlClosedHandler;

					OnWebUrlClosedHandler.BindLambda([Callback](const FString& FinalUrl) {
						Callback.Execute(true, FinalUrl);
					});
					return (OnlineSubsystem->GetExternalUIInterface()->
					                         ShowWebURL(StoreUrl, WebParams, OnWebUrlClosedHandler)
						        ? EModioOpenStoreResult::Success
						        : EModioOpenStoreResult::FailedUnknown);
				}

				// If we are not Steam, use the OSS ExternalUIInterface
				FOnShowStoreUIClosedDelegate OnStoreClosedHandler;
				OnStoreClosedHandler.BindLambda(
					[this, Callback, ModioSubsystem](bool bResult) {
						// Purchase made

						if (bResult)
						{
							RequestRefreshEntitlements();
						}

						Callback.Execute(bResult, bResult ? "Successfully opened store" : "Failed to open store");
					});

				FShowStoreParams Params;
				Params.Category = UModioSDKLibrary::GetMonetizationPurchaseCategory(CurrentPortal);
				Params.bAddToCart = false;

				return (OnlineSubsystem->GetExternalUIInterface()->ShowStoreUI(0, Params, OnStoreClosedHandler)
					        ? EModioOpenStoreResult::Success
					        : EModioOpenStoreResult::FailedUnknown);
			}
			Callback.Execute(false, "Failed to get External UI Interface when invoking store.");
			return EModioOpenStoreResult::FailedUnknown;
		}
		Callback.Execute(false, "Failed to get Modio Subsystem when invoking store.");
		return EModioOpenStoreResult::FailedUnknown;
	}

	Callback.Execute(false, "Failed to get Online Subsystem when invoking store.");
	return EModioOpenStoreResult::FailedUnknown;
}

void UModioUISubsystem::RequestRefreshEntitlements()
{
	#if !WITH_EDITOR // Only do this in a build: editor context does not have Online Subsystems
	OnEntitlementRefreshEvent.Broadcast();
	#endif
}

void UModioUISubsystem::RequestFollowModCollection(FModioModCollectionID ID)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->FollowModCollectionAsync(ID,
		                                    FOnFollowModCollectionDelegateFast::CreateUObject(
			                                    this, &UModioUISubsystem::ModCollectionFollowHandler));
	}
}

void UModioUISubsystem::RequestFollowModCollectionWithHandler(FModioModCollectionID ID,
                                                              FOnFollowModCollectionDelegate Callback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->FollowModCollectionAsync(ID, FOnFollowModCollectionDelegateFast::CreateLambda(
			                                    [HookedHandler = FOnFollowModCollectionDelegateFast::CreateUObject(
				                                    this, &UModioUISubsystem::ModCollectionFollowHandler), Callback]
		                                    (FModioErrorCode ec, TOptional<FModioModCollectionInfo> CollectionInfo) {
				                                    FModioOptionalModCollectionInfo OptionalResult = {};
				                                    OptionalResult.Internal = CollectionInfo.GetValue();
				                                    Callback.ExecuteIfBound(ec, OptionalResult);
				                                    HookedHandler.ExecuteIfBound(ec, CollectionInfo);
			                                    }));
	}
}

void UModioUISubsystem::RequestUnfollowModCollection(FModioModCollectionID ID)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->UnfollowModCollectionAsync(
			ID, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::ModCollectionUnfollowHandler, ID));
	}
}

void UModioUISubsystem::RequestUnfollowModCollectionWithHandler(FModioModCollectionID ID,
                                                                FOnErrorOnlyDelegate DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->UnfollowModCollectionAsync(
			ID, FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
						this, &UModioUISubsystem::ModCollectionUnfollowHandler, ID),
					DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::RequestSubscribeToModCollection(FModioModCollectionID ID)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->SubscribeToModCollectionAsync(
			ID, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::ModCollectionSubscribeHandler, ID));
	}
}

void UModioUISubsystem::RequestSubscribeToModCollectionWithHandler(FModioModCollectionID ID,
                                                                   FOnErrorOnlyDelegate Callback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->SubscribeToModCollectionAsync(
			ID, FOnErrorOnlyDelegateFast::CreateLambda(
				[HookedHandler =
					FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::ModCollectionSubscribeHandler, ID)
					,
					Callback](FModioErrorCode ec) {
					Callback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::RequestUnsubscribeFromModCollection(FModioModCollectionID ID)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->UnsubscribeFromModCollectionAsync(
			ID, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::ModCollectionUnsubscribeHandler, ID));
	}
}

void UModioUISubsystem::RequestUnsubscribeFromModCollectionWithHandler(FModioModCollectionID ID,
                                                                       FOnErrorOnlyDelegate DedicatedCallback)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->UnsubscribeFromModCollectionAsync(
			ID, FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
						this, &UModioUISubsystem::ModCollectionUnsubscribeHandler, ID),
					DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::QueryIsUserFollowingModCollection(FModioModCollectionID ID)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->ListUserFollowedModCollectionsAsync(
			{}, FOnListFollowedModCollectionsDelegateFast::CreateLambda(
				[ID](FModioErrorCode ec, TOptional<FModioModCollectionInfoList> FollowedCollections) {
					if (ec)
					{
						UE_LOG(ModioUICore, Error, TEXT("Failed to query user following collection state: \"%s\""),
						       *ec.GetErrorMessage());
					}
				}));
	}
}

void UModioUISubsystem::QueryIsUserFollowingModCollectionWithHandler(FModioModCollectionID ID,
                                                                     FOnQueryFollowedModCollectionCompleted Handler)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->ListUserFollowedModCollectionsAsync({},
		                                               FOnListFollowedModCollectionsDelegateFast::CreateLambda(
			                                               [ID, Handler](
			                                               FModioErrorCode ec,
			                                               TOptional<FModioModCollectionInfoList> FollowedCollections) {
				                                               if (ec)
				                                               {
					                                               Handler.Execute(ec, false);
				                                               }
				                                               else
				                                               {
					                                               if (!FollowedCollections.IsSet())
					                                               {
						                                               Handler.Execute(ec, false);
					                                               }

					                                               // can probably do this better
					                                               bool bFoundCollection = false;
					                                               for (const FModioModCollectionInfo& Info :
					                                                    FollowedCollections.GetValue().GetRawList())
					                                               {
						                                               if (Info.Id == ID)
						                                               {
							                                               bFoundCollection = true;
							                                               break;
						                                               }
					                                               }
					                                               Handler.ExecuteIfBound(ec, bFoundCollection);
				                                               }
			                                               }));
	}
}

bool UModioUISubsystem::NativeRequestModRatingChange(int64 ID, EModioRating NewRating)
{
	if (ModRatingMap.Contains(ID))
	{
		ModRatingMap[ID] = NewRating;
	}
	else
	{
		ModRatingMap.Add(ID, NewRating);
	}

	return true;
}


#include "Loc/EndModioLocNamespace.h"