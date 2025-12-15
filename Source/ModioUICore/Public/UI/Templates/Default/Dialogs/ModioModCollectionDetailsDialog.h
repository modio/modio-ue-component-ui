/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
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
#include "UI/EventHandlers/IModioUIMediaDownloadCompletedReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/EventHandlers/IModioUIWalletBalanceUpdatedEventReceiver.h"
#include "UI/EventHandlers/IModioUIModCollectionInfoReceiver.h"
#include "UI/Interfaces/IModioUIImageDisplayWidget.h"
#include "UI/Interfaces/IModioUIModListViewInterface.h"
#include "ModioModCollectionDetailsDialog.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUICORE_API UModioModCollectionDetailsDialog
	: public UModioUIComponentBase,
	  public IModioUISubscriptionsChangedReceiver,
	  public IModioUIMediaDownloadCompletedReceiver,
	  public IModioUIWalletBalanceUpdatedEventReceiver,
	  public IModioUIModCollectionInfoReceiver
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUIImageDisplayWidget> GetModCollectionLogoWidget() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUIDataSourceWidget> GetModCollectionPropertiesWidget() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUIDataSourceWidget> GetModCollectionStatusWidget() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUIDataSourceWidget> GetTagDisplayWidget() const;

	// Buttons
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetExpandDescriptionButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetFollowButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetRateUpButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetRateDownButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUICommandMenu> GetMoreOptionsMenuWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetCloseDialogButtonWidget() const;

	// Text labels
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetModCollectionNameTextWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUIDataSourceWidget> GetModCollectionCuratorDisplayWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
		Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetModCollectionSummaryShortTextWidget() const;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Mod Collection Details Dialog|Localization",
		meta = (BlueprintProtected))
	FText ExpandLabelText;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
			  Category = "mod.io|UI|Mod Collection Details Dialog|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIModListViewInterface> GetCollectionModsListViewWidget() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Collection Details Dialog",
		meta = (BlueprintProtected))
	void PopulateExtraOptionsCommands(TArray<UObject*>& Commands, UObject* Context);

	virtual void NativeSetDataSource(UObject* InDataSource) override;
	virtual void NativePreConstruct() override;

	// Interface functions
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;

	#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Mod Collection Details Dialog|Data Binding",
		meta = (BlueprintProtected))
	TObjectPtr<UObject> EditorDataSource;
	#endif

	/// @TODO we should probably not be using this at all, and make everything event-driven off of the change in the
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|Mod Collection Details Dialog", meta = (BlueprintProtected))
	FModioModCollectionInfo BoundModCollectionInfo;
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|Mod Collection Details Dialog", meta = (BlueprintProtected))
	bool bHasFullDescription;

	virtual void NativeOnModCollectionLogoDownloadCompleted(FModioModCollectionID ModCollectionID,
															FModioErrorCode ErrorCode,
															TOptional<FModioImageWrapper> Image,
															EModioLogoSize LogoSize) override;

	virtual void NativeOnModCollectionModsRequestCompleted(FModioModCollectionID CollectionID,
														   FModioErrorCode ErrorCode,
														   TOptional<FModioModInfoList> Mods) override;
};