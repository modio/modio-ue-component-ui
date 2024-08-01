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
#include "Fonts/SlateFontInfo.h"
#include "UObject/Interface.h"

#include "IModioUIHasTextWidget.generated.h"

/**
 * @brief Interface indicating the implementing widget has a configurable Text
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Text",
		   Meta = (DocGroup = "UI",
				   Comment = "@brief Interface indicating the implementing widget has a configurable Text"))
class MODIOUICORE_API UModioUIHasTextWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIHasTextWidget : public IInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeSetText(const FText& DisplayText) {}

	virtual FText NativeGetText()
	{
		return FText();
	}

	virtual void NativeSetTextFont(FSlateFontInfo FontInfo) {}

	virtual FSlateFontInfo NativeGetTextFont()
	{
		return FSlateFontInfo();
	}

	virtual void SetWidgetText_Implementation(const FText& DisplayText)
	{
		NativeSetText(DisplayText);
	}

	virtual FText GetWidgetText_Implementation()
	{
		return NativeGetText();
	}

	virtual void SetWidgetTextFont_Implementation(FSlateFontInfo FontInfo)
	{
		NativeSetTextFont(FontInfo);
	}

	virtual FSlateFontInfo GetWidgetTextFont_Implementation()
	{
		return NativeGetTextFont();
	}

public:
	/**
	 * @brief Sets the value of the associated text for the implementing object
	 * @param DisplayText the text to display
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Has Text")
	void SetWidgetText(const FText& DisplayText);

	/**
	 * @brief Retrieves the value of the implementing widget's associated text
	 * @return The text value
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Has Text")
	FText GetWidgetText();

	/**
	 * @brief Retrieves the font for the implementing widget, if applicable
	 * @return The font to use
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Has Text")
	FSlateFontInfo GetWidgetTextFont();

	/**
	 * @brief Sets the font for the implementing widget, if applicable
	 * @param FontInfo the font to use
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Has Text")
	void SetWidgetTextFont(FSlateFontInfo FontInfo);
};
