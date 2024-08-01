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

#include "Components/SlateWrapperTypes.h"
#include "Delegates/DelegateCombinations.h"
#include "Types/ModioImageWrapper.h"
#include "UObject/Interface.h"

#include "IModioUIImageDisplayWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FModioImageDisplayOnLoadStateChanged, UObject*, LoadContext, bool, bLoadSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioImageDisplayOnLoadStateChangedMulticast, UObject*, LoadContext, bool,
											 bLoadSuccess);

/**
 * @brief Interface for widgets that display images
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Image Display", Meta = (DocGroup = "UI", Comment = "@brief Interface for widgets that display images"))
class MODIOUICORE_API UModioUIImageDisplayWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIImageDisplayWidget : public IInterface
{
	GENERATED_BODY()
protected:
	virtual void NativeAddImageLoadEventHandler(const FModioImageDisplayOnLoadStateChanged& Handler) {}
	virtual void NativeRemoveImageLoadEventHandler(const FModioImageDisplayOnLoadStateChanged& Handler) {}
	virtual void NativeBeginLoadImageFromFile(FModioImageWrapper Image) {}
	virtual void NativeSetBrushMatchTextureSize(bool bMatchTextureSize) {}
	virtual void NativeSetDesiredBrushSize(FVector2D DesiredSize) {}
	virtual void NativeSetBrush(const FSlateBrush& NewBrush) {}
	virtual void AddImageLoadEventHandler_Implementation(const FModioImageDisplayOnLoadStateChanged& Handler)
	{
		NativeAddImageLoadEventHandler(Handler);
	}
	virtual void RemoveImageLoadEventHandler_Implementation(const FModioImageDisplayOnLoadStateChanged& Handler)
	{
		NativeRemoveImageLoadEventHandler(Handler);
	}
	virtual void BeginLoadImageFromFile_Implementation(FModioImageWrapper Image)
	{
		NativeBeginLoadImageFromFile(Image);
	}
	virtual void SetBrushMatchTextureSize_Implementation(bool bMatchTextureSize)
	{
		NativeSetBrushMatchTextureSize(bMatchTextureSize);
	}
	virtual void SetDesiredBrushSize_Implementation(FVector2D DesiredSize)
	{
		NativeSetDesiredBrushSize(DesiredSize);
	}
	virtual void SetBrushDirectly_Implementation(const FSlateBrush& NewBrush) {
		NativeSetBrush(NewBrush);
	}
public:
	/**
	 * @brief Register a delegate to receive callbacks when the load state of the implementing object changes
	 * @param Handler Delegate to invoke on load state change
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Image Display")
	void AddImageLoadEventHandler(const FModioImageDisplayOnLoadStateChanged& Handler);

	/**
	 * @brief Unregisters a delegate, preventing it from receiving image loading callbacks
	 * @param Handler Delegate to unregister
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Image Display")
	void RemoveImageLoadEventHandler(const FModioImageDisplayOnLoadStateChanged& Handler);

	/**
	 * @brief Begins loading an image from a file path
	 * @param Image Image data to load. Can be loaded asynchronously
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Image Display")
	void BeginLoadImageFromFile(FModioImageWrapper Image);

	/**
	 * @brief Sets whether the brush should match the size of the texture it displays
	 * @param bMatchTextureSize True if the brush should match the texture size, false if it should not
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Image Display")
	void SetBrushMatchTextureSize(bool bMatchTextureSize);

	/**
	 * @brief Sets the desired size of the brush
	 * @param DesiredSize Desired size of the brush
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Image Display")
	void SetDesiredBrushSize(FVector2D DesiredSize);

	/**
	 * @brief Sets the brush directly
	 * @param NewBrush The brush to set
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Image Display")
	void SetBrushDirectly(const FSlateBrush& NewBrush);
};
