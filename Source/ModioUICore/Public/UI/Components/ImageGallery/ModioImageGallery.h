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

#include "UI/Components/ModioUIComponentBase.h"
#include "UI/EventHandlers/IModioUIMediaDownloadCompletedReceiver.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUIImageDisplayWidget.h"
#include "UI/Interfaces/IModioUIObjectSelector.h"

#include "ModioImageGallery.generated.h"

class UListView;

/**
 * Gallery of images for a mod
 */
UCLASS()
class MODIOUICORE_API UModioImageGallery : public UModioUIComponentBase,
										   public IModioUIImageDisplayWidget,
										   public IModioUIObjectSelector,
										   public IModioUIMediaDownloadCompletedReceiver
{
	GENERATED_BODY()

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Image Gallery|Data Binding",
			  meta = (BlueprintProtected))
	int64 ModIdEditor;
#endif

	/**
	 * The mod info this Gallery is meant to display
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io|UI|Image Gallery", meta = (BlueprintProtected))
	FModioModInfo BoundModInfo;

	/**
	 * Whether it displays the logo only or the gallery. Set to true if the mod has no gallery images
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io|UI|Image Gallery", meta = (BlueprintProtected))
	bool bUseLogoOnly;

	/**
	 * Refreshes the image gallery with the current mod info (reloads images)
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Image Gallery", meta = (BlueprintProtected))
	void Refresh();

	/**
	 * Gets the widget that displays the image for the gallery in full resolution
	 * @return The image widget which implements IModioUIImageDisplayWidget
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Image Gallery|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIImageDisplayWidget> GetImageWidget() const;

	/**
	 * Gets the widget that allows the user to select between images
	 * @return The widget which implements IModioUIObjectSelector
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Image Gallery|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIObjectSelector> GetImageSelectorWidget() const;

	/**
	 * Gets the widget that goes to the previous image
	 * @return The widget which implements IModioUIClickableWidget
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Image Gallery|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetDecrementSelectionButtonWidget() const;

	/**
	 * Gets the widget that goes to the next image
	 * @return The widget which implements IModioUIClickableWidget
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Image Gallery|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetIncrementSelectionButtonWidget() const;

	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativeSetDataSource(UObject* InDataSource) override;
	//~ End IModioUIDataSourceWidget Interface

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

	//~ Begin IModioUIMediaDownloadCompletedReceiver Interface
	virtual void NativeOnModGalleryImageDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode,
														  int32 ImageIndex,
														  TOptional<FModioImageWrapper> Image) override;
	virtual void NativeOnModLogoDownloadCompleted(FModioModID ModID, FModioErrorCode ErrorCode,
												  TOptional<FModioImageWrapper> Image,
												  EModioLogoSize LogoSize) override;
	//~ End IModioUIMediaDownloadCompletedReceiver Interface
};
