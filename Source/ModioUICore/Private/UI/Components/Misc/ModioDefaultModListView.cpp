/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioDefaultModListView.h"

#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Interfaces/IModioUISelectableWidget.h"

#if WITH_EDITOR
	#include "Editor/WidgetCompilerLog.h"
#endif

#if WITH_EDITOR
void UModioDefaultModListView::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	if (EntryWidgetClass)
	{
		if (!EntryWidgetClass->ImplementsInterface(UModioUIDataSourceWidget::StaticClass()))
		{
			CompileLog.Error(FText::Format(FTextFormat::FromString("'{0}' has EntryWidgetClass property set to '{1}' "
																   "which does not implement ModioUIDataSourceWidget."),
										   FText::FromString(GetName()),
										   FText::FromString(EntryWidgetClass->GetName())));
		}
	}
}
#endif

void UModioDefaultModListView::NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting)
{
	ClearSelection();
	if (bAddToExisting)
	{
		TArray<UObject*> ExistingItems = GetListItems();
		ExistingItems.Append(InListItems);
		SetListItems(ExistingItems);
	}
	else
	{
		SetListItems(InListItems);
	}
}

void UModioDefaultModListView::NativeSetModSelectionByID(FModioModID ModID)
{
	UObject* const* FirstItemMatchingModID = GetListItems().FindByPredicate([ModID](UObject* CurrentListItem) {
		if (!CurrentListItem || !CurrentListItem->Implements<UModioModInfoUIDetails>())
		{
			return false;
		}
		return IModioModInfoUIDetails::Execute_GetModID(CurrentListItem) == ModID;
	});
	if (FirstItemMatchingModID)
	{
		SetSelectedItem(*FirstItemMatchingModID);
	}
}

void UModioDefaultModListView::NativeSetObjects(const TArray<UObject*>& InObjects)
{
	ClearListItems();
	ClearSelection();
	SetListItems(InObjects);

	// TODO: Ideally we only want to regenerate our entries if our in list is empty or not. In some places we do not
	// guard against populating the view with the same data however so this is currently required to be called.
	RegenerateAllEntries();
}

TArray<UObject*> UModioDefaultModListView::NativeGetObjects()
{
	return GetListItems();
}

UObject* UModioDefaultModListView::NativeGetObjectAt(int32 Index) const
{
	return GetItemAt(Index);
}

void UModioDefaultModListView::NativeAddObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler)
{
	OnWidgetCreated.AddUnique(Handler);
}

void UModioDefaultModListView::NativeRemoveObjectWidgetCreatedHandler(
	const FModioObjectListOnObjectWidgetCreated& Handler)
{
	OnWidgetCreated.Remove(Handler);
}

void UModioDefaultModListView::SetValues_Implementation(const TArray<UObject*>& InValues)
{
	ClearListItems();
	ClearSelection();
	SetListItems(InValues);
	RegenerateAllEntries();
}

int32 UModioDefaultModListView::GetNumEntries_Implementation()
{
	return GetNumItems();
}

UObject* UModioDefaultModListView::GetSingleSelectedValue_Implementation()
{
	return GetSelectedItem();
}

int32 UModioDefaultModListView::GetSingleSelectionIndex_Implementation()
{
	return GetIndexForItem(GetSelectedItem());
}

void UModioDefaultModListView::ClearSelectedValues_Implementation()
{
	ClearSelection();
	if (GetSelectionMode() == ESelectionMode::Single)
	{
		// Sometimes the selection is not fully cleared by ClearSelection for the displayed entries
		// so we need to manually clear the selection state for each displayed entry
		for (UUserWidget* CurrentWidget : GetDisplayedEntryWidgets())
		{
			IUserListEntry::UpdateItemSelection(*CurrentWidget, false);
		}
	}
	NotifySelectionChanged(nullptr);
}

