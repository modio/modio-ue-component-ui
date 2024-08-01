/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioModEnabledToggleWidget.h"

#include "UI/Interfaces/IModioModInfoUIDetails.h"

void UModioModEnabledToggleWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	// Register for events so this widget receives a callback when a mod is enabled or disabled
	IModioUIModEnabledStateChangedReceiver::Register<UModioModEnabledToggleWidget>();
}

void UModioModEnabledToggleWidget::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	//Once we have set the data source, if it's valid, query it for the associated mod ID
	if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			FModioModID RelevantModID = IModioModInfoUIDetails::Execute_GetModID(DataSource);
			// Query the subsystem to find out if the mod is currently enabled or not
			bool bModEnabled = UISubsystem->QueryIsModEnabled(RelevantModID);
			// Notify the widget of the initial enabled state to display
			IModioUIModEnabledStateChangedReceiver::Execute_OnModEnabledStateChanged(this, RelevantModID, bModEnabled);
		}
	}
}