/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/ImageGallery/ModioDefaultImage.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioDefaultImage)

void UModioDefaultImage::NativeAddImageLoadEventHandler(const FModioImageDisplayOnLoadStateChanged& Handler)
{
	OnLoadStateChanged.AddUnique(Handler);
}

void UModioDefaultImage::NativeRemoveImageLoadEventHandler(const FModioImageDisplayOnLoadStateChanged& Handler)
{
	OnLoadStateChanged.Remove(Handler);
}

void UModioDefaultImage::NativeBeginLoadImageFromFile(FModioImageWrapper Image)
{
	Image.LoadAsync(FOnLoadImageDelegateFast::CreateUObject(this, &UModioDefaultImage::ImageLoadCompletedHandler));
}

void UModioDefaultImage::ImageLoadCompletedHandler(class UTexture2DDynamic* LoadedImage)
{
	if (LoadedImage)
	{
		SetBrushFromTextureDynamic(LoadedImage, bShouldMatchTextureSize);
		OnLoadStateChanged.Broadcast(this, true);
	}
	else
	{
		OnLoadStateChanged.Broadcast(this, false);
	}
}

void UModioDefaultImage::NativeSetBrush(const FSlateBrush& NewBrush)
{
	SetBrush(NewBrush);
}

void UModioDefaultImage::SetTooltipEnabledState_Implementation(bool bNewEnabledState)
{
	SetToolTipText(bNewEnabledState ? GetToolTipText() : FText::GetEmpty());
}

void UModioDefaultImage::ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText, const FText& TagText)
{
	SetToolTipText(TitleText);
}

void UModioDefaultImage::NativeSetBrushMatchTextureSize(bool bMatchTextureSize)
{
	bShouldMatchTextureSize = bMatchTextureSize;
}

void UModioDefaultImage::NativeSetDesiredBrushSize(FVector2D DesiredSize)
{
	SetDesiredSizeOverride(DesiredSize);
}

void UModioDefaultImage::ConfigureWidgetColorDirectly_Implementation(FName ColorName, FSlateColor ColorValue)
{
	SetColorAndOpacity(ColorValue.GetSpecifiedColor());
}

void UModioDefaultImage::ConfigureWidgetColor_Implementation(FName ColorName, FLinearColorRef ColorValue)
{
	bool bWasColorResolved = false;
	FLinearColor ResolvedColor = ULinearColorRefLibrary::Resolve(ColorValue, bWasColorResolved);
	if (!bWasColorResolved)
	{
		UE_LOG(ModioUICore, Display, TEXT("Failed to resolve color reference %s, using fallback"),
			   *ColorName.ToString());
	}
	SetColorAndOpacity(ResolvedColor);
}
