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
	TScriptInterface<IModioUIHasTextWidget> GetProgressTextWidget() const;
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
	TScriptInterface<IModioUIImageDisplayWidget> GetOperationTypeIconWidget() const;

	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;

	bool bPollInstallProgress = false;

	float GetInstallProgressPercent();
	int32 GetNumUserSubscriptions() const;
	int32 GetNumUserInstallations() const;
	void HideSubWidgets();
	void ShowSubWidgets();
	void SetInitialVisualState();
	void HandleOperationBegin();
	UFUNCTION(BlueprintNativeEvent, Category = "mod.io|UI|Operation Tracker|Events", meta = (BlueprintProtected))
	void HandleOperationComplete();
	void OnAllOperationsComplete();
	UFUNCTION(BlueprintNativeEvent, Category = "mod.io|UI|Operation Tracker|Events", meta = (BlueprintProtected))
	void RefreshQueueData();


};
