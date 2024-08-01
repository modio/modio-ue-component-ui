/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Templates/Default/Misc/ModioOperationTracker.h"

#include "Math/UnrealMathUtility.h"
#include "Types/ModioUnsigned64.h"
#include "TimerManager.h"

void UModioOperationTracker::NativePreConstruct()
{
	Super::NativePreConstruct();
	IModioUISubscriptionsChangedReceiver::Register<UModioOperationTracker>();
	IModioUIModManagementEventReceiver::Register<UModioOperationTracker>();
	HideSubWidgets();
	RefreshQueueData();
}
void UModioOperationTracker::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bPollInstallProgress)
	{
		if (UWidget* RadialProgressWidget = ModioUI::GetInterfaceWidgetChecked(GetRadialProgressWidget()))
		{
			IModioUIProgressWidget::Execute_SetProgress(RadialProgressWidget, GetInstallProgressPercent());
		}
		if (UWidget* ProgressTextWidget = ModioUI::GetInterfaceWidgetChecked(GetProgressTextWidget()))
		{
			IModioUIHasTextWidget::Execute_SetWidgetText(ProgressTextWidget,
														 FText::AsPercent(GetInstallProgressPercent()));
		}
		RefreshQueueData();
	}
}
void UModioOperationTracker::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);

	switch (Event.Event)
	{
		case EModioModManagementEventType::BeginInstall:
		case EModioModManagementEventType::BeginUpdate:
		case EModioModManagementEventType::BeginUninstall:
			HandleOperationBegin();
			break;
		case EModioModManagementEventType::Installed:
		case EModioModManagementEventType::Updated:
		case EModioModManagementEventType::Uninstalled:
			HandleOperationComplete();
			break;
	}
	RefreshQueueData();
}
void UModioOperationTracker::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
	RefreshQueueData();
}
void UModioOperationTracker::SetInitialVisualState()
{
	if (UWidget* RadialProgress = ModioUI::GetInterfaceWidgetChecked(GetRadialProgressWidget()))
	{
		IModioUIProgressWidget::Execute_SetProgress(RadialProgress, 0.0f);
	}
	if (UWidget* ProgressText = ModioUI::GetInterfaceWidgetChecked(GetProgressTextWidget()))
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(ProgressText, FText::AsPercent(0.f));
	}
	if (UWidget* OperationTypeIcon = ModioUI::GetInterfaceWidgetChecked(GetOperationTypeIconWidget()))
	{
		IModioUIImageDisplayWidget::Execute_SetBrushDirectly(OperationTypeIcon, FSlateNoResource());
	}
}
void UModioOperationTracker::HandleOperationBegin()
{
	bPollInstallProgress = true;
	SetInitialVisualState();
}
void UModioOperationTracker::HandleOperationComplete_Implementation()
{
	bPollInstallProgress = false;
	if (UWidget* RadialProgress = ModioUI::GetInterfaceWidgetChecked(GetRadialProgressWidget()))
	{
		IModioUIProgressWidget::Execute_SetProgress(RadialProgress, 1.0f);
	}
	if (UWidget* ProgressTextWidget = ModioUI::GetInterfaceWidgetChecked(GetProgressTextWidget()))
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(ProgressTextWidget, FText::AsPercent(1.0f));
	}
}
void UModioOperationTracker::RefreshQueueData_Implementation()
{
	int32 NumQueuedMods = GetNumUserSubscriptions() - GetNumUserInstallations();
	if (NumQueuedMods == 0)
	{
		bPollInstallProgress = false;
		if (UWidget* QueueCountTextWidget = ModioUI::GetInterfaceWidgetChecked(GetQueueCountTextWidget()))
		{
			IModioUIHasTextWidget::Execute_SetWidgetText(QueueCountTextWidget, FText::FromString(FString::FromInt(0)));
		}
		FTimerHandle TimerHandle_AllOperationsComplete;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AllOperationsComplete, this,
											   &UModioOperationTracker::OnAllOperationsComplete, 4.0f, false);
		return;
	}
	if (UWidget* QueueCountTextWidget = ModioUI::GetInterfaceWidgetChecked(GetQueueCountTextWidget()))
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(QueueCountTextWidget,
													 FText::FromString(FString::FromInt(NumQueuedMods)));
	}
	ShowSubWidgets();
}
void UModioOperationTracker::OnAllOperationsComplete()
{
	HideSubWidgets();
	SetInitialVisualState();
}
float UModioOperationTracker::GetInstallProgressPercent()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		if (TOptional<FModioModProgressInfo> ProgressInfo = Subsystem->QueryCurrentModUpdate())
		{
			switch (ProgressInfo.GetValue().GetCurrentState())
			{
				case EModioModProgressState::Downloading:
					return UModioUnsigned64Library::DivideToFloat(
						ProgressInfo.GetValue().GetCurrentProgress(EModioModProgressState::Downloading),
						ProgressInfo.GetValue().GetTotalProgress(EModioModProgressState::Downloading));
					break;

				case EModioModProgressState::Extracting:
					return UModioUnsigned64Library::DivideToFloat(
						ProgressInfo.GetValue().GetCurrentProgress(EModioModProgressState::Extracting),
						ProgressInfo.GetValue().GetTotalProgress(EModioModProgressState::Extracting));
					break;
			}
		}
	}
	return 0;
}
int32 UModioOperationTracker::GetNumUserSubscriptions() const
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		const TMap<FModioModID, FModioModCollectionEntry>& UserSubscriptions = Subsystem->QueryUserSubscriptions();
		return UserSubscriptions.Num();
	}
	return 0;
}
int32 UModioOperationTracker::GetNumUserInstallations() const
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		const TMap<FModioModID, FModioModCollectionEntry>& UserInstallations = Subsystem->QueryUserInstallations(false);
		return UserInstallations.Num();
	}
	return 0;
}
void UModioOperationTracker::HideSubWidgets()
{
	if (UWidget* ProgressTextWidget = ModioUI::GetInterfaceWidgetChecked(GetProgressTextWidget()))
	{
		ProgressTextWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (UWidget* RadialProgressWidget = ModioUI::GetInterfaceWidgetChecked(GetRadialProgressWidget()))
	{
		RadialProgressWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (UWidget* ModCountIconWidget = ModioUI::GetInterfaceWidgetChecked(GetQueueCountIconWidget()))
	{
		ModCountIconWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (UWidget* ModCountTextWidget = ModioUI::GetInterfaceWidgetChecked(GetQueueCountTextWidget()))
	{
		ModCountTextWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (UWidget* OperationIconWidget = ModioUI::GetInterfaceWidgetChecked(GetOperationTypeIconWidget()))
	{
		OperationIconWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
void UModioOperationTracker::ShowSubWidgets()
{
	if (UWidget* ProgressTextWidget = ModioUI::GetInterfaceWidgetChecked(GetProgressTextWidget()))
	{
		ProgressTextWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (UWidget* RadialProgressWidget = ModioUI::GetInterfaceWidgetChecked(GetRadialProgressWidget()))
	{
		RadialProgressWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (UWidget* ModCountIconWidget = ModioUI::GetInterfaceWidgetChecked(GetQueueCountIconWidget()))
	{
		ModCountIconWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (UWidget* ModCountTextWidget = ModioUI::GetInterfaceWidgetChecked(GetQueueCountTextWidget()))
	{
		ModCountTextWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (UWidget* OperationIconWidget = ModioUI::GetInterfaceWidgetChecked(GetOperationTypeIconWidget()))
	{
		OperationIconWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
TScriptInterface<IModioUIHasTextWidget> UModioOperationTracker::GetProgressTextWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIProgressWidget> UModioOperationTracker::GetRadialProgressWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIImageDisplayWidget> UModioOperationTracker::GetQueueCountIconWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIHasTextWidget> UModioOperationTracker::GetQueueCountTextWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIImageDisplayWidget> UModioOperationTracker::GetOperationTypeIconWidget_Implementation() const
{
	return nullptr;
}
