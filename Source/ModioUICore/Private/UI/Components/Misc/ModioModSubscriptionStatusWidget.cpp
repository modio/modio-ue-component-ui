/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioModSubscriptionStatusWidget.h"

#include "UI/Interfaces/IModioModInfoUIDetails.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModSubscriptionStatusWidget)

void UModioModSubscriptionStatusWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	IModioUISubscriptionsChangedReceiver::Register<UModioModSubscriptionStatusWidget>();
}
