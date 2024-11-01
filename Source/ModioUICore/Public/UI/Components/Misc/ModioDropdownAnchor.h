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
#include "Components/MenuAnchor.h"
#include "UI/Interfaces/IModioToggleableWidget.h"
#include "ModioDropdownAnchor.generated.h"

/**
 * @default_impl_for Dropdown Anchor
 * @brief A widget that anchors a dropdown menu to a button
 * @component_display_name Dropdown Anchor
 */
UCLASS()
class MODIOUICORE_API UModioDropdownAnchor : public UMenuAnchor, public IModioToggleableWidget
{
	GENERATED_BODY()

protected:
	//~ Begin UWidget Interface
	virtual void OnWidgetRebuilt() override;
	//~ End UWidget Interface
	
	//~ Begin IModioToggleableWidget Interface
	FModioOnToggleStateChangedMulticast OnToggleStateChanged;
	virtual void Open_Implementation(bool bForce, bool bFocusMenu) override;
	virtual void Close_Implementation() override;
	virtual bool IsOpen_Implementation() const override;
	virtual void AddToggleStateChangedHandler_Implementation(const FModioOnToggleStateChanged& Handler) override;
	virtual void RemoveToggleStateChangedHandler_Implementation(const FModioOnToggleStateChanged& Handler) override;
	//~ End IModioToggleableWidget Interface

	UFUNCTION()
	virtual void ModioHandleMenuOpenChanged(bool bIsOpen);
};