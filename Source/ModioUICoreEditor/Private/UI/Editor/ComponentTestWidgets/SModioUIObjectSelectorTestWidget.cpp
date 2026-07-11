/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Editor/ComponentTestWidgets/SModioUIObjectSelectorTestWidget.h"
#include "Widgets/Input/SNumericEntryBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SModioUIObjectSelectorTestWidget)

void SModioUIObjectSelectorTestWidget::ClearSelection()
{
	if (TargetObject.IsValid())
	{
		IModioUIObjectSelector::Execute_ClearSelectedValues(TargetObject.Get());
	}
}

void SModioUIObjectSelectorTestWidget::SetSelectionFromIndex()
{
	if (TargetObject.IsValid())
	{
		if (DesiredIndexValue >= 0)
		{
			IModioUIObjectSelector::Execute_SetSingleSelectionByIndex(TargetObject.Get(), DesiredIndexValue, true);
		}
	}
}

void SModioUIObjectSelectorTestWidget::Construct(const FArguments& InArgs)
{
	TargetObject = InArgs._TargetObject.Get();
	DelegateBridge = TStrongObjectPtr<UModioUIObjectSelectorBridge>(NewObject<UModioUIObjectSelectorBridge>());
	DelegateBridge->OnSelectionChanged.BindSP(this, &SModioUIObjectSelectorTestWidget::HandleSelectionChanged);
	SelectionListener.BindUFunction(DelegateBridge.Get(), FName("HandleSelectionChanged"));

	IModioUIObjectSelector::Execute_AddSelectionChangedHandler(TargetObject.Get(), SelectionListener);

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
			.Text(FText::FromString("Clear Selection"))
			.OnClicked_Lambda([ this](){ ClearSelection();return FReply::Handled();})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SNew(SButton)
			.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
			.Text(FText::FromString("Set Selected Index"))
			.OnClicked_Lambda([ this](){ IModioUIObjectSelector::Execute_SetSingleSelectionByIndex(TargetObject.Get(), DesiredIndexValue, true); return FReply::Handled();})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(EVerticalAlignment::VAlign_Center)
		.Padding(8,0)
		[
			SAssignNew(DesiredIndex, SNumericEntryBox<int32>)
			.Value_Lambda([this](){return DesiredIndexValue;})
			.OnValueChanged_Lambda([this](int32 NewValue){DesiredIndexValue = NewValue;})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(EVerticalAlignment::VAlign_Center)
		.Padding(8,0)
		[
			SNew(STextBlock)
			.Text_Lambda([ this](){return FText::FromString(bSelected? "Selected": "Not Selected");})
		]
	];
	// clang-format on
};