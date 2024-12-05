/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/ModTagSelector/ModioModTagSelectorCategory.h"

#include "UI/Interfaces/IModioModTagUIDetails.h"
#include "UI/ModioUICommonFunctionLibrary.h"

#include "Components/ListView.h"

TScriptInterface<IModioUIObjectSelector> UModioModTagSelectorCategory::GetCategoryTagListWidget_Implementation() const
{
	return nullptr;
}

void UModioModTagSelectorCategory::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);

	if(GetTagCategoryLabel().GetObject() && InDataSource->Implements<UModioModTagCategoryUIDetails>())
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(
			GetTagCategoryLabel().GetObject(),
			IModioModTagCategoryUIDetails::Execute_GetLocalizedCategoryName(InDataSource));
	}
	
	// Ensure the data source is capable of providing information about a tag category
	if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModTagCategoryUIDetails::StaticClass()))
	{
		if (UWidget* CategoryTagListWidget = ModioUI::GetInterfaceWidgetChecked(GetCategoryTagListWidget()))
		{
			//IModioUIObjectSelector::Execute_ClearSelectedValues(CategoryTagListWidget);
			// Configure multiple or single selection on the internal widget based on the category parameters
			bool bAllowMultipleSelection =
				IModioModTagCategoryUIDetails::Execute_GetAllowMultipleSelection(InDataSource);
			IModioUIObjectSelector::Execute_SetMultiSelectionAllowed(CategoryTagListWidget,
																	 bAllowMultipleSelection);

			// Pass the tags for the associated category to the internal widget that will display them
			TArray<UObject*> TagCategoryEntries = UModioUICommonFunctionLibrary::NativeConvertInterfaceToObjectArray(
				IModioModTagCategoryUIDetails::Execute_GetCategoryTags(InDataSource));
			IModioUIObjectSelector::Execute_SetValues(CategoryTagListWidget, TagCategoryEntries);
		}
	}
}

bool UModioModTagSelectorCategory::GetMultiSelectionAllowed_Implementation()
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		return IModioUIObjectSelector::Execute_GetMultiSelectionAllowed(GetCategoryTagListWidget().GetObject());
	}
	return false;
}

int32 UModioModTagSelectorCategory::GetIndexForValue_Implementation(UObject* Value) const
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		return IModioUIObjectSelector::Execute_GetIndexForValue(GetCategoryTagListWidget().GetObject(), Value);
	}
	return INDEX_NONE;
}

UUserWidget* UModioModTagSelectorCategory::GetWidgetForValue_Implementation(UObject* Value) const
{
	if (GetCategoryTagListWidget().GetObject() &&
        GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
    {
        return IModioUIObjectSelector::Execute_GetWidgetForValue(GetCategoryTagListWidget().GetObject(), Value);
    }
    return nullptr;
}

TScriptInterface<IModioUIHasTextWidget> UModioModTagSelectorCategory::GetTagCategoryLabel_Implementation() const
{
	return nullptr;
}


void UModioModTagSelectorCategory::NativeOnEntryReleased()
{
	IUserListEntry::NativeOnEntryReleased();
}

void UModioModTagSelectorCategory::SetWidgetText_Implementation(const FText& DisplayText)
{
	if(GetTagCategoryLabel().GetObject())
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(GetTagCategoryLabel().GetObject(), DisplayText);
	}
}

FText UModioModTagSelectorCategory::GetWidgetText_Implementation()
{
	if (GetTagCategoryLabel().GetObject())
	{
		return	IModioUIHasTextWidget::Execute_GetWidgetText(GetTagCategoryLabel().GetObject());
	}
	else
	{
		return FText();
	}
}

void UModioModTagSelectorCategory::SetMultiSelectionAllowed_Implementation(bool bMultiSelectionAllowed)
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		IModioUIObjectSelector::Execute_SetMultiSelectionAllowed(GetCategoryTagListWidget().GetObject(),
																 bMultiSelectionAllowed);
	}
}

TArray<UObject*> UModioModTagSelectorCategory::GetSelectedValues_Implementation()
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		return IModioUIObjectSelector::Execute_GetSelectedValues(GetCategoryTagListWidget().GetObject());
	}
	return {};
}

void UModioModTagSelectorCategory::SetSelectedStateForValue_Implementation(UObject* Value, bool bNewSelectionState,
																		   bool bEmitSelectionEvent)
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		IModioUIObjectSelector::Execute_SetSelectedStateForValue(GetCategoryTagListWidget().GetObject(), Value,
																 bNewSelectionState, bEmitSelectionEvent);
	}
}

void UModioModTagSelectorCategory::SetSelectedStateForIndex_Implementation(int32 Index, bool bNewSelectionState,
																		   bool bEmitSelectionEvent)
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		IModioUIObjectSelector::Execute_SetSelectedStateForIndex(GetCategoryTagListWidget().GetObject(), Index,
																 bNewSelectionState, bEmitSelectionEvent);
	}
}

void UModioModTagSelectorCategory::RemoveSelectionChangedHandler_Implementation(
	const FModioOnObjectSelectionChanged& Handler)
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		IModioUIObjectSelector::Execute_RemoveSelectionChangedHandler(GetCategoryTagListWidget().GetObject(), Handler);
	}
}

void UModioModTagSelectorCategory::AddSelectionChangedHandler_Implementation(
	const FModioOnObjectSelectionChanged& Handler)
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		IModioUIObjectSelector::Execute_AddSelectionChangedHandler(GetCategoryTagListWidget().GetObject(), Handler);
	}
}

void UModioModTagSelectorCategory::SetSingleSelectionByValue_Implementation(UObject* Value, bool bEmitSelectionEvent)
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		IModioUIObjectSelector::Execute_SetSingleSelectionByValue(GetCategoryTagListWidget().GetObject(), Value,
															bEmitSelectionEvent);
	}
}

void UModioModTagSelectorCategory::SetSingleSelectionByIndex_Implementation(int32 Index, bool bEmitSelectionEvent)
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		IModioUIObjectSelector::Execute_SetSingleSelectionByIndex(GetCategoryTagListWidget().GetObject(), Index,
															bEmitSelectionEvent);
	}
}

void UModioModTagSelectorCategory::ClearSelectedValues_Implementation()
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		IModioUIObjectSelector::Execute_ClearSelectedValues(GetCategoryTagListWidget().GetObject());
	}
}

UObject* UModioModTagSelectorCategory::GetSingleSelectedValue_Implementation()
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		return IModioUIObjectSelector::Execute_GetSingleSelectedValue(GetCategoryTagListWidget().GetObject());
	}
	return nullptr;
}

int32 UModioModTagSelectorCategory::GetNumEntries_Implementation()
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		return IModioUIObjectSelector::Execute_GetNumEntries(GetCategoryTagListWidget().GetObject());
	}
	return 0;
}

int32 UModioModTagSelectorCategory::GetSingleSelectionIndex_Implementation()
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		return IModioUIObjectSelector::Execute_GetSingleSelectionIndex(GetCategoryTagListWidget().GetObject());
	}
	return INDEX_NONE;
}

void UModioModTagSelectorCategory::SetValues_Implementation(const TArray<UObject*>& InValues)
{
	if (GetCategoryTagListWidget().GetObject() &&
		GetCategoryTagListWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIObjectSelector::StaticClass()))
	{
		IModioUIObjectSelector::Execute_SetValues(GetCategoryTagListWidget().GetObject(), InValues);
	}
}
