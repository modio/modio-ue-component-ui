/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Templates/Default/FilterPanel/ModioTagCounterIcon.h"

TScriptInterface<IModioUIHasTextWidget> UModioTagCounterIcon::GetCounterLabelWidget_Implementation()
{
	return nullptr;
}

void UModioTagCounterIcon::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	SynchronizeCounter();
}

int32 UModioTagCounterIcon::SynchronizeCounter()
{
	int32 TagCount = 0;
	if (DataSource && (DataSource->GetClass()->ImplementsInterface(UModioUIModTagSelector::StaticClass())))
	{
		TagCount = IModioUIModTagSelector::Execute_GetSelectedTags(DataSource).Num();

		if (ModioUI::GetInterfaceWidgetChecked(GetCounterLabelWidget()))
		{
			IModioUIHasTextWidget::Execute_SetWidgetText(GetCounterLabelWidget().GetObject(),
														 FText::FromString(FString::FromInt(TagCount)));
		}
	}
	return TagCount;
}