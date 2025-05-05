/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/PresetFilterSelector/ModioPresetFilterSelector.h"

#include "Core/ModioFilterParamsUI.h"
#include "Loc/ModioEnumLocalizationHelpers.h"
#include "ModioUISettings.h"
#include "UI/ModioUICommonFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioPresetFilterSelector)

void UModioPresetFilterSelector::SetValues_Implementation(const TArray<UObject*>& InValues)
{
	if (GetPresetListWidget().GetObject())
	{
		IModioUIObjectSelector::Execute_SetValues(GetPresetListWidget().GetObject(), InValues);
	}
}

int32 UModioPresetFilterSelector::GetNumEntries_Implementation()
{
	if (GetPresetListWidget().GetObject())
	{
		return IModioUIObjectSelector::Execute_GetNumEntries(GetPresetListWidget().GetObject());
	}
	return -1;
}

void UModioPresetFilterSelector::RemoveSelectionChangedHandler_Implementation(
	const FModioOnObjectSelectionChanged& Handler)
{
	if (GetPresetListWidget().GetObject())
	{
		IModioUIObjectSelector::Execute_RemoveSelectionChangedHandler(GetPresetListWidget().GetObject(), Handler);
	}
}

void UModioPresetFilterSelector::AddSelectionChangedHandler_Implementation(
	const FModioOnObjectSelectionChanged& Handler)
{
	if (GetPresetListWidget().GetObject())
	{
		IModioUIObjectSelector::Execute_AddSelectionChangedHandler(GetPresetListWidget().GetObject(), Handler);
	}
}

void UModioPresetFilterSelector::SetSingleSelectionByValue_Implementation(UObject* Value, bool bEmitSelectionEvent)
{
	if (GetPresetListWidget().GetObject())
	{
		IModioUIObjectSelector::Execute_SetSingleSelectionByValue(GetPresetListWidget().GetObject(), Value,
																  bEmitSelectionEvent);
	}
}

void UModioPresetFilterSelector::SetSingleSelectionByIndex_Implementation(int32 Index, bool bEmitSelectionEvent)
{
	if (GetPresetListWidget().GetObject())
	{
		IModioUIObjectSelector::Execute_SetSingleSelectionByIndex(GetPresetListWidget().GetObject(), Index,
																  bEmitSelectionEvent);
	}
}

void UModioPresetFilterSelector::ClearSelectedValues_Implementation()
{
	if (GetPresetListWidget().GetObject())
	{
		IModioUIObjectSelector::Execute_ClearSelectedValues(GetPresetListWidget().GetObject());
	}
}

UObject* UModioPresetFilterSelector::GetSingleSelectedValue_Implementation()
{
	if (GetPresetListWidget().GetObject())
	{
		return IModioUIObjectSelector::Execute_GetSingleSelectedValue(GetPresetListWidget().GetObject());
	}
	else
	{
		return nullptr;
	}
}

int32 UModioPresetFilterSelector::GetSingleSelectionIndex_Implementation()
{
	if (GetPresetListWidget().GetObject())
	{
		return IModioUIObjectSelector::Execute_GetSingleSelectionIndex(GetPresetListWidget().GetObject());
	}
	return -1;
}

TScriptInterface<IModioUIObjectSelector> UModioPresetFilterSelector::GetPresetListWidget_Implementation() const
{
	return nullptr;
}

TArray<TScriptInterface<IModioModFilterUIDetails>> UModioPresetFilterSelector::GetFilterPresets_Implementation()
{
	CachedBoundPresets.Empty();
	// Query the plugin settings for named filters to display
	if (const UModioUISettings* Settings = GetDefault<UModioUISettings>())
	{
		// Wrap each named filter in a UObject for data binding purposes
		TArray<TScriptInterface<IModioModFilterUIDetails>> BindablePresets;
		for (auto NamedFilter : Settings->PresetNamedFilters)
		{
			UModioFilterParamsUI* NewParams = NewObject<UModioFilterParamsUI>();
			NewParams->Underlying = NamedFilter.ToFilterParams();
			NewParams->FilterName = NamedFilter.PresetName;
			CachedBoundPresets.Add(NewParams);
			BindablePresets.Add(TScriptInterface<IModioModFilterUIDetails>(NewParams));
		}
		return BindablePresets;
	}
	return {};
}

void UModioPresetFilterSelector::NativePreConstruct()
{
	Super::NativePreConstruct();
	// Query for available filter presets and pass them as bindable objects to the internal selector widget
	IModioUIObjectSelector::Execute_SetValues(
		this, UModioUICommonFunctionLibrary::NativeConvertInterfaceToObjectArray(GetFilterPresets()));
}

bool UModioPresetFilterSelector::GetMultiSelectionAllowed_Implementation()
{
	return false;
}

void UModioPresetFilterSelector::SetMultiSelectionAllowed_Implementation(bool bMultiSelectionAllowed) {}

TArray<UObject*> UModioPresetFilterSelector::GetSelectedValues_Implementation()
{
	return {IModioUIObjectSelector::Execute_GetSingleSelectedValue(this)};
}

void UModioPresetFilterSelector::SetSelectedStateForValue_Implementation(UObject* Value, bool bNewSelectionState,
																		 bool bEmitSelectionEvent)
{
	// Because we are only supporting single selection, if we want to select a new value, just set it as the single
	// selection
	if (bNewSelectionState)
	{
		IModioUIObjectSelector::Execute_SetSingleSelectionByValue(this, Value, bEmitSelectionEvent);
	}
	else // If we are wanting to deselect, just defer to the existing single-selection code to clear the selection if
		 // we're trying to deselect the selected value
	{
		if (IModioUIObjectSelector::Execute_GetSingleSelectedValue(this) == Value)
		{
			IModioUIObjectSelector::Execute_ClearSelectedValues(this);
		}
	}
}

void UModioPresetFilterSelector::SetSelectedStateForIndex_Implementation(int32 Index, bool bNewSelectionState,
																		 bool bEmitSelectionEvent)
{
	if (bNewSelectionState)
	{
		IModioUIObjectSelector::Execute_SetSingleSelectionByIndex(this, Index, bEmitSelectionEvent);
	}
	else
	{
		if (Index < CachedBoundPresets.Num())
		{
			if (IModioUIObjectSelector::Execute_GetSingleSelectedValue(this) == CachedBoundPresets[Index])
			{
				IModioUIObjectSelector::Execute_ClearSelectedValues(this);
			}
		}
		else
		{
			IModioUIObjectSelector::Execute_ClearSelectedValues(this);
		}
	}
}
