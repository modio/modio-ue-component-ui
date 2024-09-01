/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/ModTagSelector/ModioModTagSelector.h"

#include "UI/ModioUICommonFunctionLibrary.h"

void UModioModTagSelector::SetAvailableTagsFromModTagOptions_Implementation(const FModioModTagOptions& InOptions)
{
	IModioUIModTagSelector::Execute_SetAvailableTagsFromBoundModTagOptions(
		this, TScriptInterface<UModioModTagOptionsUIDetails>(UModioUICommonFunctionLibrary::CreateBindableModTagOptions(InOptions)));
}

void UModioModTagSelector::SetAvailableTagsFromBoundModTagOptions_Implementation(
	const TScriptInterface<UModioModTagOptionsUIDetails>& InOptions)
{
	// Keep a pointer to the bound tags so that we can inspect the selection state directly rather than asking the menu
	CachedTagOptions = InOptions;
	if (GetSelectorMenuWidget().GetObject())
	{
		IModioUIModTagSelector::Execute_SetAvailableTagsFromBoundModTagOptions(GetSelectorMenuWidget().GetObject(),
																			   CachedTagOptions);
	}
}

TArray<FString> UModioModTagSelector::GetSelectedTags_Implementation()
{
	if (CachedTagOptions.GetObject() &&
		CachedTagOptions.GetObject()->GetClass()->ImplementsInterface(UModioModTagOptionsUIDetails::StaticClass()))
	{
		return IModioModTagOptionsUIDetails::Execute_GetSelectedTags(CachedTagOptions.GetObject());
	}
	return {};
}

void UModioModTagSelector::ClearSelectedTags_Implementation()
{
	if (GetSelectorMenuWidget().GetObject())
	{
		IModioUIModTagSelector::Execute_ClearSelectedTags(GetSelectorMenuWidget().GetObject());
	}
}

void UModioModTagSelector::AddTagSelectionChangedHandler_Implementation(const FModioOnTagSelectionChanged& Handler)
{
	if (GetSelectorMenuWidget().GetObject())
	{
		IModioUIModTagSelector::Execute_AddTagSelectionChangedHandler(GetSelectorMenuWidget().GetObject(), Handler);
	}
}

void UModioModTagSelector::RemoveTagSelectionChangedHandler_Implementation(const FModioOnTagSelectionChanged& Handler)
{
	if (GetSelectorMenuWidget().GetObject())
	{
		IModioUIModTagSelector::Execute_RemoveTagSelectionChangedHandler(GetSelectorMenuWidget().GetObject(), Handler);
	}
}

TScriptInterface<UModioUIModTagSelector> UModioModTagSelector::GetSelectorMenuWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<UModioUIClickableWidget> UModioModTagSelector::GetMenuToggleClickableWidget_Implementation() const
{
	return nullptr;
}
