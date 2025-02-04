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

#include "Internationalization/Text.h"
#include "Libraries/ModioSDKLibrary.h"
#include "TimerManager.h"
#include "Types/ModioUnsigned64.h"

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
		if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
		{
			if (TOptional<FModioModProgressInfo> ProgressInfo = Subsystem->QueryCurrentModUpdate())
			{
				RefreshSizeAndSpeedText(ProgressInfo.GetValue(), InDeltaTime);

				if (UWidget* RadialProgressWidget = ModioUI::GetInterfaceWidgetChecked(GetRadialProgressWidget()))
				{
					IModioUIProgressWidget::Execute_SetProgress(RadialProgressWidget,
																GetInstallProgressPercent(ProgressInfo.GetValue()));
				}
				if (UWidget* PercentProgressTextWidget =
						ModioUI::GetInterfaceWidgetChecked(GetPercentProgressTextWidget()))
				{
					IModioUIHasTextWidget::Execute_SetWidgetText(
						PercentProgressTextWidget,
						FText::AsPercent(GetInstallProgressPercent(ProgressInfo.GetValue())));
				}
			}
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
			HandleOperationBegin();
			break;
		case EModioModManagementEventType::Installed:
		case EModioModManagementEventType::Updated:
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
	if (UWidget* ProgressText = ModioUI::GetInterfaceWidgetChecked(GetPercentProgressTextWidget()))
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(ProgressText, FText::AsPercent(0.f));
	}
	if (UWidget* OperationTypeIcon = ModioUI::GetInterfaceWidgetChecked(GetOperationTypeIconWidget()))
	{
		IModioUIImageDisplayWidget::Execute_SetBrushDirectly(OperationTypeIcon, FSlateNoResource());
	}
	if (UWidget* DownloadSpeedTextWidget = ModioUI::GetInterfaceWidgetChecked(GetDownloadSpeedTextWidget()))
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(DownloadSpeedTextWidget, FText());
	}
	if (UWidget* DownloadSizeTextWidget = ModioUI::GetInterfaceWidgetChecked(GetDownloadSizeTextWidget()))
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(DownloadSizeTextWidget, FText());
	}
}
void UModioOperationTracker::HandleOperationBegin()
{
	bPollInstallProgress = true;
	bPollSpeed = true;

	DownloadProgressSize = FModioUnsigned64(0);
	DownloadTotalSize = FModioUnsigned64(0);
	TimeElapsed = 0.0f;
	SizePerSecond = FModioUnsigned64(0);

	SetInitialVisualState();
	ShowSubWidgets();
}
void UModioOperationTracker::HandleOperationComplete_Implementation()
{
	bPollInstallProgress = false;
	bPollSpeed = false;

	// Some installations (e.g. small or fast) may skip setting the final bit of progress
	// The below acts as a fallback ensuring we always display a complete installation state
	if (UWidget* RadialProgress = ModioUI::GetInterfaceWidgetChecked(GetRadialProgressWidget()))
	{
		IModioUIProgressWidget::Execute_SetProgress(RadialProgress, 1.0f);
	}
	if (UWidget* ProgressTextWidget = ModioUI::GetInterfaceWidgetChecked(GetPercentProgressTextWidget()))
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(ProgressTextWidget, FText::AsPercent(1.0f));
	}
	if (DownloadProgressSize < DownloadTotalSize)
	{
		DownloadProgressSize = DownloadTotalSize;
		SetDownloadSizeText();
	}
}
void UModioOperationTracker::OnAllOperationsComplete()
{
	HideSubWidgets();
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
}
void UModioOperationTracker::RefreshSizeAndSpeedText(FModioModProgressInfo ProgressInfo, float InDeltaTime)
{
	if (ProgressInfo.GetCurrentState() == EModioModProgressState::Downloading)
	{
		FModioUnsigned64 SizeLastTick = DownloadProgressSize;
		DownloadTotalSize = ProgressInfo.GetTotalProgress(EModioModProgressState::Downloading);
		DownloadProgressSize = ProgressInfo.GetCurrentProgress(EModioModProgressState::Downloading);
		SetDownloadSizeText();

		if (bPollSpeed)
		{
			SizePerSecond += DownloadProgressSize - SizeLastTick;
			TimeElapsed += InDeltaTime;

			if (TimeElapsed >= 1.0f)
			{
				if (UWidget* DownloadSpeedTextWidget = ModioUI::GetInterfaceWidgetChecked(GetDownloadSpeedTextWidget()))
				{
					IModioUIHasTextWidget::Execute_SetWidgetText(
						DownloadSpeedTextWidget,
						FText::Format(FTextFormat(SpeedFormatText),
									  UModioSDKLibrary::Filesize_ToString(SizePerSecond.Underlying, 2, 2,
																		  EFileSizeUnit::Largest, true)));
				}
				SizePerSecond = FModioUnsigned64(0);
				TimeElapsed = 0;
			}
		}
		return;
	}
	// if bPollSpeed is still true, we have just transistioned from Downloading to Extracting
	if (ProgressInfo.GetCurrentState() == EModioModProgressState::Extracting && bPollSpeed)
	{
		// No longer installing, so download speed should be cleared
		if (UWidget* DownloadSpeedTextWidget = ModioUI::GetInterfaceWidgetChecked(GetDownloadSpeedTextWidget()))
		{
			IModioUIHasTextWidget::Execute_SetWidgetText(DownloadSpeedTextWidget, FText());
		}
		// Ensure the download size represents a complete state
		if (DownloadProgressSize < DownloadTotalSize)
		{
			DownloadProgressSize = DownloadTotalSize;
			SetDownloadSizeText();
		}
		// Ensure we don't enter this block of code again until a new operation begins
		bPollSpeed = false;
	}
}
float UModioOperationTracker::GetInstallProgressPercent(FModioModProgressInfo ProgressInfo)
{
	switch (ProgressInfo.GetCurrentState())
	{
		case EModioModProgressState::Downloading:
			return UModioUnsigned64Library::DivideToFloat(
				ProgressInfo.GetCurrentProgress(EModioModProgressState::Downloading),
				ProgressInfo.GetTotalProgress(EModioModProgressState::Downloading));
			break;

		case EModioModProgressState::Extracting:
			return UModioUnsigned64Library::DivideToFloat(
				ProgressInfo.GetCurrentProgress(EModioModProgressState::Extracting),
				ProgressInfo.GetTotalProgress(EModioModProgressState::Extracting));
			break;
	}
	return 0;
}
void UModioOperationTracker::SetDownloadSizeText()
{
	if (UWidget* DownloadSizeWidget = ModioUI::GetInterfaceWidgetChecked(GetDownloadSizeTextWidget()))
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(
			DownloadSizeWidget, FText::Format(FTextFormat(SizeFormatText),
											  UModioSDKLibrary::Filesize_ToString(DownloadProgressSize.Underlying, 2, 2,
																				  EFileSizeUnit::Largest, true),
											  UModioSDKLibrary::Filesize_ToString(DownloadTotalSize.Underlying, 2, 2,
																				  EFileSizeUnit::Largest, true)));
	}
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
	if (UWidget* ProgressTextWidget = ModioUI::GetInterfaceWidgetChecked(GetPercentProgressTextWidget()))
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
	if (UWidget* ModCountTextWidget = ModioUI::GetInterfaceWidgetChecked(GetQueueLabelTextWidget()))
	{
		ModCountTextWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (UWidget* OperationIconWidget = ModioUI::GetInterfaceWidgetChecked(GetOperationTypeIconWidget()))
	{
		OperationIconWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (UWidget* DownloadSizeTextWidget = ModioUI::GetInterfaceWidgetChecked(GetDownloadSizeTextWidget()))
	{
		DownloadSizeTextWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	if (UWidget* DownloadSpeedTextWidget = ModioUI::GetInterfaceWidgetChecked(GetDownloadSpeedTextWidget()))
	{
		DownloadSpeedTextWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
void UModioOperationTracker::ShowSubWidgets()
{
	if (UWidget* ProgressTextWidget = ModioUI::GetInterfaceWidgetChecked(GetPercentProgressTextWidget()))
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
	if (UWidget* ModCountTextWidget = ModioUI::GetInterfaceWidgetChecked(GetQueueLabelTextWidget()))
	{
		ModCountTextWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (UWidget* OperationIconWidget = ModioUI::GetInterfaceWidgetChecked(GetOperationTypeIconWidget()))
	{
		OperationIconWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (UWidget* DownloadSizeTextWidget = ModioUI::GetInterfaceWidgetChecked(GetDownloadSizeTextWidget()))
	{
		DownloadSizeTextWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (UWidget* DownloadSpeedTextWidget = ModioUI::GetInterfaceWidgetChecked(GetDownloadSpeedTextWidget()))
	{
		DownloadSpeedTextWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}
TScriptInterface<IModioUIHasTextWidget> UModioOperationTracker::GetPercentProgressTextWidget_Implementation() const
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
TScriptInterface<IModioUIHasTextWidget> UModioOperationTracker::GetQueueLabelTextWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIImageDisplayWidget> UModioOperationTracker::GetOperationTypeIconWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIHasTextWidget> UModioOperationTracker::GetDownloadSizeTextWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIHasTextWidget> UModioOperationTracker::GetDownloadSpeedTextWidget_Implementation() const
{
	return nullptr;
}