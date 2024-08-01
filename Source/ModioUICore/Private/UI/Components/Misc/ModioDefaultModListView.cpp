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
		if (UObject* DataSource = IModioUIDataSourceWidget::Execute_GetDataSource(CurrentListItem))
		{
			if (DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
			{
				return IModioModInfoUIDetails::Execute_GetModID(DataSource) == ModID;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	});
	if (FirstItemMatchingModID)
	{
		SetSelectedItem(*FirstItemMatchingModID);
	}
}

void UModioDefaultModListView::NativeSetObjects(const TArray<UObject*>& InObjects)
{
	SetListItems(InObjects);
}

TArray<UObject*> UModioDefaultModListView::NativeGetObjects()
{
	return GetListItems();
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

UUserWidget& UModioDefaultModListView::OnGenerateEntryWidgetInternal(UObject* Item,
																	 TSubclassOf<UUserWidget> DesiredEntryClass,
																	 const TSharedRef<STableViewBase>& OwnerTable)
{
	UUserWidget& GeneratedEntryWidget = Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	OnWidgetCreated.Broadcast(&GeneratedEntryWidget, Item);
	return GeneratedEntryWidget;
}