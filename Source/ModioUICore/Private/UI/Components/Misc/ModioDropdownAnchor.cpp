/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioDropdownAnchor.h"

void UModioDropdownAnchor::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	OnMenuOpenChanged.AddUniqueDynamic(this, &UModioDropdownAnchor::ModioHandleMenuOpenChanged);
}

void UModioDropdownAnchor::Open_Implementation(bool bForce, bool bFocusMenu)
{
	if (bForce || ShouldOpenDueToClick())
	{
		Cast<UMenuAnchor>(this)->Open(bFocusMenu);
	}
}

void UModioDropdownAnchor::Close_Implementation()
{
	Cast<UMenuAnchor>(this)->Close();
}

bool UModioDropdownAnchor::IsOpen_Implementation() const
{
	return Cast<UMenuAnchor>(this)->IsOpen();
}

void UModioDropdownAnchor::AddToggleStateChangedHandler_Implementation(const FModioOnToggleStateChanged& Handler)
{
	OnToggleStateChanged.AddUnique(Handler);
}

void UModioDropdownAnchor::RemoveToggleStateChangedHandler_Implementation(const FModioOnToggleStateChanged& Handler)
{
	OnToggleStateChanged.Remove(Handler);
}

void UModioDropdownAnchor::ModioHandleMenuOpenChanged(bool bIsOpen)
{
	OnToggleStateChanged.Broadcast(this, bIsOpen);
}