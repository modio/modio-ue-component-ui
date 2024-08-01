/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "ModioUISubsystem.h"
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
				Callback.ExecuteIfBound(ec, FModioOptionalUInt64 {Balance});
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
										 Callback](FModioErrorCode ec, TOptional<FModioTransactionRecord> Transaction) {
											Callback.ExecuteIfBound(ec, FModioOptionalTransactionRecord {Transaction});
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

	return IModioUIModEnabledStateProvider::Execute_RequestModEnabledStateChange(ModEnabledStateDataProvider, ID, bNewEnabledState);
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
			default:
				return false;
		}
	}
	else
	{
		return false;
	}
}

void UModioUISubsystem::OnUGCSubsystemModEnabledChanged(int64 RawModID, bool bNewEnabledState)
{
	OnModEnabledStateChanged.Broadcast(FModioModID(RawModID), bNewEnabledState);
}

void UModioUISubsystem::SubscriptionHandler(FModioErrorCode ErrorCode, FModioModID ID)
{
	OnSubscriptionRequestCompleted.Broadcast(ErrorCode, ID);
	if (!ErrorCode)
	{
		OnSubscriptionStatusChanged.Broadcast(ID, true);
	}
}

void UModioUISubsystem::PurchaseRequestHandler(FModioErrorCode ErrorCode,
											   TOptional<FModioTransactionRecord> Transaction)
{
	OnPurchaseRequestCompleted.Broadcast(ErrorCode, Transaction);
	if (!ErrorCode)
	{
		OnGetUserWalletBalanceRequestCompleted.Broadcast(Transaction.GetValue().UpdatedUserWalletBalance.Underlying);
	}
}

void UModioUISubsystem::UnsubscribeHandler(FModioErrorCode ErrorCode, FModioModID ID)
{
	if (!ErrorCode)
	{
		OnSubscriptionStatusChanged.Broadcast(ID, false);
	}
}

void UModioUISubsystem::UninstallHandler(FModioErrorCode ErrorCode, FModioModID ID)
{
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
}

void UModioUISubsystem::SetModEnabledStateDataProvider(TScriptInterface<IModioUIModEnabledStateProvider> InModEnabledStateDataProvider)
{
	ModEnabledStateDataProvider = InModEnabledStateDataProvider.GetObject();
}

FModioErrorCode UModioUISubsystem::EnableModManagement()
{
	UModioSubsystem* DependencySubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (DependencySubsystem)
	{
		return DependencySubsystem->EnableModManagement(
			FOnModManagementDelegateFast::CreateUObject(this, &UModioUISubsystem::ModManagementEventHandler));
	}
	return FModioErrorCode::SystemError();
}

void UModioUISubsystem::DisableModManagement()
{
	UModioSubsystem* DependencySubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (DependencySubsystem)
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
		Subsystem->ForceUninstallModAsync(
			ID, FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
															this, &UModioUISubsystem::UninstallHandler, ID),
														DedicatedCallback](FModioErrorCode ec) {
				DedicatedCallback.ExecuteIfBound(ec);
				HookedHandler.ExecuteIfBound(ec);
			}));
	}
}

void UModioUISubsystem::RequestRateUpForModId(FModioModID ModId, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		// Needs additional payload param so we know which type of operation was completed
		Subsystem->SubmitModRatingAsync(
			ModId, EModioRating::Positive,
			FOnErrorOnlyDelegateFast::CreateLambda(
				[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
					 this, &UModioUISubsystem::OnRatingSubmissionComplete, EModioRating::Positive),
				 DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::RequestRateDownForModId(FModioModID ModId, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		// Needs additional payload param so we know which type of operation was completed
		Subsystem->SubmitModRatingAsync(
			ModId, EModioRating::Negative,
			FOnErrorOnlyDelegateFast::CreateLambda(
				[HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
					 this, &UModioUISubsystem::OnRatingSubmissionComplete, EModioRating::Negative),
				 DedicatedCallback](FModioErrorCode ec) {
					DedicatedCallback.ExecuteIfBound(ec);
					HookedHandler.ExecuteIfBound(ec);
				}));
	}
}

void UModioUISubsystem::OnRatingSubmissionComplete(FModioErrorCode ErrorCode, EModioRating Rating) {}

void UModioUISubsystem::OnExternalUpdatesFetched(FModioErrorCode ErrorCode) {}

void UModioUISubsystem::RequestRemoveSubscriptionForModID(FModioModID ID)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
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
	else
	{
		return 1.0f;
	}
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

void UModioUISubsystem::LogoDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image,
											FModioModID ID, EModioLogoSize LogoSize)
{
	OnModLogoDownloadCompleted.Broadcast(ID, ErrorCode, Image, LogoSize);
}

void UModioUISubsystem::UserAvatarDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image)
{
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

void UModioUISubsystem::RequestEmailAuthentication(FModioEmailAuthCode Code)
{
	OnAuthenticationChangeStarted.Broadcast();

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->AuthenticateUserEmailAsync(
			Code, FOnErrorOnlyDelegateFast::CreateUObject(this, &UModioUISubsystem::OnAuthenticationComplete));
	}
}

void UModioUISubsystem::RequestEmailAuthentication(FModioEmailAuthCode Code, FOnErrorOnlyDelegateFast DedicatedCallback)
{
	OnAuthenticationChangeStarted.Broadcast();

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		Subsystem->AuthenticateUserEmailAsync(
			Code, FOnErrorOnlyDelegateFast::CreateLambda([HookedHandler = FOnErrorOnlyDelegateFast::CreateUObject(
															  this, &UModioUISubsystem::OnAuthenticationComplete),
														  DedicatedCallback](FModioErrorCode ec) {
				DedicatedCallback.ExecuteIfBound(ec);
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
	OnModGalleryImageDownloadCompleted.Broadcast(ID, ErrorCode, Index, Image);
}

void UModioUISubsystem::CreatorAvatarDownloadHandler(FModioErrorCode ErrorCode, TOptional<FModioImageWrapper> Image,
													 FModioModID ID)
{
	OnModCreatorAvatarDownloadCompleted.Broadcast(ID, ErrorCode, Image);
}

void UModioUISubsystem::OnAuthenticationComplete(FModioErrorCode ec)
{
	if (!ec)
	{
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			TOptional<FModioUser> NewUser = Subsystem->QueryUserProfile();
			OnUserChanged.Broadcast(NewUser);
		}
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

void UModioUISubsystem::ListAllModsCompletedHandler(FModioErrorCode ErrorCode, TOptional<FModioModInfoList> ModInfos,
													FString RequestIdentifier)
{
	OnListAllModsRequestCompleted.Broadcast(RequestIdentifier, ErrorCode, ModInfos);
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

void UModioUISubsystem::OnLogoutComplete(FModioErrorCode ec)
{
	if (!ec)
	{
		OnUserChanged.Broadcast({});
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
}

void UModioUISubsystem::WalletBalanceRequestHandler(FModioErrorCode ErrorCode, TOptional<uint64> Balance)
{
	if (!ErrorCode)
	{
		OnGetUserWalletBalanceRequestCompleted.Broadcast(Balance.GetValue());
	}
}

void UModioUISubsystem::SetModRatingStateDataProvider(TScriptInterface<IModRatingStateProvider> InModRatingStateProvider)
{
	ModRatingStateProvider = InModRatingStateProvider.GetObject();
}

EModioRating UModioUISubsystem::NativeQueryModRating(int64 ModID)
{
	if (ModRatingMap.Contains(ModID))
	{
		return ModRatingMap[ModID];
	}

	return EModioRating::Neutral;
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
