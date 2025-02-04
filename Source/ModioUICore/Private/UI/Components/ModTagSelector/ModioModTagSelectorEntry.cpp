/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/ModTagSelector/ModioModTagSelectorEntry.h"

#include "UI/Interfaces//IModioModTagUIDetails.h"

void UModioModTagSelectorEntry::NotifySelectionChanged()
{
	if (bAllowSelection)
	{
		OnSelectionStateChanged.Broadcast(this, bCurrentSelectionState);
	}
}

void UModioModTagSelectorEntry::NotifyClicked()
{
	if (bAllowClick)
	{
		OnClicked.Broadcast(this);
	}
}

void UModioModTagSelectorEntry::NativeConstruct()
{
	Super::Construct();

	if (GetClickableWidget().GetObject())
	{
		FModioClickableOnClicked OnClickedDelegate;
		OnClickedDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UModioModTagSelectorEntry, OnTagEntryClicked));
		IModioUIClickableWidget::Execute_AddClickedHandler(GetClickableWidget().GetObject(), OnClickedDelegate);
	}
}

void UModioModTagSelectorEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (ListItemObject && ListItemObject->Implements<UModioModTagUIDetails>())
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(this,
													 IModioModTagUIDetails::Execute_GetLocalizedText(ListItemObject));
		IModioUISelectableWidget::Execute_SetSelectedState(
			this, IModioModTagUIDetails::Execute_GetSelectionState(ListItemObject));
	}
}

void UModioModTagSelectorEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);
}

bool UModioModTagSelectorEntry::IsListItemSelectable() const
{
	return bAllowSelection;
}

void UModioModTagSelectorEntry::NativeOnEntryReleased()
{
	IUserListEntry::NativeOnEntryReleased();
	IModioUISelectableWidget::Execute_SetSelectedState(this, false);
}

void UModioModTagSelectorEntry::DisableClick_Implementation()
{
	bAllowClick = false;
}

void UModioModTagSelectorEntry::SetWidgetText_Implementation(const FText& DisplayText)
{
	if (GetTagLabelWidget().GetObject())
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(GetTagLabelWidget().GetObject(), DisplayText);
	}
}

FText UModioModTagSelectorEntry::GetWidgetText_Implementation()
{
	return GetTagLabelWidget()->GetWidgetText();
}

void UModioModTagSelectorEntry::OnTagEntryClicked(UObject* ClickedObject)
{
	IModioUISelectableWidget::Execute_SetSelectedState(this, !bCurrentSelectionState);
	NotifyClicked();
}

TScriptInterface<IModioUISelectableWidget> UModioModTagSelectorEntry::GetSelectionWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIClickableWidget> UModioModTagSelectorEntry::GetClickableWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIHasTextWidget> UModioModTagSelectorEntry::GetTagLabelWidget_Implementation() const
{
	return nullptr;
}

void UModioModTagSelectorEntry::EnableClick_Implementation()
{
	bAllowClick = true;
}

void UModioModTagSelectorEntry::RemoveClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnClicked.Remove(Handler);
}

void UModioModTagSelectorEntry::AddClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnClicked.AddUnique(Handler);
}

void UModioModTagSelectorEntry::GetToggleable_Implementation(bool& bIsToggleable)
{
	bIsToggleable = bAllowToggleable;
}

void UModioModTagSelectorEntry::SetToggleable_Implementation(bool bNewToggleableState)
{
	bAllowToggleable = bNewToggleableState;
}

void UModioModTagSelectorEntry::GetSelectedState_Implementation(bool& bCurrentSelectedState)
{
	bCurrentSelectedState = bCurrentSelectionState;
}

void UModioModTagSelectorEntry::ToggleSelectedState_Implementation()
{
	if (bAllowSelection && bAllowToggleable)
	{
		IModioUISelectableWidget::Execute_SetSelectedState(this, !bCurrentSelectionState);
	}
}

void UModioModTagSelectorEntry::SetSelectedState_Implementation(bool bNewSelectedState)
{
	if (bAllowSelection)
	{
		bCurrentSelectionState = bNewSelectedState;
		NotifySelectionChanged();
		if (Execute_GetDataSource(this) && Execute_GetDataSource(this)->Implements<UModioModTagUIDetails>())
		{
			IModioModTagUIDetails::Execute_SetSelectionState(IModioUIDataSourceWidget::Execute_GetDataSource(this), bCurrentSelectionState);
		}
	}
	if (GetSelectionWidget().GetObject())
	{
		IModioUISelectableWidget::Execute_SetSelectedState(GetSelectionWidget().GetObject(), bNewSelectedState);
	}
}

void UModioModTagSelectorEntry::GetSelectable_Implementation(bool& bIsSelectable)
{
	bIsSelectable = bAllowSelection;
}

void UModioModTagSelectorEntry::SetSelectable_Implementation(bool bNewSelectableState)
{
	bAllowSelection = bNewSelectableState;
}

void UModioModTagSelectorEntry::RemoveSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	OnSelectionStateChanged.Remove(Handler);
}

void UModioModTagSelectorEntry::AddSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	OnSelectionStateChanged.AddUnique(Handler);
}
