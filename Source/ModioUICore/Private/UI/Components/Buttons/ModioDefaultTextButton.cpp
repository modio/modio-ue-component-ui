/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Buttons/ModioDefaultTextButton.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioDefaultTextButton)

TScriptInterface<IModioUIHasTextWidget> UModioDefaultTextButton::GetLabelWidget_Implementation() const
{
	return nullptr;
}

void UModioDefaultTextButton::NativeSetText(const FText& DisplayText)
{
	LabelText = DisplayText;
	if (GetLabelWidget().GetObject() &&
		GetLabelWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIHasTextWidget::StaticClass()))
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(GetLabelWidget().GetObject(), LabelText);
	}
}

FText UModioDefaultTextButton::NativeGetText()
{
	if (GetLabelWidget().GetObject() &&
		GetLabelWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIHasTextWidget::StaticClass()))
	{
		return IModioUIHasTextWidget::Execute_GetWidgetText(GetLabelWidget().GetObject());
	}
	return {};
}

void UModioDefaultTextButton::NativeSetTextFont(FSlateFontInfo FontInfo)
{
	if (GetLabelWidget().GetObject() &&
		GetLabelWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIHasTextWidget::StaticClass()))
	{
		IModioUIHasTextWidget::Execute_SetWidgetTextFont(GetLabelWidget().GetObject(), FontInfo);
	}
}

FSlateFontInfo UModioDefaultTextButton::NativeGetTextFont()
{
	if (GetLabelWidget().GetObject() &&
		GetLabelWidget().GetObject()->GetClass()->ImplementsInterface(UModioUIHasTextWidget::StaticClass()))
	{
		return IModioUIHasTextWidget::Execute_GetWidgetTextFont(GetLabelWidget().GetObject());
	}
	return FSlateFontInfo();
}

void UModioDefaultTextButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	Execute_SetWidgetText(this, LabelText);
}
