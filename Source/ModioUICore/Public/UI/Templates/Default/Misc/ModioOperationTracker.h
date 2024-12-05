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
#include "UI/EventHandlers/IModioUIModManagementEventReceiver.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUIImageDisplayWidget.h"
#include "UI/Interfaces/IModioUIProgressWidget.h"

#include "ModioOperationTracker.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioOperationTracker : public UModioUIComponentBase,
											   public IModioUIModManagementEventReceiver,
											   public IModioUISubscriptionsChangedReceiver
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Operation Tracker|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetPercentProgressTextWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Operation Tracker|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIProgressWidget> GetRadialProgressWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Operation Tracker|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIImageDisplayWidget> GetQueueCountIconWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Operation Tracker|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetQueueCountTextWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Operation Tracker|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetQueueLabelTextWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Operation Tracker|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIImageDisplayWidget> GetOperationTypeIconWidget() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Operation Tracker|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetDownloadSizeTextWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Operation Tracker|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetDownloadSpeedTextWidget() const;

	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;

	bool bPollInstallProgress = false;
	bool bPollSpeed = false;

	FModioUnsigned64 DownloadTotalSize;
	FModioUnsigned64 DownloadProgressSize;

	float TimeElapsed;
	FModioUnsigned64 SizePerSecond;

	void RefreshSizeAndSpeedText(FModioModProgressInfo ProgressInfo, float InDeltaTime);
	void SetDownloadSizeText();
	UPROPERTY(EditDefaultsOnly, Category = "mod.io|UI|Operation Tracker|Localization")
	FText SizeFormatText;
	UPROPERTY(EditDefaultsOnly, Category = "mod.io|UI|Operation Tracker|Localization")
	FText SpeedFormatText;

	float GetInstallProgressPercent(FModioModProgressInfo ProgressInfo);
	int32 GetNumUserSubscriptions() const;
	int32 GetNumUserInstallations() const;
	void HideSubWidgets();
	void ShowSubWidgets();
	void SetInitialVisualState();

	UFUNCTION(BlueprintNativeEvent, Category = "mod.io|UI|Operation Tracker|Events", meta = (BlueprintProtected))
	void RefreshQueueData();

	void HandleOperationBegin();
	UFUNCTION(BlueprintNativeEvent, Category = "mod.io|UI|Operation Tracker|Events", meta = (BlueprintProtected))
	void HandleOperationComplete();
	void OnAllOperationsComplete();
};
