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
#include "UI/EventHandlers/IModioUIDialogDisplayEventReceiver.h"
#include "UI/EventHandlers/IModioUIModInfoReceiver.h"
#include "UI/EventHandlers/IModioUIWalletBalanceUpdatedEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUIModListViewInterface.h"
#include "UI/Interfaces/IModioUIModTagSelector.h"
#include "UI/Interfaces/IModioUIObjectSelector.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"

#include "ModioModBrowser.generated.h"

UENUM(BlueprintType)
enum class EModioModBrowserState : uint8
{
	ModsView,
	LibraryView
};

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioModBrowser : public UModioUIComponentBase,
										 public IModioUIModInfoReceiver,
										 public IModioUIDialogDisplayEventReceiver,
										 public IModioUIModTagSelector,
										 public IModioUIWalletBalanceUpdatedEventReceiver,
										 public IModioUISubscriptionsChangedReceiver
{
	GENERATED_BODY()

protected:
	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

	//~ Begin IModioUIModInfoReceiver Interface
	virtual void NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ErrorCode,
	                                                 TOptional<FModioModInfoList> List) override;
	//~ End IModioUIModInfoReceiver Interface

	//~ Begin IModioUIDialogDisplayEventReceiver Interface
	virtual void NativeOnDialogDisplayEvent(EModioUIDialogType DialogRequested, UObject* InDataSource) override;
	//~ End IModioUIDialogDisplayEventReceiver Interface

	//~ Begin IModioUIWalletBalanceUpdatedEventReceiver Interface
	virtual void NativeOnWalletBalanceUpdated(uint64 NewBalance) override;
	//~ End IModioUIWalletBalanceUpdatedEventReceiver Interface

	//~ Begin IModioUISubscriptionsChangedReceiver Interface
	void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState);
	//~ End IModioUISubscriptionsChangedReceiver Interface

	// Toggle between Mods / Library View
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetModsViewButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetLibraryViewButtonWidget() const;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Mod Browser|State", meta = (BlueprintProtected))
	EModioModBrowserState CurrentView = EModioModBrowserState::ModsView;

	// Select preset filtered searches
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIObjectSelector> GetPresetFilterSelectorWidget() const;

	// Tab left and right across the preset filter buttons
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetPresetFilterTabLeftButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetPresetFilterTabRightButtonWidget() const;
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Browser|Search and Filters", meta = (BlueprintProtected))
	void DecrementPresetFilterSelection();
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Browser|Search and Filters", meta = (BlueprintProtected))
	void IncrementPresetFilterSelection();

	// Filter Library View by Subscriptions or Purchases
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetSubscriptionsFilterButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetPurchasesFilterButtonWidget() const;

	// Expand search box and execute searching by string
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetSearchButtonWidget() const;
	// String search input
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIStringInputWidget> GetSearchEditableTextBoxWidget() const;
	// Displayed after a string search has completed
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetSearchTextWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetClearSearchButtonWidget() const;
	UPROPERTY(BlueprintReadWrite, Category = "mod.io|UI|Mod Browser|Search and Filters", Transient, meta = (BlueprintProtected))
	FString StoredSearchString;

	// Display panel with tag categories to filter on
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetFilterButtonWidget() const;
	// Store filter tags between pushing and popping of filter widget
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|Mod Browser|Search and Filters", Transient, meta = (BlueprintProtected))
	TObjectPtr<UObject> StoredTagData;
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Browser|Search and Filters", meta = (BlueprintProtected))
	void InitializeTagData(UObject* InTagData);

	// Searching in Library View
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Browser|Search and Filters", meta = (BlueprintProtected))
	TArray<FModioModInfo> SearchLibraryWithStoredParams() const;
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Browser|Search and Filters", meta = (BlueprintProtected))
	TArray<FModioModInfo> SearchSubscriptionsAndInstallsWithStoredParams() const;
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Browser|Search and Filters", meta = (BlueprintProtected))
	TArray<FModioModInfo> SearchPurchasesWithStoredParams() const;
	TArray<FModioModInfo> FilterModArrayBySelectedTags(const TArray<FModioModInfo>& ModArray) const;
	static bool ModIncludesTag(const FModioModInfo& Mod, const FString& Tag);
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Browser|Search and Filters", meta = (BlueprintProtected))
	static bool DoFilterParamsIncludeUserId(const FModioFilterParams& Params);

	// A placeholder button for future monetization features. Currently just displays user balance.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetWalletButtonWidget() const;

	// Grid of ModTiles. Updates based on search and filters
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIModListViewInterface> GetModTileViewWidget() const;

	// Close mod browser
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetCloseBrowserButtonWidget() const;
};
