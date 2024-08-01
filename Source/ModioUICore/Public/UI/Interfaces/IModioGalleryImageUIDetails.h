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

#include "UObject/Interface.h"

#include "Types/ModioImageWrapper.h"

#include "IModioGalleryImageUIDetails.generated.h"

/**
 * @brief Data binding interface indicating that the implementing object can be queried for an associated FModioImageWrapper and index
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding|Gallery Image",
		   Meta = (DocGroup = "Data Binding Interfaces"))
class UModioGalleryImageUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioGalleryImageUIDetails : public IInterface
{
	GENERATED_BODY()

	virtual FModioImageWrapper GetImage_Implementation()
	{
		return FModioImageWrapper();
	}
	virtual int32 GetIndex_Implementation()
	{
		return -1;
	}

public:
	/**
	 * @brief Queries the implementing object for the image wrapper that the object represents
	 * @return FModioImageWrapper associated with the object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Gallery Image")
	FModioImageWrapper GetImage();

	/**
	 * @brief Queries the implementing object for the image index that the object represents
	 * @return Index associated with the object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Gallery Image")
	int32 GetIndex();
};
