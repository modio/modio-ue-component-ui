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

#include "CoreMinimal.h"
#include "UI/Components/Buttons/ModioDefaultTextButton.h"
#include "UI/Interfaces/IModioUIImageDisplayWidget.h"

#include "ModioDefaultIconButton.generated.h"

class UImage;

/**
 * @default_impl_for Button
 * @brief The Icon Button is a default implementation of the button component, which extends Text Button and adds
 * support for an image to be displayed on the button at runtime. The default blueprint implementation implements
 * IModioUIImageDisplayWidget to allow the image to be set at runtime, and forwards those interface messages onto an
 * Image sub-component which actually displays the image as an icon beside the button label.
 * @component_display_name Icon Button
 * @required_interface IModioUIImageDisplayWidget|Allows the widget to receive messages regarding displaying an image
 */
UCLASS()
class MODIOUICORE_API UModioDefaultIconButton : public UModioDefaultTextButton, public IModioUIImageDisplayWidget
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Gets a reference to the sub-component responsible for displaying the image. The default Blueprint
	 * implementation will forward IModioUIImageDisplayWidget messages to this widget if it is valid.
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Icon Button|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIImageDisplayWidget> GetImageWidget() const;

	TOptional<FModioImageWrapper> CurrentlyLoadingImageWrapper;

	/**
	 * @brief Passes `this` as LoadContext, `bLoadSuccess` indicates if loading succeeded
	 * @default_component_event FModioImageDisplayOnLoadStateChanged
	 */
	UPROPERTY(meta = (BlueprintProtected))
	FModioImageDisplayOnLoadStateChangedMulticast OnLoadStateChanged;

	//~ Begin IModioUIImageDisplayWidget Interface
	virtual void NativeAddImageLoadEventHandler(const FModioImageDisplayOnLoadStateChanged& Handler) override;
	virtual void NativeRemoveImageLoadEventHandler(const FModioImageDisplayOnLoadStateChanged& Handler) override;
	virtual void NativeBeginLoadImageFromFile(FModioImageWrapper Image) override;
	virtual void NativeSetBrushMatchTextureSize(bool bMatchTextureSize) override;
	virtual void NativeSetDesiredBrushSize(FVector2D DesiredSize) override;
	virtual void ImageLoadCompletedHandler(class UTexture2DDynamic* LoadedImage);
	virtual void NativeSetBrush(const FSlateBrush& NewBrush) override;
	//~ End IModioUIImageDisplayWidget Interface
};
