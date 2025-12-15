/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Editor/ComponentTestWidgets/SModioUIHasTextTestWidget.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"

void SModioUIHasTextTestWidget::SetTextOnTarget()
{
	if (InputText)
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(TargetObject.Get(), InputText->GetText());
	}
}

void SModioUIHasTextTestWidget::GetTextFromTarget()
{
	if (TargetObject.IsValid() && OutputText.IsValid())
	{
		OutputText->SetText(IModioUIHasTextWidget::Execute_GetWidgetText(TargetObject.Get()));

		// will need something here later to trigger a ForceRefresh on the details view observing the object
	}
}

void SModioUIHasTextTestWidget::Construct(const FArguments& InArgs)
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
			SNew(SButton)
			.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
			.Text(FText::FromString("Set Text"))
			.OnClicked_Lambda([this](){SetTextOnTarget(); return FReply::Handled();})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SAssignNew(InputText, SEditableTextBox)
			.MinDesiredWidth(200)
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SNew(SButton)
			.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
			.Text(FText::FromString("Get Text"))
			.OnClicked_Lambda([this](){GetTextFromTarget(); return FReply::Handled();})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SAssignNew(OutputText, STextBlock)
			.Text(FText::FromString("Widget text here"))
		]
	];
	// clang-format on
};