/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioModRatingDisplay.h"

#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"

TScriptInterface<IModioUIHasTextWidget> UModioModRatingDisplay::GetRatingPercentageLabelWidget_Implementation() const 
{
	return nullptr;
}

TScriptInterface<IModioUIHasTextWidget> UModioModRatingDisplay::GetRatingDescriptionLabelWidget_Implementation() const
{
	return nullptr;
}

void UModioModRatingDisplay::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	// Query the new data source for details about a mod
	if (DataSource&& DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
	{
		FModioModInfo BoundModInfo = IModioModInfoUIDetails::Execute_GetFullModInfo(DataSource);
		// Display the textual rating and numeric rating on the respective text widgets
		if (GetRatingDescriptionLabelWidget().GetObject())
		{
			IModioUIHasTextWidget::Execute_SetWidgetText(GetRatingDescriptionLabelWidget().GetObject(),
														 FText::FromString(BoundModInfo.Stats.RatingDisplayText));
		}
		if (GetRatingPercentageLabelWidget().GetObject())
		{
			IModioUIHasTextWidget::Execute_SetWidgetText(GetRatingPercentageLabelWidget().GetObject(),
														 FText::AsPercent(static_cast<float>(BoundModInfo.Stats.RatingPercentagePositive)/100.f));
		}
	}
}

void UModioModRatingDisplay::NativePreConstruct()
{
	Super::NativePreConstruct();
#if WITH_EDITORONLY_DATA
	if (EditorDataSource && IsDesignTime())
	{
		IModioUIDataSourceWidget::Execute_SetDataSource(this, EditorDataSource);
	}
#endif
}
