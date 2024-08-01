/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Templates/Default/FilterPanel/ModioFilterPanel.h"
#include "Engine.h"

#include "ModioSubsystem.h"

void UModioFilterPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		FOnGetModTagOptionsDelegateFast GetModTagOptionsCallback;
		GetModTagOptionsCallback.BindUObject(this, &UModioFilterPanel::SetModTagOptions);
		Subsystem->GetModTagOptionsAsync(GetModTagOptionsCallback);
	}
}

void UModioFilterPanel::SetModTagOptions(FModioErrorCode ErrorCode, TOptional<FModioModTagOptions> ModTagOptions)
{
	if (ErrorCode || !ModTagOptions.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("Bad response requesting mod tag options: %s"), *ErrorCode.GetErrorMessage());
		return;
	}
	if (GetTagSelectorWidget())
	{
		IModioUIModTagSelector::Execute_SetAvailableTagsFromModTagOptions(GetTagSelectorWidget().GetObject(),
																		  *ModTagOptions);
	}
}

void UModioFilterPanel::AddTagSelectionChangedHandler_Implementation(const FModioOnTagSelectionChanged& Handler) 
{
	if (ModioUI::GetInterfaceWidgetChecked(GetTagSelectorWidget()))
	{
		IModioUIModTagSelector::Execute_AddTagSelectionChangedHandler(GetTagSelectorWidget().GetObject(), Handler);
	}
}

TArray<FString> UModioFilterPanel::GetSelectedTags_Implementation()
{
	if (ModioUI::GetInterfaceWidgetChecked(GetTagSelectorWidget()))
	{
		return IModioUIModTagSelector::Execute_GetSelectedTags(GetTagSelectorWidget().GetObject());
	}
	return {};
}

void UModioFilterPanel::ClearSelectedTags_Implementation()
{
	if (ModioUI::GetInterfaceWidgetChecked(GetTagSelectorWidget()))
	{
		IModioUIModTagSelector::Execute_ClearSelectedTags(GetTagSelectorWidget().GetObject());
	}
}
