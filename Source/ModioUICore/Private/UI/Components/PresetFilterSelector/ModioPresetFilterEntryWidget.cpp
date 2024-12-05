/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/PresetFilterSelector//ModioPresetFilterEntryWidget.h"

#include "UI/Interfaces/IModioModFilterUIDetails.h"


TScriptInterface<IModioUIHasTextWidget> UModioPresetFilterEntryWidget::GetLabelWidget_Implementation() const 
{
	return nullptr;
}

void UModioPresetFilterEntryWidget::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	// Query data source for the display name of the associated filter, and set the display text using the result
	if (InDataSource->Implements<UModioModFilterUIDetails>())
	{
		FText FilterName;
		if (IModioModFilterUIDetails::Execute_GetFilterName(InDataSource, FilterName))
		{
			IModioUIHasTextWidget::Execute_SetWidgetText(this, FilterName);
		}
	}
}

FText UModioPresetFilterEntryWidget::NativeGetText()
{
	if (GetLabelWidget().GetObject())
	{
		return IModioUIHasTextWidget::Execute_GetWidgetText(GetLabelWidget().GetObject());
	}
	else
	{
		return {};
	}
}

void UModioPresetFilterEntryWidget::NativeSetTextFont(FSlateFontInfo FontInfo)
{
	if (GetLabelWidget().GetObject())
	{
		IModioUIHasTextWidget::Execute_SetWidgetTextFont(GetLabelWidget().GetObject(), FontInfo);
	}
}

FSlateFontInfo UModioPresetFilterEntryWidget::NativeGetTextFont()
{
	if (GetLabelWidget().GetObject())
	{
		return IModioUIHasTextWidget::Execute_GetWidgetTextFont(GetLabelWidget().GetObject());
	}
	else
	{
		return FSlateFontInfo();
	}
}

void UModioPresetFilterEntryWidget::NativeSetText(const FText& DisplayText)
{
	if (GetLabelWidget().GetObject())
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(GetLabelWidget().GetObject(), DisplayText);
	}
}