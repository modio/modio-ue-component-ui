/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioDefaultBorder.h"
#include "ModioUICore.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioDefaultBorder)

void UModioDefaultBorder::ConfigureWidgetColorDirectly_Implementation(FName ColorName, FSlateColor ColorValue)
{
	SetBrushColor(ColorValue.GetSpecifiedColor());
}

void UModioDefaultBorder::ConfigureWidgetColor_Implementation(FName ColorName, FLinearColorRef ColorValue)
{
	bool bWasColorResolved = false;
	FLinearColor ResolvedColor = ULinearColorRefLibrary::Resolve(ColorValue, bWasColorResolved);
	if (!bWasColorResolved)
	{
		UE_LOG(ModioUICore, Display, TEXT("Failed to resolve color reference %s, using fallback"), *ColorName.ToString());
	}
	SetBrushColor(ResolvedColor);
}
