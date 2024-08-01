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

#include "ModioModEnabledStatusWidget.generated.h"

/**
 * @default_impl_for Mod Enabled Status Widget
 * @brief A widget that displays the enabled status of a mod
 * @component_display_name Mod Enabled Status Widget
 */
UCLASS()
class MODIOUICORE_API UModioModEnabledStatusWidget : public UModioUIComponentBase,
													 public IModioUIModEnabledStateChangedReceiver
{
	GENERATED_BODY()

protected:
	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface
};
