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

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioDefaultTagWidget)

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
			const FText LocalizedText = IModioModTagUIDetails::Execute_GetLocalizedText(InDataSource);
			const FString RawStringValue = IModioModTagUIDetails::Execute_GetRawStringValue(InDataSource);
			IModioUIHasTextWidget::Execute_SetWidgetText(GetLabelWidget().GetObject(),
			                                             LocalizedText.IsEmpty()
				                                             ? FText::FromString(RawStringValue)
				                                             : LocalizedText);
		}
	}
}
