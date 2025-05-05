/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/EnumSelector/ModioEnumSelectorMenu.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioEnumSelectorMenu)

void UModioEnumSelectorMenu::SetValues_Implementation(const TArray<UObject*>& InValues)
{
	CachedValues = InValues;
	if (UWidget* InnerListWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		IModioUIObjectSelector::Execute_SetValues(InnerListWidget, CachedValues);
	}
}

UObject* UModioEnumSelectorMenu::GetSingleSelectedValue_Implementation()
{
	if (UWidget* InnerListWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		return IModioUIObjectSelector::Execute_GetSingleSelectedValue(InnerListWidget);
	}
	return nullptr;
}

void UModioEnumSelectorMenu::ClearSelectedValues_Implementation()
{
	if (UWidget* InnerListWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		IModioUIObjectSelector::Execute_ClearSelectedValues(InnerListWidget);
	}
}

void UModioEnumSelectorMenu::SetSingleSelectionByIndex_Implementation(int32 Index, bool bEmitSelectionEvent)
{
	if (UWidget* InnerListWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		IModioUIObjectSelector::Execute_SetSingleSelectionByIndex(InnerListWidget, Index, bEmitSelectionEvent);
	}
}

void UModioEnumSelectorMenu::SetSingleSelectionByValue_Implementation(UObject* Value, bool bEmitSelectionEvent)
{
	if (UWidget* InnerListWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		IModioUIObjectSelector::Execute_SetSingleSelectionByValue(InnerListWidget, Value, bEmitSelectionEvent);
	}
}

void UModioEnumSelectorMenu::AddSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler)
{
	OnSelectionChanged.AddUnique(Handler);
}

void UModioEnumSelectorMenu::RemoveSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler)
{
	OnSelectionChanged.Remove(Handler);
}

void UModioEnumSelectorMenu::InternalSelectionChangedHandler(UObject* SelectedObject)
{
	NotifySelectionChanged(SelectedObject);
}

void UModioEnumSelectorMenu::NotifySelectionChanged(UObject* SelectedObject)
{
	OnSelectionChanged.Broadcast(SelectedObject);
}

TScriptInterface<IModioUIObjectSelector> UModioEnumSelectorMenu::GetSelectorWidget_Implementation() const
{
	return nullptr;
}

bool UModioEnumSelectorMenu::GetMultiSelectionAllowed_Implementation()
{
	if (UWidget* InnerListWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		return IModioUIObjectSelector::Execute_GetMultiSelectionAllowed(InnerListWidget);
	}
	return false;
}

void UModioEnumSelectorMenu::SetMultiSelectionAllowed_Implementation(bool bMultiSelectionAllowed)
{
	if (UWidget* InnerListWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		IModioUIObjectSelector::Execute_SetMultiSelectionAllowed(InnerListWidget, bMultiSelectionAllowed);
	}
}

TArray<UObject*> UModioEnumSelectorMenu::GetSelectedValues_Implementation()
{
	if (UWidget* InnerListWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		return IModioUIObjectSelector::Execute_GetSelectedValues(InnerListWidget);
	}
	return {};
}

void UModioEnumSelectorMenu::SetSelectedStateForValue_Implementation(UObject* Value, bool bNewSelectionState,
																	 bool bEmitSelectionEvent)
{
	if (UWidget* InnerListWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		IModioUIObjectSelector::Execute_SetSelectedStateForValue(InnerListWidget, Value, bNewSelectionState,
																 bEmitSelectionEvent);
	}
}

void UModioEnumSelectorMenu::SetSelectedStateForIndex_Implementation(int32 Index, bool bNewSelectionState,
																	 bool bEmitSelectionEvent)
{
	if (UWidget* InnerListWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		IModioUIObjectSelector::Execute_SetSelectedStateForIndex(InnerListWidget, Index, bNewSelectionState,
																 bEmitSelectionEvent);
	}
}

void UModioEnumSelectorMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
	// Bind a handler to the internal list widget's selection changed event so we can rebroadcast it
	if (UWidget* InnerListWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		FModioOnObjectSelectionChanged SelectionDelegate;
		SelectionDelegate.BindUFunction(
			this, GET_FUNCTION_NAME_CHECKED(UModioEnumSelectorMenu, InternalSelectionChangedHandler));
		IModioUIObjectSelector::Execute_AddSelectionChangedHandler(InnerListWidget, SelectionDelegate);
	}
}
