/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/EnumSelector/ModioEnumSelectorEntry.h"

#include "UI/Interfaces/IModioEnumEntryUIDetails.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioEnumSelectorEntry)

void UModioEnumSelectorEntry::InternalClickHandler(UObject* ClickContext)
{
	NotifyClicked();
}

void UModioEnumSelectorEntry::NotifyClicked()
{
	if (bEmitClickEvents)
	{
		OnClicked.Broadcast(this);
	}
}

void UModioEnumSelectorEntry::InternalSelectionChangedHandler(UObject* SelectionContext, bool bNewSelectedState)
{
	NotifySelectionChanged(bNewSelectedState);
}

void UModioEnumSelectorEntry::NotifySelectionChanged(bool bNewSelectedState)
{
	OnSelectionChanged.Broadcast(DataSource, bNewSelectedState);
}

TScriptInterface<IModioUIHasTextWidget> UModioEnumSelectorEntry::GetLabelWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIClickableWidget> UModioEnumSelectorEntry::GetClickableWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUISelectableWidget> UModioEnumSelectorEntry::GetSelectableWidget_Implementation() const
{
	return nullptr;
}

void UModioEnumSelectorEntry::NativeSetDataSource(UObject* InDataSource)
{
	if (InDataSource && InDataSource->GetClass()->ImplementsInterface(UModioEnumEntryUIDetails::StaticClass()))
	{
		Super::NativeSetDataSource(InDataSource);

		if (UWidget* TextWidget = ModioUI::GetInterfaceWidgetChecked(GetLabelWidget()))
		{
			// Query the data source for the display text associated with this enum value and apply it to the text
			// widget
			uint8 EnumValue;
			FText EnumDisplayText;
			IModioEnumEntryUIDetails::Execute_GetEnumEntryDetails(DataSource, EnumValue, EnumDisplayText);
			IModioUIHasTextWidget::Execute_SetWidgetText(TextWidget, EnumDisplayText);
		}
	}
	else
	{
		Super::NativeSetDataSource(nullptr);
	}
}

void UModioEnumSelectorEntry::NativePreConstruct()
{
	Super::NativePreConstruct();
	// Bind to the clicked event of the internal clickable widget so we can rebroadcast it, passing ourselves as the
	// clicked widget
	if (UWidget* ClickWidget = ModioUI::GetInterfaceWidgetChecked(GetClickableWidget()))
	{
		FModioClickableOnClicked ClickDelegate;
		ClickDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UModioEnumSelectorEntry, InternalClickHandler));
		IModioUIClickableWidget::Execute_AddClickedHandler(ClickWidget, ClickDelegate);
		IModioUIClickableWidget::Execute_EnableClick(ClickWidget);
	}
	// Ensure that our internal selectable widget is configured as selectable so it can display selection feedback
	if (UWidget* SelectableWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectableWidget()))
	{
		IModioUISelectableWidget::Execute_SetSelectable(SelectableWidget, true);
	}
}

void UModioEnumSelectorEntry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	// Notify our internal selectable widget that the selection state has changed so it can display feedback as
	// appropriate
	if (UWidget* SelectableWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectableWidget()))
	{
		IModioUISelectableWidget::Execute_SetSelectedState(SelectableWidget, bIsSelected);
	}
	// Broadcast our own selection changed event
	NotifySelectionChanged(bIsSelected);
}

void UModioEnumSelectorEntry::GetToggleable_Implementation(bool& bIsToggleable)
{
	if (UWidget* SelectableWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectableWidget()))
	{
		IModioUISelectableWidget::Execute_GetToggleable(SelectableWidget, bIsToggleable);
	}
}

void UModioEnumSelectorEntry::SetToggleable_Implementation(bool bNewToggleableState)
{
	if (UWidget* SelectableWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectableWidget()))
	{
		IModioUISelectableWidget::Execute_SetToggleable(SelectableWidget, bNewToggleableState);
	}
}

void UModioEnumSelectorEntry::GetSelectedState_Implementation(bool& bCurrentSelectedState)
{
	if (UWidget* SelectableWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectableWidget()))
	{
		IModioUISelectableWidget::Execute_GetSelectedState(SelectableWidget, bCurrentSelectedState);
	}
}

void UModioEnumSelectorEntry::ToggleSelectedState_Implementation()
{
	if (UWidget* SelectableWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectableWidget()))
	{
		IModioUISelectableWidget::Execute_ToggleSelectedState(SelectableWidget);
	}
}

void UModioEnumSelectorEntry::SetSelectedState_Implementation(bool bNewSelectedState)
{
	if (UWidget* SelectableWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectableWidget()))
	{
		IModioUISelectableWidget::Execute_SetSelectedState(SelectableWidget, bNewSelectedState);
	}
}

void UModioEnumSelectorEntry::GetSelectable_Implementation(bool& bIsSelectable)
{
	if (UWidget* SelectableWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectableWidget()))
	{
		IModioUISelectableWidget::Execute_GetSelectable(SelectableWidget, bIsSelectable);
	}
}

void UModioEnumSelectorEntry::SetSelectable_Implementation(bool bNewSelectableState)
{
	if (UWidget* SelectableWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectableWidget()))
	{
		IModioUISelectableWidget::Execute_SetSelectable(SelectableWidget, bNewSelectableState);
	}
}

void UModioEnumSelectorEntry::RemoveSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	OnSelectionChanged.Remove(Handler);
}

void UModioEnumSelectorEntry::AddSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	OnSelectionChanged.AddUnique(Handler);
}

void UModioEnumSelectorEntry::DisableClick_Implementation()
{
	bEmitClickEvents = false;
}

void UModioEnumSelectorEntry::EnableClick_Implementation()
{
	bEmitClickEvents = true;
}

void UModioEnumSelectorEntry::RemoveClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnClicked.Remove(Handler);
}

void UModioEnumSelectorEntry::AddClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnClicked.AddUnique(Handler);
}
