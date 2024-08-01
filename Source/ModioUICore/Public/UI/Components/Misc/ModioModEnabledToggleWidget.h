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

#include "ModioModEnabledToggleWidget.generated.h"

/**
 * @default_impl_for Mod Enabled Toggle Widget
 * @brief A widget that allows the user to toggle the enabled status of a mod
 * @component_display_name Mod Enabled Toggle Widget
 */
UCLASS()
class MODIOUICORE_API UModioModEnabledToggleWidget : public UModioUIComponentBase,
													 public IModioUIModEnabledStateChangedReceiver
{
	GENERATED_BODY()

protected:
	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativeSetDataSource(UObject* InDataSource) override;
	//~ End IModioUIDataSourceWidget Interface
};
