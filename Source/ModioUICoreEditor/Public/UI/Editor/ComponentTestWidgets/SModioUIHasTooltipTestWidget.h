/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once
#include "Widgets/SCompoundWidget.h"
#include "UI/Interfaces/IModioUIHasTooltipWidget.h"

class SModioUIHasTooltipTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIHasTooltipTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TSharedPtr<SEditableTextBox> TooltipTitleText;
	TSharedPtr<SEditableTextBox> TooltipInfoText;
	TSharedPtr<SEditableTextBox> TooltipTagText;

	void Construct(const FArguments& InArgs);
};
