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
#include "UObject/NoExportTypes.h"

#include "UI/Interfaces/IModioGalleryImageUIDetails.h"
#include "Types/ModioImageWrapper.h"

#include "ModioGalleryImageUI.generated.h"

/**
 * @brief Wrapper around FModioImageWrapper when consumer needs UObject-representable data (e.g. for UMG as a list item or data source)
 */
UCLASS(BlueprintType, Category = "mod.io|UI|ModioGalleryImageUI")
class MODIOUICORE_API UModioGalleryImageUI : public UObject, public IModioGalleryImageUIDetails
{
	GENERATED_BODY()

public:

	static UModioGalleryImageUI* ConstructGalleryImage(UObject* Outer, FModioImageWrapper& Image, int32 Index);

	virtual FModioImageWrapper GetImage_Implementation() override;
	virtual int32 GetIndex_Implementation() override;

	/**
	 * @brief Underlying image data
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioGalleryImageUI")
	FModioImageWrapper UnderlyingImage;

	/**
	 * @brief Index of this image in the gallery
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioGalleryImageUI")
	int32 UnderlyingIndex;
};
