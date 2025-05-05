/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Core/ModioGalleryImageUI.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioGalleryImageUI)

UModioGalleryImageUI* UModioGalleryImageUI::ConstructGalleryImage(UObject* Outer, FModioImageWrapper& Image,
																  int32 Index)
{
	UModioGalleryImageUI* Out = NewObject<UModioGalleryImageUI>(Outer);
	Out->UnderlyingImage = Image;
	Out->UnderlyingIndex = Index;
	return Out;
}

FModioImageWrapper UModioGalleryImageUI::GetImage_Implementation()
{
	return UnderlyingImage;
}

int32 UModioGalleryImageUI::GetIndex_Implementation()
{
	return UnderlyingIndex;
}
