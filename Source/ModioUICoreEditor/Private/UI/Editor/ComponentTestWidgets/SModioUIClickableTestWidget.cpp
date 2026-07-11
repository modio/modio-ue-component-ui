/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Editor/ComponentTestWidgets/SModioUIClickableTestWidget.h"
#include "UI/Editor/ModioWeakThis.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SModioUIClickableTestWidget)

void SModioUIClickableTestWidget::ToggleHandler()
{
	if (TargetObject.IsValid())
	{
		if (bHandlerAdded)
		{
			IModioUIClickableWidget::Execute_RemoveClickedHandler(TargetObject.Get(), ClickListener);
		}
		else
		{
			IModioUIClickableWidget::Execute_AddClickedHandler(TargetObject.Get(), ClickListener);
		}
		bHandlerAdded = !bHandlerAdded;
	}
}

void SModioUIClickableTestWidget::ToggleEvents()
{
	if (TargetObject.IsValid())
	{
		if (bClickEnabled)
		{
			IModioUIClickableWidget::Execute_DisableClick(TargetObject.Get());
		}
		else
		{
			IModioUIClickableWidget::Execute_EnableClick(TargetObject.Get());
		}
		bClickEnabled = !bClickEnabled;
	}
}

void SModioUIClickableTestWidget::Construct(const FArguments& InArgs)
{
	TargetObject = InArgs._TargetObject.Get();
	DelegateBridge = TStrongObjectPtr<UModioUIClickableBridge>(NewObject<UModioUIClickableBridge>());
	DelegateBridge->OnClicked.BindSP(this, &SModioUIClickableTestWidget::HandleClick);
	ClickListener.BindUFunction(DelegateBridge.Get(), FName("HandleClick"));

	IModioUIClickableWidget::Execute_EnableClick(TargetObject.Get());

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
			.Text_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return GetClickEventButtonLabel();} return FText{};})
			.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ToggleEvents();} return FReply::Handled();})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8,0)
		[
			SNew(SButton)
			.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
			.Text(FText::FromString("Reset Clicked State"))
			.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ResetClick();} return FReply::Handled();})
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(EVerticalAlignment::VAlign_Center)
		.Padding(8,0)
		[
			SNew(STextBlock)
			.Text_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){return FText::FromString(bClicked? "Clicked!": "Not Clicked");} return FText{};})
		]

	];
	// clang-format on
};
