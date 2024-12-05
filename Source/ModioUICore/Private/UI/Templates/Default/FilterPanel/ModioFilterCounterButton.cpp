/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Templates/Default/FilterPanel/ModioFilterCounterButton.h"

#include "UI/Components/ComponentHelpers.h"

TScriptInterface<IModioUIDataSourceWidget> UModioFilterCounterButton::GetFilterCounterWidget_Implementation()
{
	return nullptr;
}

void UModioFilterCounterButton::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);

	if (ModioUI::GetInterfaceWidgetChecked(GetFilterCounterWidget()))
	{
		IModioUIDataSourceWidget::Execute_SetDataSource(GetFilterCounterWidget().GetObject(), InDataSource);
	}
}