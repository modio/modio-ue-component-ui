/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioDefaultProgressBar.h"

#include "Misc/EngineVersionComparison.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioDefaultProgressBar)

void UModioDefaultProgressBar::AddValueChangedHandler_Implementation(const FModioProgressWidgetValueChanged& Handler)
{
	OnValueChanged.AddUnique(Handler);
}

void UModioDefaultProgressBar::RemoveValueChangedHandler_Implementation(const FModioProgressWidgetValueChanged& Handler)
{
	OnValueChanged.Remove(Handler);
}

void UModioDefaultProgressBar::SetColorOverride_Implementation(FLinearColor Override)
{
	if (!DefaultFillColor.IsSet())
	{
		DefaultFillColor = 
#if UE_VERSION_OLDER_THAN(5, 1, 0)
			FillColorAndOpacity;
#else
			GetFillColorAndOpacity();
#endif
	}
	SetFillColorAndOpacity(Override);
}

void UModioDefaultProgressBar::ClearColorOverride_Implementation()
{
	if (DefaultFillColor.IsSet())
	{
		SetFillColorAndOpacity(DefaultFillColor.GetValue());
	}
}

float UModioDefaultProgressBar::GetProgress_Implementation()
{
#if UE_VERSION_OLDER_THAN(5, 1, 0)
	return Percent;
#else
	return GetPercent();
#endif
}

void UModioDefaultProgressBar::SetProgress_Implementation(float NewProgressValue)
{
	SetPercent(NewProgressValue);
	OnValueChanged.Broadcast(NewProgressValue);
}

void UModioDefaultProgressBar::SetMarquee_Implementation(bool bNewIsMarquee)
{
	SetIsMarquee(bNewIsMarquee);
}

void UModioDefaultProgressBar::SetTooltipEnabledState_Implementation(bool bNewEnabledState)
{
	SetToolTipText(bNewEnabledState ? GetToolTipText() : FText::GetEmpty());
}

void UModioDefaultProgressBar::ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText, const FText& TagText)
{
	SetToolTipText(TitleText);
}
