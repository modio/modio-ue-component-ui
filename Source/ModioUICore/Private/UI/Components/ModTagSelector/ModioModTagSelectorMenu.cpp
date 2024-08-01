/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/ModTagSelector/ModioModTagSelectorMenu.h"

#include "UI/Interfaces/IModioUIObjectSelector.h"
#include "UI/ModioUICommonFunctionLibrary.h"

void UModioModTagSelectorMenu::SetAvailableTagsFromModTagOptions_Implementation(const FModioModTagOptions& InOptions)
{
	// Wrap the struct data in a UObject so it can be used for data binding and forward onto the method taking such a
	// wrapped UObject
	IModioUIModTagSelector::Execute_SetAvailableTagsFromBoundModTagOptions(
		this, TScriptInterface<UModioModTagOptionsUIDetails>(
				  UModioUICommonFunctionLibrary::CreateBindableModTagOptions(InOptions)));
}

void UModioModTagSelectorMenu::SetAvailableTagsFromBoundModTagOptions_Implementation(
	const TScriptInterface<UModioModTagOptionsUIDetails>& InOptions)
{
	CachedTagOptions = InOptions;
	// Pass the array of category objects to the internal widget that visualizes the categories
	if (GetCategoryContainerWidget().GetObject())
	{
		IModioUIObjectListWidget::Execute_SetObjects(
			GetCategoryContainerWidget().GetObject(),
			UModioUICommonFunctionLibrary::NativeConvertInterfaceToObjectArray(
				IModioModTagOptionsUIDetails::Execute_GetCategories(CachedTagOptions.GetObject())));
	}
}

TArray<FString> UModioModTagSelectorMenu::GetSelectedTags_Implementation()
{
	if (CachedTagOptions.GetObject() &&
		CachedTagOptions.GetObject()->GetClass()->ImplementsInterface(UModioModTagOptionsUIDetails::StaticClass()))
	{
		return IModioModTagOptionsUIDetails::Execute_GetSelectedTags(CachedTagOptions.GetObject());
	}
	return {};
}

void UModioModTagSelectorMenu::ClearSelectedTags_Implementation()
{
	if (UWidget* CategoryContainerWidget = ModioUI::GetInterfaceWidgetChecked(GetCategoryContainerWidget()))
	{
		// Clear the selected tags on our data source
		IModioModTagOptionsUIDetails::Execute_ClearSelectedTags(CachedTagOptions.GetObject());
		// Redraw the categories (and in turn their tags) by re-setting the data source containing categories to display
		// on the internal widget
		IModioUIObjectListWidget::Execute_SetObjects(
			CategoryContainerWidget,
			UModioUICommonFunctionLibrary::NativeConvertInterfaceToObjectArray(
				IModioModTagOptionsUIDetails::Execute_GetCategories(CachedTagOptions.GetObject())));
		NotifySelectionChanged();
	}
}

void UModioModTagSelectorMenu::AddTagSelectionChangedHandler_Implementation(const FModioOnTagSelectionChanged& Handler)
{
	OnTagSelectionChanged.AddUnique(Handler);
}

void UModioModTagSelectorMenu::RemoveTagSelectionChangedHandler_Implementation(
	const FModioOnTagSelectionChanged& Handler)
{
	OnTagSelectionChanged.Remove(Handler);
}

TScriptInterface<IModioUIObjectListWidget> UModioModTagSelectorMenu::GetCategoryContainerWidget_Implementation() const
{
	return nullptr;
}

void UModioModTagSelectorMenu::NotifySelectionChanged()
{
	OnTagSelectionChanged.Broadcast();
}

void UModioModTagSelectorMenu::OnCategoryWidgetCreated_Implementation(UWidget* CategoryWidget,
																	  UObject* CategoryDataSource)
{
	// For each category widget, when it is created, listen for an event indicating a tag's selection state has been
	// changed
	FModioOnObjectSelectionChanged CategorySelectionChangedDelegate;
	CategorySelectionChangedDelegate.BindUFunction(
		this, GET_FUNCTION_NAME_CHECKED(UModioModTagSelectorMenu, OnCategoryEntrySelectionChanged));
	IModioUIObjectSelector::Execute_AddSelectionChangedHandler(CategoryWidget, CategorySelectionChangedDelegate);
}

void UModioModTagSelectorMenu::OnCategoryEntrySelectionChanged_Implementation(UObject* NewlySelectedEntry)
{
	NotifySelectionChanged();
}

void UModioModTagSelectorMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
	// Bind a handler to the internal widget visualizing categories, so we get an event containing a reference to each
	// category widget when it is created
	if (GetCategoryContainerWidget().GetObject())
	{
		FModioObjectListOnObjectWidgetCreated WidgetCreationDelegate;
		WidgetCreationDelegate.BindUFunction(
			this, GET_FUNCTION_NAME_CHECKED(UModioModTagSelectorMenu, OnCategoryWidgetCreated));

		IModioUIObjectListWidget::Execute_AddObjectWidgetCreatedHandler(GetCategoryContainerWidget().GetObject(),
																		WidgetCreationDelegate);
	}
}