void UModioDefaultModListView::SetSingleSelectionByIndex_Implementation(int32 Index, bool bEmitSelectionEvent)
{
	EmitSelectionEvents.Push(bEmitSelectionEvent);
	SetSelectedIndex(Index);
	NotifySelectionChanged(GetItemAt(Index));
	EmitSelectionEvents.Pop();
}

void UModioDefaultModListView::SetSingleSelectionByValue_Implementation(UObject* Value, bool bEmitSelectionEvent)
{
	EmitSelectionEvents.Push(bEmitSelectionEvent);
	SetSelectedItem(Value);
	NotifySelectionChanged(Value);
	EmitSelectionEvents.Pop();
}

void UModioDefaultModListView::AddSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler)
{
	OnSelectedValueChanged.AddUnique(Handler);
}

void UModioDefaultModListView::RemoveSelectionChangedHandler_Implementation(
	const FModioOnObjectSelectionChanged& Handler)
{
	OnSelectedValueChanged.Remove(Handler);
}

void UModioDefaultModListView::SetSelectedStateForIndex_Implementation(int32 Index, bool bNewSelectionState,
	bool bEmitSelectionEvent)
{
	IModioUIObjectSelector::Execute_SetSelectedStateForValue(this, GetItemAt(Index), bNewSelectionState,
															 bEmitSelectionEvent);
}

void UModioDefaultModListView::SetSelectedStateForValue_Implementation(UObject* Value, bool bNewSelectionState,
	bool bEmitSelectionEvent)
{
	// override the default event emission setting so that our handler for selection changed knows if it should emit
	// events
	EmitSelectionEvents.Push(bEmitSelectionEvent);
	SetItemSelection(Value, bNewSelectionState, ESelectInfo::Direct);
	NotifySelectionChanged(Value);
	EmitSelectionEvents.Pop();
}

TArray<UObject*> UModioDefaultModListView::GetSelectedValues_Implementation()
{
	TArray<UObject*> OutArray;
	GetSelectedItems(OutArray);
	return OutArray;
}

void UModioDefaultModListView::SetMultiSelectionAllowed_Implementation(bool bMultiSelectionAllowed)
{
	SetSelectionMode(bMultiSelectionAllowed ? ESelectionMode::Multi : ESelectionMode::Single);
}

bool UModioDefaultModListView::GetMultiSelectionAllowed_Implementation()
{
	return SelectionMode == ESelectionMode::Multi;
}

void UModioDefaultModListView::SetListEntryWidgetClass_Implementation(TSubclassOf<UWidget> InNewEntryClass)
{
	EntryWidgetClass = InNewEntryClass;
	//some kind of rebuild here maybe?
}

int32 UModioDefaultModListView::GetIndexForValue_Implementation(UObject* Value) const
{
	return GetIndexForItem(Value);
}

UUserWidget& UModioDefaultModListView::OnGenerateEntryWidgetInternal(UObject* Item,
                                                                     TSubclassOf<UUserWidget> DesiredEntryClass,
                                                                     const TSharedRef<STableViewBase>& OwnerTable)
{
	UUserWidget& GeneratedEntryWidget = Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	OnWidgetCreated.Broadcast(&GeneratedEntryWidget, Item);
	return GeneratedEntryWidget;
}

void UModioDefaultModListView::NotifySelectionChanged(UObject* SelectedItem)
{
	// If we had an existing selection, make sure we notify it that it is no longer selected
	if (!GetMultiSelectionAllowed_Implementation())
	{
		if (PreviouslySelectedWidget != GetEntryWidgetFromItem(SelectedItem))
		{
			if (PreviouslySelectedWidget.IsValid() &&
				PreviouslySelectedWidget->GetClass()->ImplementsInterface(UModioUISelectableWidget::StaticClass()))
			{
				IModioUISelectableWidget::Execute_SetSelectedState(PreviouslySelectedWidget.Get(), false);
			}
			PreviouslySelectedWidget = GetEntryWidgetFromItem(SelectedItem);
		}
	}

	if (EmitSelectionEvents.Peek(true))
	{
		OnSelectedValueChanged.Broadcast(SelectedItem);
	}
}