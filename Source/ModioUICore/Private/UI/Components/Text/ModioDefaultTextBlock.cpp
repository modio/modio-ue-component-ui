/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Text/ModioDefaultTextBlock.h"
#include "UI/Components/ComponentHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioDefaultTextBlock)

void UModioDefaultTextBlock::NativeSetText(const FText& DisplayText)
{
	SetText(DisplayText);
	SynchronizeProperties();
}

FText UModioDefaultTextBlock::NativeGetText()
{
	return GetText();
}

void UModioDefaultTextBlock::NativeSetTextFont(FSlateFontInfo FontInfo)
{
	SetFont(FontInfo);
	SynchronizeProperties();
}

FSlateFontInfo UModioDefaultTextBlock::NativeGetTextFont()
{
	return GetFont();
}

void UModioDefaultTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	ApplyColorOverrideImpl();
}

void UModioDefaultTextBlock::ApplyColorOverrideImpl()
{
	bool bTextColorOverrideSet = false;
	FLinearColor ResolvedColor = ULinearColorRefLibrary::ResolveOverride(TextColorOverride, bTextColorOverrideSet);
	if (bTextColorOverrideSet)
	{
		SetColorAndOpacity(ResolvedColor);
	}
}

void UModioDefaultTextBlock::ConfigureWidgetColor_Implementation(FName ColorName, FLinearColorRef ColorValue)
{
	TextColorOverride.bOverride = true;
	TextColorOverride.Color = ColorValue;
	ApplyColorOverrideImpl();
}

void UModioDefaultTextBlock::ConfigureWidgetColorDirectly_Implementation(FName ColorName, FSlateColor ColorValue)
{
	MODIO_INTERFACE_UNIMPLEMENTED_FUNCTION_MSG("Use ConfigureWidgetColor with a color reference instead");
}

void UModioDefaultTextBlock::SetTooltipEnabledState_Implementation(bool bNewEnabledState)
{
	SetToolTipText(bNewEnabledState ? GetToolTipText() : FText::GetEmpty());
}

void UModioDefaultTextBlock::ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText,
															 const FText& TagText)
{
	SetToolTipText(TitleText);
}
