/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Editor/ComponentTestWidgets/SModioUISelectableTestWidget.h"
#include "UI/Editor/ModioWeakThis.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SModioUISelectableTestWidget)

void SModioUISelectableTestWidget::ResetSelected()
{
	bSelected = false;
	IModioUISelectableWidget::Execute_SetSelectedState(TargetObject.Get(), false);
}

void SModioUISelectableTestWidget::ToggleHandler()
{
	if (TargetObject.IsValid())
	{
		if (bHandlerAdded)
		{
			IModioUISelectableWidget::Execute_RemoveSelectedStateChangedHandler(TargetObject.Get(),
																				OnSelectionChanged);
		}
		else
		{
			IModioUISelectableWidget::Execute_AddSelectedStateChangedHandler(TargetObject.Get(),
																			 OnSelectionChanged);
		}
		bHandlerAdded = !bHandlerAdded;
	}
}

void SModioUISelectableTestWidget::ToggleEvents()
{
	if (TargetObject.IsValid())
	{
		if (bSelectionEnabled)
		{
			IModioUISelectableWidget::Execute_SetSelectable(TargetObject.Get(), false);
		}
		else
		{
			IModioUISelectableWidget::Execute_SetSelectable(TargetObject.Get(), true);
		}
		bSelectionEnabled = !bSelectionEnabled;
	}
}

void SModioUISelectableTestWidget::ToggleToggleable()
{
	if (TargetObject.IsValid())
	{
		if (bToggleableEnabled)
		{
			IModioUISelectableWidget::Execute_SetToggleable(TargetObject.Get(), false);
		}
		else
		{
			IModioUISelectableWidget::Execute_SetToggleable(TargetObject.Get(), true);
		}
		bToggleableEnabled = !bToggleableEnabled;
	}
}

void SModioUISelectableTestWidget::Construct(const FArguments& InArgs)
{
	TargetObject = InArgs._TargetObject.Get();
	DelegateBridge = TStrongObjectPtr<UModioUISelectableBridge>(NewObject<UModioUISelectableBridge>());
	DelegateBridge->OnSelectionChanged.BindSP(this, &SModioUISelectableTestWidget::HandleSelectionChanged);
	OnSelectionChanged.BindUFunction(DelegateBridge.Get(), FName("HandleSelectionChanged"));

	IModioUISelectableWidget::Execute_SetSelectable(TargetObject.Get(), true);
	IModioUISelectableWidget::Execute_SetToggleable(TargetObject.Get(), true);

	// clang-format off
	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SNew(SButton)
			.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
			.Text_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return GetHandlerButtonLabel();} return FText{};})
			.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ToggleHandler();} return FReply::Handled();})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SNew(SButton)
			.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
			.Text_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return GetSelectionEventButtonLabel();} return FText{};})
			.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ToggleEvents();} return FReply::Handled();})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SNew(SButton)
			.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
			.Text_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return GetToggleableButtonLabel();} return FText{};})
			.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ToggleToggleable();} return FReply::Handled();})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SNew(SButton)
			.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
			.Text(FText::FromString("Reset Selected State"))
			.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ResetSelected();} return FReply::Handled();})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(EVerticalAlignment::VAlign_Center)
		.Padding(8,0)
		[
			SNew(STextBlock)
			.Text_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){return FText::FromString(bSelected? "Selected!": "Not Selected");} return FText{};})
		]

	];
	// clang-format on
};

