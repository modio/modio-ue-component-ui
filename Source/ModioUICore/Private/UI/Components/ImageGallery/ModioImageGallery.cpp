/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/ImageGallery/ModioImageGallery.h"

#include "Components/ListView.h"

#include "Core/ModioGalleryImageUI.h"
#include "ModioUISubsystem.h"
#include "UI/Components/ImageGallery/ModioGalleryListEntry.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Interfaces/IModioUIImageDisplayWidget.h"

void UModioImageGallery::NativePreConstruct()
{
	Super::NativePreConstruct();

	IModioUIMediaDownloadCompletedReceiver::Register<UModioImageGallery>(
		EModioUIMediaDownloadEventType::ModGalleryImages);
	IModioUIMediaDownloadCompletedReceiver::Register<UModioImageGallery>(EModioUIMediaDownloadEventType::ModLogo);
}

void UModioImageGallery::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);

	if (InDataSource && InDataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
	{
		BoundModInfo = IModioModInfoUIDetails::Execute_GetFullModInfo(InDataSource);
		bUseLogoOnly = (BoundModInfo.NumGalleryImages == 0);
	}
	else
	{
		BoundModInfo = {};
		bUseLogoOnly = true;
	}
	Refresh();
}

void UModioImageGallery::Refresh()
{
	if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (bUseLogoOnly)
		{
			UISubsystem->RequestLogoDownloadForModID(BoundModInfo.ModId, EModioLogoSize::Thumb1280);
			return;
		}
		if (BoundModInfo.NumGalleryImages == 1)
		{
			UISubsystem->RequestGalleryImageDownloadForModID(BoundModInfo.ModId, 0, EModioGallerySize::Thumb1280);
			return;
		}
		if (UWidget* SelectorWidget = ModioUI::GetInterfaceWidgetChecked(GetImageSelectorWidget()))
		{
			IModioUIObjectSelector::Execute_SetValues(SelectorWidget, TArray<UObject*>());
			TArray<UObject*> SelectorEntries;
			for (int i = 0; i < BoundModInfo.NumGalleryImages; i++)
			{
				UModioGalleryListEntry* Entry = NewObject<UModioGalleryListEntry>();
				SelectorEntries.Add(Entry);
			}
			IModioUIObjectSelector::Execute_SetValues(SelectorWidget, SelectorEntries);
			IModioUIObjectSelector::Execute_SetSingleSelectionByIndex(SelectorWidget, 0, true);
		}
	}
}

TScriptInterface<IModioUIImageDisplayWidget> UModioImageGallery::GetImageWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIObjectSelector> UModioImageGallery::GetImageSelectorWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIClickableWidget> UModioImageGallery::GetDecrementSelectionButtonWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIClickableWidget> UModioImageGallery::GetIncrementSelectionButtonWidget_Implementation() const
{
	return nullptr;
}

void UModioImageGallery::NativeOnModGalleryImageDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode,
																  int32 ImageIndex, TOptional<FModioImageWrapper> Image)
{
	IModioUIMediaDownloadCompletedReceiver::NativeOnModGalleryImageDownloadCompleted(ModID, ErrorCode, ImageIndex,
																					 Image);
	if (ModID != BoundModInfo.ModId || bUseLogoOnly)
	{
		return;
	}
	if (ErrorCode || !Image.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("Get bad response requesting image: %s"), *ErrorCode.GetErrorMessage());
		return;
	}
	if (ModioUI::GetInterfaceWidgetChecked(GetImageWidget()))
	{
		IModioUIImageDisplayWidget::Execute_BeginLoadImageFromFile(GetImageWidget().GetObject(), Image.GetValue());
	}
}

void UModioImageGallery::NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode,
														  TOptional<FModioImageWrapper> Image, EModioLogoSize LogoSize)
{
	IModioUIMediaDownloadCompletedReceiver::NativeOnModLogoDownloadCompleted(ModID, ErrorCode, Image, LogoSize);

	if (ModID != BoundModInfo.ModId || !bUseLogoOnly)
	{
		return;
	}
	if (ErrorCode || !Image.IsSet())
	{
		UE_LOG(LogTemp, Error, TEXT("Get bad response requesting image: %s"), *ErrorCode.GetErrorMessage());
		return;
	}
	if (ModioUI::GetInterfaceWidgetChecked(GetImageWidget()))
	{
		IModioUIImageDisplayWidget::Execute_BeginLoadImageFromFile(GetImageWidget().GetObject(), Image.GetValue());
	}
}
