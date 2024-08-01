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
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/EventHandlers/IModioUIModEnabledStateChangedReceiver.h"
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"

#include "ModioModInstallationStatusWidget.generated.h"

UENUM(BlueprintType)
enum class EModioModInstallationStatus : uint8
{
	// Not installed by any user on the current system
	NotInstalled,
	// Mod has encountered an error during processing
	Error,
	// Installed and enabled by the current user
	Enabled,
	// Installed but disabled by the current user
	Disabled,
	// Purchased by the current user, but not installed by any user on the current system
	Purchased,
	// Installed by another user on the current system. This status takes precedence over Purchased.
	SystemInstall,
	// Mod is queued for processing
	Queued,
	// Beginning a new mod processing state
	Initializing,
	// Mod is currently downloading
	Downloading,
	// Mod is currently extracting
	Extracting,
	// Mod is currently uploading
	Uploading,
	// Mod upload has completed
	Uploaded
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnModInstallationStatusChanged, EModioModInstallationStatus, NewStatus);

/**
 * @default_impl_for Mod Installation Status Widget
 * @brief A widget that displays the installation status of a mod
 * @component_display_name Mod Installation Status Widget
 */
UCLASS()
class MODIOUICORE_API UModioModInstallationStatusWidget : public UModioUIComponentBase,
														  public IModioUIModManagementEventReceiver,
														  public IModioUISubscriptionsChangedReceiver,
														  public IModioUIModEnabledStateChangedReceiver

{
	GENERATED_BODY()

protected:
	/**
	 * @brief Full information for the mod this widget represents, based on its data source
	 */
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|InstallationStatus|Data Binding", meta = (BlueprintProtected))
	FModioModInfo BoundModInfo;

	/**
	 * @brief Broadcasts when the mod's installation status is updated
	 */
	UPROPERTY(BlueprintAssignable, Category = "mod.io|UI|InstallationStatus|Events", meta = (BlueprintProtected))
	FOnModInstallationStatusChanged OnInstallationStatusChanged;

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|InstallationStatus|Data Binding", meta = (BlueprintProtected))
	EModioModInstallationStatus CurrentInstallationStatus = EModioModInstallationStatus::NotInstalled;

	void SetModInstallationStatus(EModioModInstallationStatus NewStatus);

	bool bEnableModProgressPolling = false;
	void SetPollModProgressInfo(bool bEnablePolling);

	/**
	 * @brief Helper function to query the progress of a state-specific operation for the bound mod
	 * @param ProgressState The state for which to query progress
	 * @return A float between 0 and 1 measuring progress, where 0 represents none and 1 is complete.  Returns 0 if the
	 * specified state is invalid or not in progress for this mod
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|InstallationStatus|Data Binding", meta = (BlueprintProtected))
	float QueryProgressValueForState(EModioModProgressState ProgressState) const;

	void UpdateInternalStateFromModManagementEvent(FModioModManagementEvent Event);
	void UpdateInternalStateFromModInfo(FModioModInfo RelevantModInfo);
	void UpdateInternalStateFromModProgressInfo(FModioModProgressInfo CurrentProgressInfo);
	void UpdateInternalStateBasedOnModEnabledState(FModioModID ModId);
	static bool IsModPurchased(const FModioModID& ModId);

	//~ Begin UUserWidget Interface
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativeSetDataSource(UObject* InDataSource) override;
	//~ End IModioUIDataSourceWidget Interface

	//~ Begin IModioUIModManagementEventReceiver Interface
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
	//~ End IModioUIModManagementEventReceiver Interface

	//~ Begin IModioUISubscriptionsChangedReceiver Interface
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;
	//~ End IModioUISubscriptionsChangedReceiver Interface

	//~ Begin IModioUIModEnabledStateChangedReceiver Interface
	virtual void NativeModEnabledStateChanged(FModioModID ModID, bool bNewEnabledState) override;
	//~ End IModioUIModEnabledStateChangedReceiver Interface
};
