/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/ModTile/ModioDefaultModTileView.h"

#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#if WITH_EDITOR
	#include "Editor/WidgetCompilerLog.h"
#endif
void UModioDefaultModTileView::NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting)
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

	// TODO: Ideally we only want to regenerate our entries if our in list is empty or not. In some places we do not
	// guard against populating the view with the same data however so this is currently required to be called.
	RegenerateAllEntries();
}

void UModioDefaultModTileView::NativeSetModSelectionByID(FModioModID ModID)
{
	UObject* const* FirstItemMatchingModID = GetListItems().FindByPredicate([ModID](UObject* CurrentListItem) {
		if (UObject* DataSource = IModioUIDataSourceWidget::Execute_GetDataSource(CurrentListItem))
		{
			return IModioModInfoUIDetails::Execute_GetModID(DataSource) == ModID;
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

void UModioDefaultModTileView::NativeSetObjects(const TArray<UObject*>& InObjects)
{
	SetListItems(InObjects);

	// TODO: Ideally we only want to regenerate our entries if our in list is empty or not. In some places we do not
	// guard against populating the view with the same data however so this is currently required to be called.
	RegenerateAllEntries();
}

TArray<UObject*> UModioDefaultModTileView::NativeGetObjects()
{
	return GetListItems();
}

void UModioDefaultModTileView::NativeAddObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler)
{
	OnWidgetCreated.AddUnique(Handler);
}

void UModioDefaultModTileView::NativeRemoveObjectWidgetCreatedHandler(
	const FModioObjectListOnObjectWidgetCreated& Handler)
{
	OnWidgetCreated.Remove(Handler);
}

UUserWidget& UModioDefaultModTileView::OnGenerateEntryWidgetInternal(UObject* Item,
																	 TSubclassOf<UUserWidget> DesiredEntryClass,
																	 const TSharedRef<STableViewBase>& OwnerTable)
{
	UUserWidget& GeneratedEntryWidget = Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);
	OnWidgetCreated.Broadcast(&GeneratedEntryWidget, Item);
	return GeneratedEntryWidget;
}

#if WITH_EDITOR
void UModioDefaultModTileView::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
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