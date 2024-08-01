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
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"

#include "ModioModSubscriptionStatusWidget.generated.h"

/**
 * @default_impl_for Mod Subscription Status Widget
 * @brief A widget that displays the subscription status of a mod
 * @component_display_name Mod Subscription Status Widget
 */
UCLASS()
class MODIOUICORE_API UModioModSubscriptionStatusWidget : public UModioUIComponentBase,
														  public IModioUISubscriptionsChangedReceiver
{
	GENERATED_BODY()
protected:
	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface
};
