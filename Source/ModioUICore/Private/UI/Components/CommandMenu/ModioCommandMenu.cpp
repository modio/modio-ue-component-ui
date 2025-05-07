/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/CommandMenu/ModioCommandMenu.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioCommandMenu)

void UModioCommandMenu::AddCommandListBuilder_Implementation(const FModioCommandListBuilder& Builder)
{
	OnBuildCommandList.AddUnique(Builder);
}

void UModioCommandMenu::RemoveCommandListBuilder_Implementation(const FModioCommandListBuilder& Builder)
{
	OnBuildCommandList.Remove(Builder);
}

void UModioCommandMenu::RebuildCommandList_Implementation()
{
	CommandObjects.Empty();
	OnBuildCommandList.Broadcast(MutableView(CommandObjects), this);
	if (UWidget* SelectorWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		IModioUIObjectSelector::Execute_SetValues(SelectorWidget, CommandObjects);
	}
}

TScriptInterface<IModioUIObjectSelector> UModioCommandMenu::GetSelectorWidget_Implementation() const
{
	return nullptr;
}

void UModioCommandMenu::OnCommandSelected(UObject* SelectedCommandObject)
{
	if (SelectedCommandObject->GetClass()->ImplementsInterface(UModioUICommandObject::StaticClass()))
	{
		if (IModioUICommandObject::Execute_CanExecute(SelectedCommandObject, nullptr))
		{
			IModioUICommandObject::Execute_Execute(SelectedCommandObject, nullptr);
		}
	}
	// need to close or deactivate or similar
}

void UModioCommandMenu::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (UWidget* SelectorWidget = ModioUI::GetInterfaceWidgetChecked(GetSelectorWidget()))
	{
		FModioOnObjectSelectionChanged SelectionChangedDelegate;
		SelectionChangedDelegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UModioCommandMenu, OnCommandSelected));
		IModioUIObjectSelector::Execute_AddSelectionChangedHandler(SelectorWidget, SelectionChangedDelegate);
	}
}
