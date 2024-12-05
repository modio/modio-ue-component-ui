/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/TokenPackTile/ModioDefaultTokenPackTileView.h"

#include "UI/Components/Slate/SModioDataSourceAwareTableRow.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Interfaces/IModioUISelectableWidget.h"
#include "UI/Interfaces/ModioTokenPackUIDetails.h"

#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"

void UModioDefaultTokenPackTileView::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
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

void UModioDefaultTokenPackTileView::NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting)
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

	// TODO: Ideally we only want to regenerate our entries if our in list is empty or not. In some places we do not
	// guard against populating the view with the same data however so this is currently required to be called.
	RegenerateAllEntries();
}

void UModioDefaultTokenPackTileView::NativeSetTokenPackSelectionByID(FModioTokenPackID TokenPackID)
{
	UObject* const* FirstItemMatchingTokenPackID = GetListItems().FindByPredicate([TokenPackID](UObject* CurrentListItem) {
		if (!CurrentListItem || !CurrentListItem->Implements<UModioTokenPackUIDetails>())
		{
			return false;
		}
		return IModioTokenPackUIDetails::Execute_GetTokenPackID(CurrentListItem) == TokenPackID;
	});
	if (FirstItemMatchingTokenPackID)
	{
		SetSelectedItem(*FirstItemMatchingTokenPackID);
	}
}

void UModioDefaultTokenPackTileView::NativeSetObjects(const TArray<UObject*>& InObjects)
{
	ClearListItems();
	ClearSelection();
	SetListItems(InObjects);

	// TODO: Ideally we only want to regenerate our entries if our in list is empty or not. In some places we do not
	// guard against populating the view with the same data however so this is currently required to be called.
	RegenerateAllEntries();
}

TArray<UObject*> UModioDefaultTokenPackTileView::NativeGetObjects()
{
	return GetListItems();
}

UObject* UModioDefaultTokenPackTileView::NativeGetObjectAt(int32 Index) const
{
	return GetItemAt(Index);
}

void UModioDefaultTokenPackTileView::NativeAddObjectWidgetCreatedHandler(
	const FModioObjectListOnObjectWidgetCreated& Handler)
{
	OnWidgetCreated.AddUnique(Handler);
}

void UModioDefaultTokenPackTileView::NativeRemoveObjectWidgetCreatedHandler(
	const FModioObjectListOnObjectWidgetCreated& Handler)
{
	OnWidgetCreated.Remove(Handler);
}

void UModioDefaultTokenPackTileView::SetValues_Implementation(const TArray<UObject*>& InValues)
{
	ClearListItems();
	ClearSelection();
	SetListItems(InValues);
	RegenerateAllEntries();
}

int32 UModioDefaultTokenPackTileView::GetNumEntries_Implementation()
{
	return GetNumItems();
}

UObject* UModioDefaultTokenPackTileView::GetSingleSelectedValue_Implementation()
{
	return GetSelectedItem();
}

int32 UModioDefaultTokenPackTileView::GetSingleSelectionIndex_Implementation()
{
	return GetIndexForItem(GetSelectedItem());
}

void UModioDefaultTokenPackTileView::ClearSelectedValues_Implementation()
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

void UModioDefaultTokenPackTileView::SetSingleSelectionByIndex_Implementation(int32 Index, bool bEmitSelectionEvent)
{
	EmitSelectionEvents.Push(bEmitSelectionEvent);
	SetSelectedIndex(Index);
	NotifySelectionChanged(GetItemAt(Index));
	EmitSelectionEvents.Pop();
}

void UModioDefaultTokenPackTileView::SetSingleSelectionByValue_Implementation(UObject* Value, bool bEmitSelectionEvent)
{
	EmitSelectionEvents.Push(bEmitSelectionEvent);
	SetSelectedItem(Value);
	NotifySelectionChanged(Value);
	EmitSelectionEvents.Pop();
}

void UModioDefaultTokenPackTileView::AddSelectionChangedHandler_Implementation(
	const FModioOnObjectSelectionChanged& Handler)
{
	OnSelectedValueChanged.AddUnique(Handler);
}

void UModioDefaultTokenPackTileView::RemoveSelectionChangedHandler_Implementation(
	const FModioOnObjectSelectionChanged& Handler)
{
	OnSelectedValueChanged.Remove(Handler);
}

void UModioDefaultTokenPackTileView::SetSelectedStateForIndex_Implementation(int32 Index, bool bNewSelectionState,
	bool bEmitSelectionEvent)
{
	IModioUIObjectSelector::Execute_SetSelectedStateForValue(this, GetItemAt(Index), bNewSelectionState,
															 bEmitSelectionEvent);
}

void UModioDefaultTokenPackTileView::SetSelectedStateForValue_Implementation(UObject* Value, bool bNewSelectionState,
	bool bEmitSelectionEvent)
{
	// override the default event emission setting so that our handler for selection changed knows if it should emit
	// events
	EmitSelectionEvents.Push(bEmitSelectionEvent);
	SetItemSelection(Value, bNewSelectionState, ESelectInfo::Direct);
	NotifySelectionChanged(Value);
	EmitSelectionEvents.Pop();
}

TArray<UObject*> UModioDefaultTokenPackTileView::GetSelectedValues_Implementation()
{
	TArray<UObject*> OutArray;
	GetSelectedItems(OutArray);
	return OutArray;
}

int32 UModioDefaultTokenPackTileView::GetLastSelectionIndex_Implementation()
{
	return LastSelectedIndex;
}

void UModioDefaultTokenPackTileView::SetMultiSelectionAllowed_Implementation(bool bMultiSelectionAllowed)
{
	SetSelectionMode(bMultiSelectionAllowed ? ESelectionMode::Multi : ESelectionMode::Single);
}

bool UModioDefaultTokenPackTileView::GetMultiSelectionAllowed_Implementation()
{
	return SelectionMode == ESelectionMode::Multi;
}

void UModioDefaultTokenPackTileView::SetListEntryWidgetClass_Implementation(TSubclassOf<UWidget> InNewEntryClass)
{
	EntryWidgetClass = InNewEntryClass;
	// some kind of rebuild here maybe?
}

int32 UModioDefaultTokenPackTileView::GetIndexForValue_Implementation(UObject* Value) const
{
	return GetIndexForItem(Value);
}

UUserWidget& UModioDefaultTokenPackTileView::OnGenerateEntryWidgetInternal(UObject* Item,
	TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	UUserWidget& GeneratedEntryWidget = [this, DesiredEntryClass, OwnerTable, Item]() -> UUserWidget&
	{
		if (DesiredEntryClass->ImplementsInterface(UModioUIDataSourceWidget::StaticClass()))
		{
			return GenerateTypedEntry<UUserWidget, SModioDataSourceAwareTableRow<UObject*>>(DesiredEntryClass, OwnerTable);
		}
		return Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	}();
	OnWidgetCreated.Broadcast(&GeneratedEntryWidget, Item);
	return GeneratedEntryWidget;
}

void UModioDefaultTokenPackTileView::NotifySelectionChanged(UObject* SelectedItem)
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

	LastSelectedIndex = GetIndexForItem(SelectedItem);
}
