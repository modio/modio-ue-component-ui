/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Editor/ComponentTestWidgets/SModioUIHasTooltipTestWidget.h"
#include "UI/Editor/SModioToggleButton.h"

void SModioUIHasTooltipTestWidget::Construct(const FArguments& InArgs)
	{
		TargetObject = InArgs._TargetObject.Get();

		// clang-format off
		ChildSlot
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SModioToggleButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.bInitialState(true)
				.TrueText(FText::FromString("Enable Tooltip State"))
				.FalseText(FText::FromString("Disable Tooltip State"))
				.ClickedWhenTrue_Lambda([this](){IModioUIHasTooltipWidget::Execute_SetTooltipEnabledState(TargetObject.Get(), false); })
				.ClickedWhenFalse_Lambda([this](){IModioUIHasTooltipWidget::Execute_SetTooltipEnabledState(TargetObject.Get(), true); })
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Configure Tooltip"))
				.OnClicked_Lambda([this]() {
					IModioUIHasTooltipWidget::Execute_ConfigureTooltip(
						TargetObject.Get(),
						TooltipTitleText->GetText(),
						TooltipInfoText->GetText(),
						TooltipTagText->GetText());
					
					return FReply::Handled();
				})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SAssignNew(TooltipTitleText, SEditableTextBox)
				.MinDesiredWidth(200)
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SAssignNew(TooltipInfoText, SEditableTextBox)
				.MinDesiredWidth(200)
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SAssignNew(TooltipTagText, SEditableTextBox)
				.MinDesiredWidth(200)
			]
		];
		// clang-format on
	};