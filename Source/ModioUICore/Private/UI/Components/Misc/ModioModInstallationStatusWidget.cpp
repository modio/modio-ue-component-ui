/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioModInstallationStatusWidget.h"

#include "ModioSubsystem.h"
#include "ModioUISubsystem.h"
#include "UI/Components/ComponentHelpers.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/ModioUICommonFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModInstallationStatusWidget)

void UModioModInstallationStatusWidget::SetModInstallationStatus(EModioModInstallationStatus NewStatus)
{
	CurrentInstallationStatus = NewStatus;
	OnInstallationStatusChanged.Broadcast(CurrentInstallationStatus);
}

void UModioModInstallationStatusWidget::SetPollModProgressInfo(bool bEnablePolling)
{
	bEnableModProgressPolling = bEnablePolling;
}

void UModioModInstallationStatusWidget::UpdateInternalStateFromModManagementEvent(FModioModManagementEvent Event)
{
	if (Event.Status)
	{
		SetModInstallationStatus(EModioModInstallationStatus::Error);
		return;
	}
	switch (Event.Event)
	{
		case EModioModManagementEventType::BeginInstall:
		case EModioModManagementEventType::BeginUninstall:
		case EModioModManagementEventType::BeginUpdate:
		case EModioModManagementEventType::BeginUpload:
		{
			SetModInstallationStatus(EModioModInstallationStatus::Initializing);
			SetPollModProgressInfo(true);
		}
		break;
		case EModioModManagementEventType::Updated:
		case EModioModManagementEventType::Installed:
		{
			UpdateInternalStateBasedOnModEnabledState(Event.ID);
			SetPollModProgressInfo(false);
		}
		break;
		case EModioModManagementEventType::Uninstalled:
		{
			if (IsModPurchased(Event.ID))
			{
				SetModInstallationStatus(EModioModInstallationStatus::Purchased);
			}
			else
			{
				SetModInstallationStatus(EModioModInstallationStatus::NotInstalled);
			}
			SetPollModProgressInfo(false);
		}
		break;
		case EModioModManagementEventType::Uploaded:
		{
			SetModInstallationStatus(EModioModInstallationStatus::Uploaded);
			SetPollModProgressInfo(false);
		}
	}
}

void UModioModInstallationStatusWidget::UpdateInternalStateFromModInfo(FModioModInfo RelevantModInfo)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		// Is the mod part of the user's subscriptions?
		if (Subsystem->QueryUserSubscriptions().Contains(RelevantModInfo.ModId))
		{
			// If so, is the mod already installed?
			if (Subsystem->QueryUserInstallations(true).Contains(RelevantModInfo.ModId))
			{
				// If so, display feedback based on whether the mod is enabled or not
				UpdateInternalStateBasedOnModEnabledState(RelevantModInfo.ModId);
				return;
			}
			// If subscribed but not installed, is the mod currently being installed?
			if (TOptional<FModioModProgressInfo> ProgressInfo = Subsystem->QueryCurrentModUpdate();
				ProgressInfo.IsSet() && ProgressInfo->ID == RelevantModInfo.ModId)
			{
				// If so, enable polling for progress info and perform an initial update
				SetPollModProgressInfo(true);
				UpdateInternalStateFromModProgressInfo(ProgressInfo.GetValue());
				return;
			}
			// If it's part of subscriptions but not installed and not currently installing, indicate it is pending
			SetModInstallationStatus(EModioModInstallationStatus::Queued);
			return;
		}
		// Is it installed by another user on the system?
		if (Subsystem->QuerySystemInstallations().Contains(RelevantModInfo.ModId))
		{
			SetModInstallationStatus(EModioModInstallationStatus::SystemInstall);
			return;
		}
		// Is it a premium mod that the current user has purchased?
		if (RelevantModInfo.Price.Underlying > 0 && IsModPurchased(RelevantModInfo.ModId))
		{
			SetModInstallationStatus(EModioModInstallationStatus::Purchased);
			return;
		}
		// Not part of user subscriptions or purchases, not installed by another user, so hide everything
		SetModInstallationStatus(EModioModInstallationStatus::NotInstalled);
	}
}

