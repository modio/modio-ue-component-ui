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
#include "UI/EventHandlers/IModioUIMediaDownloadCompletedReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/EventHandlers/IModioUIWalletBalanceUpdatedEventReceiver.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUICommandMenu.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"

#include "ModioModDetailsDialog.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioModDetailsDialog : public UModioUIComponentBase,
											   public IModioUISubscriptionsChangedReceiver,
											   public IModioUIMediaDownloadCompletedReceiver,
											   public IModioUIWalletBalanceUpdatedEventReceiver
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIMediaDownloadCompletedReceiver> GetModImageGalleryWidget() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIDataSourceWidget> GetModPropertiesWidget() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIDataSourceWidget> GetModStatusWidget() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIDataSourceWidget> GetTagDisplayWidget() const;

	// Buttons
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetExpandDescriptionButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetSubscribeButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetPurchaseButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetRateUpButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetRateDownButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUICommandMenu> GetMoreOptionsMenuWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetModDependenciesButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetCloseDialogButtonWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetWalletButtonWidget() const;

	// Text labels
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetModNameTextWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIDataSourceWidget> GetModAuthorDisplayWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetModSummaryShortTextWidget() const;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Mod Details Dialog|Localization",
			  meta = (BlueprintProtected))
	FText ExpandLabelText;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Details Dialog",
			  meta = (BlueprintProtected))
	void PopulateExtraOptionsCommands(TArray<UObject*>& Commands, UObject* Context);

	virtual void NativeSetDataSource(UObject* InDataSource) override;
	virtual void NativePreConstruct() override;

	// Interface functions
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Mod Details Dialog|Data Binding",
			  meta = (BlueprintProtected))
	TObjectPtr<UObject> EditorDataSource;
#endif

	/// @TODO we should probably not be using this at all, and make everything event-driven off of the change in the
	/// user's subscriptions
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Details Dialog", meta = (BlueprintProtected))
	bool IsUserSubscribed();
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Details Dialog", meta = (BlueprintProtected))
	bool IsModPurchased();
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|Mod Details Dialog", meta = (BlueprintProtected))
	FModioModInfo BoundModInfo;
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|Mod Details Dialog", meta = (BlueprintProtected))
	bool bHasFullDescription;
};
