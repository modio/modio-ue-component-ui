/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/TagDisplay//ModioDefaultTagWidget.h"

#include "UI/Interfaces/IModioModTagUIDetails.h"

TScriptInterface<IModioUIHasTextWidget> UModioDefaultTagWidget::GetLabelWidget_Implementation() const 
{
	return nullptr;
}

void UModioDefaultTagWidget::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	if (GetLabelWidget().GetObject())
	{
		if (InDataSource && InDataSource->GetClass()->ImplementsInterface(UModioModTagUIDetails::StaticClass()))
		{
			IModioUIHasTextWidget::Execute_SetWidgetText(
				GetLabelWidget().GetObject(), IModioModTagUIDetails::Execute_GetLocalizedText(InDataSource));
		}
	}
}
