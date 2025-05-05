/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/CommandMenu/ModioPopupCommandMenu.h"

#include "UI/Components/ComponentHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioPopupCommandMenu)

void UModioPopupCommandMenu::ConstructCommandMenuWidget_Implementation() {}

TScriptInterface<IModioUICommandMenu> UModioPopupCommandMenu::GetCommandMenuWidget_Implementation() const
{
	return nullptr;
}

void UModioPopupCommandMenu::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	if (UWidget* CommandMenuWidget = ModioUI::GetInterfaceWidgetChecked(GetCommandMenuWidget()))
	{
		IModioUIDataSourceWidget::Execute_SetDataSource(CommandMenuWidget, InDataSource);
	}
}

void UModioPopupCommandMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
	ConstructCommandMenuWidget();
}

void UModioPopupCommandMenu::RebuildCommandList_Implementation()
{
	if (UWidget* CommandMenuWidget = ModioUI::GetInterfaceWidgetChecked(GetCommandMenuWidget()))
	{
		IModioUICommandMenu::Execute_RebuildCommandList(CommandMenuWidget);
	}
}

void UModioPopupCommandMenu::RemoveCommandListBuilder_Implementation(const FModioCommandListBuilder& Builder)
{
	if (UWidget* CommandMenuWidget = ModioUI::GetInterfaceWidgetChecked(GetCommandMenuWidget()))
	{
		IModioUICommandMenu::Execute_RemoveCommandListBuilder(CommandMenuWidget, Builder);
	}
}

void UModioPopupCommandMenu::AddCommandListBuilder_Implementation(const FModioCommandListBuilder& Builder)
{
	if (UWidget* CommandMenuWidget = ModioUI::GetInterfaceWidgetChecked(GetCommandMenuWidget()))
	{
		IModioUICommandMenu::Execute_AddCommandListBuilder(CommandMenuWidget, Builder);
	}
}
