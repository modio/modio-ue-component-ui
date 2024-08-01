/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/EnumSelector/ModioEnumSelector.h"

void UModioEnumSelector::SetValues_Implementation(const TArray<UObject*>& InValues)
{
	CachedValues = InValues;
	if (UWidget* SelectorMenu = ModioUI::GetInterfaceWidgetChecked(GetSelectorMenuWidget()))
	{
		IModioUIObjectSelector::Execute_SetValues(SelectorMenu, CachedValues);
	}
}

UObject* UModioEnumSelector::GetSingleSelectedValue_Implementation()
{
	if (UWidget* SelectorMenu = ModioUI::GetInterfaceWidgetChecked(GetSelectorMenuWidget()))
	{
		return IModioUIObjectSelector::Execute_GetSingleSelectedValue(SelectorMenu);
	}
	return nullptr;
}

void UModioEnumSelector::ClearSelectedValues_Implementation()
{
	if (UWidget* SelectorMenu = ModioUI::GetInterfaceWidgetChecked(GetSelectorMenuWidget()))
	{
		IModioUIObjectSelector::Execute_ClearSelectedValues(SelectorMenu);
	}
}

void UModioEnumSelector::SetSingleSelectionByIndex_Implementation(int32 Index, bool bEmitSelectionEvent)
{
	if (UWidget* SelectorMenu = ModioUI::GetInterfaceWidgetChecked(GetSelectorMenuWidget()))
	{
		IModioUIObjectSelector::Execute_SetSingleSelectionByIndex(SelectorMenu, Index, bEmitSelectionEvent);
	}
}

void UModioEnumSelector::SetSingleSelectionByValue_Implementation(UObject* Value, bool bEmitSelectionEvent)
{
	if (UWidget* SelectorMenu = ModioUI::GetInterfaceWidgetChecked(GetSelectorMenuWidget()))
	{
		IModioUIObjectSelector::Execute_SetSingleSelectionByValue(SelectorMenu, Value, bEmitSelectionEvent);
	}
}

void UModioEnumSelector::AddSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler)
{
	OnSelectionChanged.AddUnique(Handler);
}

void UModioEnumSelector::RemoveSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler)
{
	OnSelectionChanged.Remove(Handler);
}

TScriptInterface<IModioUIObjectSelector> UModioEnumSelector::GetSelectorMenuWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIClickableWidget> UModioEnumSelector::GetMenuToggleClickableWidget_Implementation() const
{
	return nullptr;
}

void UModioEnumSelector::InternalSelectionChangedHandler(UObject* SelectedObject)
{
	NotifySelectionChanged(SelectedObject);
}

void UModioEnumSelector::NotifySelectionChanged(UObject* SelectedObject)
{
	OnSelectionChanged.Broadcast(SelectedObject);
}

void UModioEnumSelector::NativePreConstruct()
{
	Super::NativePreConstruct();
	// Bind a handler to the internal menu widget's selection changed event so we can rebroadcast it
	if (UWidget* SelectorMenu = ModioUI::GetInterfaceWidgetChecked(GetSelectorMenuWidget()))
	{
		FModioOnObjectSelectionChanged SelectionDelegate;
		SelectionDelegate.BindUFunction(this,
										GET_FUNCTION_NAME_CHECKED(UModioEnumSelector, InternalSelectionChangedHandler));
		IModioUIObjectSelector::Execute_AddSelectionChangedHandler(SelectorMenu, SelectionDelegate);
	}
}

bool UModioEnumSelector::GetMultiSelectionAllowed_Implementation()
{
	if (UWidget* SelectorMenu = ModioUI::GetInterfaceWidgetChecked(GetSelectorMenuWidget()))
	{
		return IModioUIObjectSelector::Execute_GetMultiSelectionAllowed(SelectorMenu);
	}
	return false;
}

void UModioEnumSelector::SetMultiSelectionAllowed_Implementation(bool bMultiSelectionAllowed)
{
	if (UWidget* SelectorMenu = ModioUI::GetInterfaceWidgetChecked(GetSelectorMenuWidget()))
	{
		IModioUIObjectSelector::Execute_SetMultiSelectionAllowed(SelectorMenu, bMultiSelectionAllowed);
	}
}

TArray<UObject*> UModioEnumSelector::GetSelectedValues_Implementation()
{
	if (UWidget* SelectorMenu = ModioUI::GetInterfaceWidgetChecked(GetSelectorMenuWidget()))
	{
		return IModioUIObjectSelector::Execute_GetSelectedValues(SelectorMenu);
	}
	return {};
}

void UModioEnumSelector::SetSelectedStateForValue_Implementation(UObject* Value, bool bNewSelectionState,
																 bool bEmitSelectionEvent)
{
	if (UWidget* SelectorMenu = ModioUI::GetInterfaceWidgetChecked(GetSelectorMenuWidget()))
	{
		IModioUIObjectSelector::Execute_SetSelectedStateForValue(SelectorMenu, Value, bNewSelectionState,
																 bEmitSelectionEvent);
	}
}

void UModioEnumSelector::SetSelectedStateForIndex_Implementation(int32 Index, bool bNewSelectionState,
																 bool bEmitSelectionEvent)
{
	if (UWidget* SelectorMenu = ModioUI::GetInterfaceWidgetChecked(GetSelectorMenuWidget()))
	{
		IModioUIObjectSelector::Execute_SetSelectedStateForIndex(SelectorMenu, Index, bNewSelectionState,
																 bEmitSelectionEvent);
	}
}
