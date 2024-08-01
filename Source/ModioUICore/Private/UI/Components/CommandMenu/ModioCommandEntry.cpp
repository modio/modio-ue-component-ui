/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/CommandMenu/ModioCommandEntry.h"

#include "UI/Interfaces/IModioUICommandMenu.h"

void UModioCommandEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IModioUIDataSourceWidget::Execute_SetDataSource(this, ListItemObject);
}

void UModioCommandEntry::NativeSetDataSource(UObject* InDataSource)
{
	if (InDataSource && InDataSource->GetClass()->ImplementsInterface(UModioUICommandObject::StaticClass()))
	{
		DataSource = InDataSource;
		FText CommandDisplayText = IModioUICommandObject::Execute_GetCommandDisplayString(DataSource);
		if (UWidget* LabelWidget = ModioUI::GetInterfaceWidgetChecked(GetLabelWidget()))
		{
			IModioUIHasTextWidget::Execute_SetWidgetText(LabelWidget, CommandDisplayText);
		}
		if (UWidget* ClickWidget = ModioUI::GetInterfaceWidgetChecked(GetClickableWidget()))
		{
			// May need to be a BlueprintNativeEvent for enable/disable visual state
			if (IModioUICommandObject::Execute_CanExecute(DataSource, nullptr))
			{
				IModioUIClickableWidget::Execute_EnableClick(ClickWidget);
			}
			else
			{
				IModioUIClickableWidget::Execute_DisableClick(ClickWidget);
			}
		}
		if (UWidget* IconWidget = ModioUI::GetInterfaceWidgetChecked(GetIconWidget()))
		{
			FSlateBrush CommandIconBrush = IModioUICommandObject::Execute_GetCommandBrush(DataSource);
			IModioUIImageDisplayWidget::Execute_SetBrushDirectly(IconWidget, CommandIconBrush);
		}
	}
}

TScriptInterface<IModioUIHasTextWidget> UModioCommandEntry::GetLabelWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIClickableWidget> UModioCommandEntry::GetClickableWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIImageDisplayWidget> UModioCommandEntry::GetIconWidget_Implementation() const
{
	return nullptr;
}


void UModioCommandEntry::HandleInnerButtonClicked(UObject* ClickContext) 
{
	OnInnerButtonClicked.Broadcast(this);
}

void UModioCommandEntry::NativePreConstruct()
{
	Super::NativePreConstruct();
	// Add an intermediate delegate to the inner button so that we can replace a reference to the inner clickable with a reference to this widget
	if (UWidget* ClickWidget = ModioUI::GetInterfaceWidgetChecked(GetClickableWidget()))
	{
		FModioClickableOnClicked ClickDelegate;
		ClickDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED_OneParam(UModioCommandEntry, HandleInnerButtonClicked, UObject*));
		IModioUIClickableWidget::Execute_AddClickedHandler(ClickWidget, ClickDelegate);
	}

}

void UModioCommandEntry::DisableClick_Implementation()
{
	if (UWidget* ClickWidget = ModioUI::GetInterfaceWidgetChecked(GetClickableWidget()))
	{
		IModioUIClickableWidget::Execute_DisableClick(ClickWidget);
	}
}

void UModioCommandEntry::EnableClick_Implementation()
{
	if (UWidget* ClickWidget = ModioUI::GetInterfaceWidgetChecked(GetClickableWidget()))
	{
		IModioUIClickableWidget::Execute_EnableClick(ClickWidget);
	}
}

void UModioCommandEntry::RemoveClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnInnerButtonClicked.Remove(Handler);
}

void UModioCommandEntry::AddClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnInnerButtonClicked.AddUnique(Handler);
}