void UModioModInstallationStatusWidget::UpdateInternalStateFromModProgressInfo(
	FModioModProgressInfo CurrentProgressInfo)
{
	switch (CurrentProgressInfo.GetCurrentState())
	{
		case EModioModProgressState::Initializing:
		{
			SetModInstallationStatus(EModioModInstallationStatus::Initializing);
		}
		break;
		case EModioModProgressState::Downloading:
		{
			SetModInstallationStatus(EModioModInstallationStatus::Downloading);
		}
		break;
		case EModioModProgressState::Extracting:
		{
			SetModInstallationStatus(EModioModInstallationStatus::Extracting);
		}
		break;
		case EModioModProgressState::Uploading:
		{
			SetModInstallationStatus(EModioModInstallationStatus::Uploading);
		}
	}
}

void UModioModInstallationStatusWidget::UpdateInternalStateBasedOnModEnabledState(FModioModID ModId)
{
	if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UISubsystem->QueryIsModEnabled(ModId))
		{
			SetModInstallationStatus(EModioModInstallationStatus::Enabled);
		}
		else
		{
			SetModInstallationStatus(EModioModInstallationStatus::Disabled);
		}
	}
}

void UModioModInstallationStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bEnableModProgressPolling)
	{
		return;
	}
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (TOptional<FModioModProgressInfo> ProgressInfo = Subsystem->QueryCurrentModUpdate())
		{
			if (ProgressInfo->ID == BoundModInfo.ModId)
			{
				UpdateInternalStateFromModProgressInfo(ProgressInfo.GetValue());
			}
		}
	}
}

void UModioModInstallationStatusWidget::NativePreConstruct()
{
	IModioUIModManagementEventReceiver::Register<UModioModInstallationStatusWidget>();
	IModioUISubscriptionsChangedReceiver::Register<UModioModInstallationStatusWidget>();
	IModioUIModEnabledStateChangedReceiver::Register<UModioModInstallationStatusWidget>();
}

void UModioModInstallationStatusWidget::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	if (DataSource)
	{
		BoundModInfo = IModioModInfoUIDetails::Execute_GetFullModInfo(DataSource);
		UpdateInternalStateFromModInfo(BoundModInfo);
	}
}

void UModioModInstallationStatusWidget::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);
	if (Event.ID == BoundModInfo.ModId)
	{
		UpdateInternalStateFromModManagementEvent(Event);
	}
}

void UModioModInstallationStatusWidget::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
	if (ModID == BoundModInfo.ModId)
	{
		UpdateInternalStateFromModInfo(BoundModInfo);
	}
}

void UModioModInstallationStatusWidget::NativeModEnabledStateChanged(FModioModID ModID, bool bNewEnabledState)
{
	IModioUIModEnabledStateChangedReceiver::NativeModEnabledStateChanged(ModID, bNewEnabledState);
	if (ModID == BoundModInfo.ModId)
	{
		UpdateInternalStateFromModInfo(BoundModInfo);
	}
}

bool UModioModInstallationStatusWidget::IsModPurchased(const FModioModID& ModId)
{
	if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (!UISubsystem->IsUGCFeatureEnabled(EModioUIFeatureFlags::Monetization))
		{
			return false;
		}
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			// This assumes the host application has an up-to-date cache of purchased mods, initially populated by
			// calling Subsystem->FetchUserPurchasesAsync()
			return Subsystem->QueryUserPurchasedMods().Contains(ModId);
		}
	}
	return false;
}
float UModioModInstallationStatusWidget::QueryProgressValueForState(EModioModProgressState ProgressState) const
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (TOptional<FModioModProgressInfo> ProgressInfo = Subsystem->QueryCurrentModUpdate())
		{
			if (ProgressInfo->ID == BoundModInfo.ModId)
			{
				return UModioUnsigned64Library::DivideToFloat(ProgressInfo.GetValue().GetCurrentProgress(ProgressState),
															  ProgressInfo.GetValue().GetTotalProgress(ProgressState));
			}
		}
	}
	return 0;
}
