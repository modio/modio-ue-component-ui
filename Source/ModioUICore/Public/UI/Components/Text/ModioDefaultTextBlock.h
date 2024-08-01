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

#include "Components/TextBlock.h"
#include "Core/LinearColorRef.h"
#include "CoreMinimal.h"
#include "UI/Components/ComponentHelpers.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUIHasTooltipWidget.h"
#include "UI/Interfaces/IModioUIHasConfigurableColor.h"

#include "ModioDefaultTextBlock.generated.h"

/**
 * @default_impl_for Text
 * @brief The text block is a default implementation of a Text component that supports displaying a localized piece of
 * text, which can be externally configured.
 * @component_display_name Text Block
 * @required_interface IModioUIHasTextWidget|Allows the text being displayed to be set or queried at runtime
 * @required_interface IModioUIHasConfigurableColor|Allows the text color to be configured at runtime
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioDefaultTextBlock : public UTextBlock,
											   public IModioUIHasTextWidget,
											   public IModioUIHasConfigurableColor,
											   public IModioUIHasTooltipWidget
{
	GENERATED_BODY()

protected:
	MODIO_WIDGET_PALETTE_OVERRIDE();

	TOptional<FSlateColor> OriginalColorAndOpacity;

	virtual void ApplyColorOverrideImpl();

	//~ Begin IModioUIHasTextWidget Interface
	virtual void NativeSetText(const FText& DisplayText) override;
	virtual FText NativeGetText() override;
	virtual void NativeSetTextFont(FSlateFontInfo FontInfo) override;
	virtual FSlateFontInfo NativeGetTextFont() override;
	//~ End IModioUIHasTextWidget Interface

	//~ Begin IModioUIHasTooltipWidget Interface
	virtual void SetTooltipEnabledState_Implementation(bool bNewEnabledState) override;
	virtual void ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText,
												 const FText& TagText) override;
	//~ End IModioUIHasTooltipWidget Interface

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Text Block")
	FLinearColorRefOverride TextColorOverride;

	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin IModioUIHasConfigurableColor Interface
	virtual void ConfigureWidgetColorDirectly_Implementation(FName ColorName, FSlateColor ColorValue) override;
	virtual void ConfigureWidgetColor_Implementation(FName ColorName, FLinearColorRef ColorValue) override;
	//~ End IModioUIHasConfigurableColor Interface
};
