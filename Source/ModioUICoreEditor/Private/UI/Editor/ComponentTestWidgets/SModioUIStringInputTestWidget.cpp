/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Editor/ComponentTestWidgets/SModioUIStringInputTestWidget.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"

void SModioUIStringInputTestWidget::SetInputOnTarget()
{
	if (InputText)
	{
		IModioUIStringInputWidget::Execute_SetInput(TargetObject.Get(), InputText->GetText().ToString());
	}
}
void SModioUIStringInputTestWidget::GatherInputFromTarget()
{
	if (TargetObject.IsValid() && OutputText.IsValid())
	{
		OutputText->SetText(FText::FromString(IModioUIStringInputWidget::Execute_GatherInput(TargetObject.Get())));

		// will need something here later to trigger a ForceRefresh on the details view observing the object
	}
}

void SModioUIStringInputTestWidget::Construct(const FArguments& InArgs)
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
			.Text(FText::FromString("Set Input"))
			.OnClicked_Lambda([this](){SetInputOnTarget(); return FReply::Handled();})
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
			.Text(FText::FromString("Gather Input"))
			.OnClicked_Lambda([this](){GatherInputFromTarget(); return FReply::Handled();})
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
}