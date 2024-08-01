/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Components/Image.h"
#include "CoreMinimal.h"
#include "UI/Components/ComponentHelpers.h"
#include "UI/Interfaces/IModioUIHasTooltipWidget.h"
#include "UI/Interfaces/IModioUIHasConfigurableColor.h"
#include "UI/Interfaces/IModioUIImageDisplayWidget.h"

#include "ModioDefaultImage.generated.h"

/**
 * @default_impl_for Image
 * @brief The Image widget is a default implementation of the Image component, which uses UImage for the underlying implementation.
 * @component_display_name Image
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioDefaultImage : public UImage,
										   public IModioUIImageDisplayWidget,
										   public IModioUIHasTooltipWidget,
										   public IModioUIHasConfigurableColor
{
	GENERATED_BODY()

protected:
	bool bShouldMatchTextureSize = false;

	//~ Begin IModioUIImageDisplayWidget Interface
	virtual void NativeAddImageLoadEventHandler(const FModioImageDisplayOnLoadStateChanged& Handler) override;
	virtual void NativeRemoveImageLoadEventHandler(const FModioImageDisplayOnLoadStateChanged& Handler) override;
	virtual void NativeBeginLoadImageFromFile(FModioImageWrapper Image) override;
	virtual void NativeSetBrushMatchTextureSize(bool bMatchTextureSize) override;
	virtual void NativeSetDesiredBrushSize(FVector2D DesiredSize) override;
	virtual void ImageLoadCompletedHandler(class UTexture2DDynamic* LoadedImage);
	virtual void NativeSetBrush(const FSlateBrush& NewBrush) override;
	//~ End IModioUIImageDisplayWidget Interface

	//~ Begin IModioUIHasTooltipWidget Interface
	virtual void SetTooltipEnabledState_Implementation(bool bNewEnabledState) override;
	virtual void ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText,
												 const FText& TagText) override;
	//~ End IModioUIHasTooltipWidget Interface

	//~ Begin IModioUIHasConfigurableColor Interface
	virtual void ConfigureWidgetColorDirectly_Implementation(FName ColorName, FSlateColor ColorValue) override;
	virtual void ConfigureWidgetColor_Implementation(FName ColorName, FLinearColorRef ColorValue) override;
	//~ End IModioUIHasConfigurableColor Interface

	FModioImageDisplayOnLoadStateChangedMulticast OnLoadStateChanged;

	MODIO_WIDGET_PALETTE_OVERRIDE();
};
