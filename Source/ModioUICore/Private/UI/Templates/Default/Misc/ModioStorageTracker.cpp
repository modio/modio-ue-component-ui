/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Templates/Default/Misc/ModioStorageTracker.h"

#include "Libraries/ModioSDKLibrary.h"
#include "ModioSubsystem.h"

void UModioStorageTracker::NativePreConstruct()
{
	Super::NativePreConstruct();
	IModioUIModManagementEventReceiver::Register<UModioStorageTracker>();
	RefreshData();
}
void UModioStorageTracker::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);

	switch (Event.Event)
	{
		case EModioModManagementEventType::Installed:
		case EModioModManagementEventType::Uninstalled:
		case EModioModManagementEventType::Uploaded:
		case EModioModManagementEventType::Updated:
			RefreshData();
	}
}
void UModioStorageTracker::RefreshData()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		FModioUnsigned64 TotalSize = FModioUnsigned64(0);
		TMap<FModioModID, FModioModCollectionEntry> InstalledMods = Subsystem->QuerySystemInstallations();
		for (auto& Mod : InstalledMods)
		{
			TotalSize += Mod.Value.GetSizeOnDisk();
		}
		if (UWidget* TextWidget = ModioUI::GetInterfaceWidgetChecked(GetStorageUsedTextWidget()))
		{
			IModioUIHasTextWidget::Execute_SetWidgetText(
				TextWidget,
				UModioSDKLibrary::Filesize_ToString(TotalSize.Underlying, 2, 2, EFileSizeUnit::Largest, true));
		}
	}
}
TScriptInterface<IModioUIHasTextWidget> UModioStorageTracker::GetStorageUsedTextWidget_Implementation() const
{
	return nullptr;
}