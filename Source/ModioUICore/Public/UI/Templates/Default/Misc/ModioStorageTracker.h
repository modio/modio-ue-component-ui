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
#include "UI/Interfaces/IModioUIHasTextWidget.h"

#include "ModioStorageTracker.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioStorageTracker : public UModioUIComponentBase, public IModioUIModManagementEventReceiver
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Storage Tracker|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetStorageUsedTextWidget() const;

	virtual void NativePreConstruct() override;
	virtual void NativeOnModManagementEvent(FModioModManagementEvent Event) override;

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Storage Tracker|Data", meta = (BlueprintProtected))
	void RefreshData();
};
