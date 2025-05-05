/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Components/Misc/ModioPlatformSpecificWidget.h"

#include "Engine/Engine.h"
#include "Widgets/Layout/SSpacer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioPlatformSpecificWidget)

TSharedRef<SWidget> UModioPlatformSpecificWidget::RebuildWidget()
{
	/*if (ConcreteWidget == nullptr || bContentRequiresUpdate)
	{
		UpdateContent();
		bContentRequiresUpdate = false;
	}

	if (GetChildrenCount() > 0)
	{
		UPanelSlot* ContentSlot = GetContentSlot();
		if (ContentSlot->Content)
		{
			WrappedWidget = ContentSlot->Content->TakeWidget();
			return WrappedWidget.ToSharedRef();
		}
	}
	*/
	return SNew(SSpacer);
}
/*
void UModioPlatformSpecificWidget::UpdateContent()
{
	if (UPlatformInfoSubsystem* Subsystem = GEngine->GetEngineSubsystem<UPlatformInfoSubsystem>())
	{
		EPlatformName CurrentPlatformName = Subsystem->GetPlatform(false);
		if (PlatformWidgetReplacements.Contains(CurrentPlatformName))
		{
			if (!PlatformWidgetReplacements[CurrentPlatformName].IsNull())
			{
				UClass* WidgetClass = PlatformWidgetReplacements[CurrentPlatformName].LoadSynchronous();
				if (WidgetClass != nullptr)
				{
					ConcreteWidget = CreateWidget(this, WidgetClass);
					if (ConcreteWidget != nullptr)
					{
						SetContent(ConcreteWidget);
					}
				}
			}
		}
	}
}


void UModioPlatformSpecificWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	WrappedWidget.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}

UModioPlatformSpecificWidget::UModioPlatformSpecificWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Ensure that the TMap has all the required keys
	UEnum* PlatformEnumDescriptor = StaticEnum<EPlatformName>();
	if (PlatformEnumDescriptor != nullptr)
	{
		for (int32 CurrentValueIndex = 0; CurrentValueIndex < PlatformEnumDescriptor->NumEnums() - 1; CurrentValueIndex++)
		{
			PlatformWidgetReplacements.Add(
				static_cast<EPlatformName>(PlatformEnumDescriptor->GetValueByIndex(CurrentValueIndex)), {});
		}
	}
	Visibility = ESlateVisibility::SelfHitTestInvisible;
}

#if WITH_EDITOR
void UModioPlatformSpecificWidget::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	bContentRequiresUpdate = true;
	RebuildWidget();
}
#endif
*/