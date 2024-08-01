/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioFeatureVisibilityWidget.h"

#include "ModioUISubsystem.h"
#include "Widgets/Layout/SSpacer.h"

TSharedRef<SWidget> UModioFeatureVisibilityWidget::RebuildWidget()
{
	if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (UISubsystem->IsUGCFeatureEnabled(FeatureToCheck))
		{
			if (GetChildrenCount() > 0)
			{
				UPanelSlot* ContentSlot = GetContentSlot();
				if (ContentSlot && ContentSlot->Content)
				{
					return ContentSlot->Content->TakeWidget();
				}
			}
			return SNew(SSpacer);
		}
		else
		{
			return SNew(SSpacer);
		}
	}
	return SNew(SSpacer);
}

UModioFeatureVisibilityWidget::UModioFeatureVisibilityWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

#if WITH_EDITOR
void UModioFeatureVisibilityWidget::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	RebuildWidget();
}
#endif