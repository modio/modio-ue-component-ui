/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/TokenPackTile/ModioDefaultTokenPackTile.h"

#include "Engine/GameInstance.h"
#include "Input/CommonUIActionRouterBase.h"
#include "UI/Interfaces/IModioUICommandMenu.h"
#include "UI/Interfaces/ModioTokenPackUIDetails.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioDefaultTokenPackTile)

void UModioDefaultTokenPackTile::NotifyClicked()
{
	if (bEmitClickEvents)
	{
		OnClicked.Broadcast(this);
	}
}

bool UModioDefaultTokenPackTile::GetSelectedStateEx()
{
	if (!IUserObjectListEntry::GetListItem())
	{
		return false;
	}
	if (IUserListEntry::GetOwningListView() != nullptr)
	{
		return IUserListEntry::IsListItemSelected();
	}
	else
	{
		return bSelected;
	}
}

void UModioDefaultTokenPackTile::NotifyHoverStateChanged(bool bNewHoveredState)
{
	if (bEmitHoverEvents)
	{
		OnHoverStateChanged.Broadcast(this, bNewHoveredState);
	}
}

void UModioDefaultTokenPackTile::NativeOnItemSelectionChanged(bool bIsSelected)
{
	if (bSelectable)
	{
		// Calls up into BP
		IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

		bSelected = bIsSelected;
		// Emits our external selection event
		OnSelected.Broadcast(this, bIsSelected);

		// During the selection change, some Common UI-specific actions may need to be updated
		// This doesn't lead to performance issues, as it's only called when the selection state changes
		if (const UGameInstance* GameInstance = GetGameInstance())
		{
			for (const ULocalPlayer* LocalPlayer : GameInstance->GetLocalPlayers())
			{
				if (const UCommonUIActionRouterBase* ActionRouter =
						ULocalPlayer::GetSubsystem<UCommonUIActionRouterBase>(LocalPlayer))
				{
					ActionRouter->OnBoundActionsUpdated().Broadcast();
				}
			}
		}
	}
}

void UModioDefaultTokenPackTile::AddSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	OnSelected.AddUnique(Handler);
}

void UModioDefaultTokenPackTile::RemoveSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	OnSelected.Remove(Handler);
}

void UModioDefaultTokenPackTile::SetSelectable_Implementation(bool bNewSelectableState)
{
	bSelectable = bNewSelectableState;
}

void UModioDefaultTokenPackTile::GetSelectable_Implementation(bool& bIsSelectable)
{
	bIsSelectable = bSelectable;
}

void UModioDefaultTokenPackTile::SetSelectedState_Implementation(bool bNewSelectedState)
{
	if (bSelectable)
	{
		bool bCurrentSelectedState = GetSelectedStateEx();
		if (bCurrentSelectedState != bNewSelectedState)
		{
			bSelected = bNewSelectedState;
			NativeOnItemSelectionChanged(bNewSelectedState);
		}
	}
}

void UModioDefaultTokenPackTile::ToggleSelectedState_Implementation()
{
	if (bSelectable)
	{
		bool bCurrentSelectionState;
		IModioUISelectableWidget::Execute_GetSelectedState(this, bCurrentSelectionState);
		IModioUISelectableWidget::Execute_SetSelectedState(this, !bCurrentSelectionState);
	}
}

void UModioDefaultTokenPackTile::GetSelectedState_Implementation(bool& bCurrentSelectedState)
{
	if (bSelectable)
	{
		bCurrentSelectedState = GetSelectedStateEx();
	}
	else
	{
		bCurrentSelectedState = false;
	}
}

void UModioDefaultTokenPackTile::SetToggleable_Implementation(bool bNewToggleableState)
{
	bToggleable = bNewToggleableState;
}

void UModioDefaultTokenPackTile::GetToggleable_Implementation(bool& bIsToggleable)
{
	bIsToggleable = bToggleable;
}

void UModioDefaultTokenPackTile::AddClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnClicked.AddUnique(Handler);
}

void UModioDefaultTokenPackTile::RemoveClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnClicked.Remove(Handler);
}

void UModioDefaultTokenPackTile::EnableClick_Implementation()
{
	bEmitClickEvents = true;
}

void UModioDefaultTokenPackTile::DisableClick_Implementation()
{
	bEmitClickEvents = false;
}

void UModioDefaultTokenPackTile::AddHoverStateChangedHandler_Implementation(
	const FModioHoverableOnHoverStateChanged& Handler)
{
	OnHoverStateChanged.AddUnique(Handler);
}

void UModioDefaultTokenPackTile::RemoveHoverStateChangedHandler_Implementation(
	const FModioHoverableOnHoverStateChanged& Handler)
{
	OnHoverStateChanged.Remove(Handler);
}

void UModioDefaultTokenPackTile::EnableHoverEvents_Implementation()
{
	bEmitHoverEvents = true;
}

void UModioDefaultTokenPackTile::DisableHoverEvents_Implementation()
{
	bEmitHoverEvents = false;
}

void UModioDefaultTokenPackTile::NativePreConstruct()
{
	Super::NativePreConstruct();

#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		NativeOnListItemObjectSet(EditorDataSource);
	}
#endif
}

void UModioDefaultTokenPackTile::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	DataSource = InDataSource;
}

bool UModioDefaultTokenPackTile::IsListItemSelectable() const
{
	return bSelectable;
}
