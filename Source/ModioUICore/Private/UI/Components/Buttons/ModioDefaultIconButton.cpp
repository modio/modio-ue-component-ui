/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Buttons/ModioDefaultIconButton.h"

#include "UI/Components/ComponentHelpers.h"
#include "Engine/Texture2DDynamic.h"
#include "Brushes/SlateImageBrush.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioDefaultIconButton)

TScriptInterface<IModioUIImageDisplayWidget> UModioDefaultIconButton::GetImageWidget_Implementation() const
{
	return nullptr;
}

void UModioDefaultIconButton::NativeAddImageLoadEventHandler(const FModioImageDisplayOnLoadStateChanged& Handler)
{
	OnLoadStateChanged.AddUnique(Handler);
}

void UModioDefaultIconButton::NativeRemoveImageLoadEventHandler(const FModioImageDisplayOnLoadStateChanged& Handler)
{
	OnLoadStateChanged.Remove(Handler);
}

void UModioDefaultIconButton::NativeBeginLoadImageFromFile(FModioImageWrapper Image)
{
	CurrentlyLoadingImageWrapper = Image;
	CurrentlyLoadingImageWrapper->LoadAsync(FOnLoadImageDelegateFast::CreateUObject(this, &UModioDefaultIconButton::ImageLoadCompletedHandler));
}

void UModioDefaultIconButton::NativeSetBrushMatchTextureSize(bool bMatchTextureSize)
{
	if (UWidget* ImageWidget = ModioUI::GetInterfaceWidgetChecked(GetImageWidget()))
	{
		IModioUIImageDisplayWidget::Execute_SetBrushMatchTextureSize(ImageWidget, bMatchTextureSize);
	}
}

void UModioDefaultIconButton::NativeSetDesiredBrushSize(FVector2D DesiredSize)
{
	if (UWidget* ImageWidget = ModioUI::GetInterfaceWidgetChecked(GetImageWidget()))
	{
		IModioUIImageDisplayWidget::Execute_SetDesiredBrushSize(ImageWidget, DesiredSize);
	}
}

void UModioDefaultIconButton::ImageLoadCompletedHandler(UTexture2DDynamic* LoadedImage)
{
	if (LoadedImage && GetImageWidget())
	{
		if (UWidget* ImageWidget = ModioUI::GetInterfaceWidgetChecked(GetImageWidget()))
		{
			IModioUIImageDisplayWidget::Execute_SetBrushDirectly(ImageWidget, FSlateImageBrush(LoadedImage, FVector2D(LoadedImage->SizeX, LoadedImage->SizeY)));
			
		}
		OnLoadStateChanged.Broadcast(this, true);
	}
	else
	{
		OnLoadStateChanged.Broadcast(this, false);
	}
}

void UModioDefaultIconButton::NativeSetBrush(const FSlateBrush& NewBrush)
{
	if (UWidget* ImageWidget = ModioUI::GetInterfaceWidgetChecked(GetImageWidget()))
	{
		IModioUIImageDisplayWidget::Execute_SetBrushDirectly(
			ImageWidget, NewBrush);
	}
}
