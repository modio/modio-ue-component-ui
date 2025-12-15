/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Editor/ComponentTestWidgets/SModioUIHoverableTestWidget.h"
#include "UI/Editor/ModioWeakThis.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SModioUIHoverableTestWidget)

void SModioUIHoverableTestWidget::ToggleHandler()
{
	if (TargetObject.IsValid())
	{
		if (bHandlerAdded)
		{
			IModioUIHoverableWidget::Execute_RemoveHoverStateChangedHandler(TargetObject.Get(), HoverListener);
		}
		else
		{
			IModioUIHoverableWidget::Execute_AddHoverStateChangedHandler(TargetObject.Get(), HoverListener);
		}
		bHandlerAdded = !bHandlerAdded;
	}
}

void SModioUIHoverableTestWidget::ToggleEvents()
{
	if (TargetObject.IsValid())
	{
		if (bHoverEnabled)
		{
			IModioUIHoverableWidget::Execute_DisableHoverEvents(TargetObject.Get());
		}
		else
		{
			IModioUIHoverableWidget::Execute_EnableHoverEvents(TargetObject.Get());
		}
		bHoverEnabled = !bHoverEnabled;
	}
}

void SModioUIHoverableTestWidget::Construct(const FArguments& InArgs)
{
	TargetObject = InArgs._TargetObject.Get();
	DelegateBridge = TStrongObjectPtr<UModioUIHoverableBridge>(NewObject<UModioUIHoverableBridge>());
	DelegateBridge->OnHoverStateChanged.BindSP(this, &SModioUIHoverableTestWidget::HandleHovered);
	HoverListener.BindUFunction(DelegateBridge.Get(), FName("HandleHovered"));

	IModioUIHoverableWidget::Execute_EnableHoverEvents(TargetObject.Get());

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
		.VAlign(EVerticalAlignment::VAlign_Center)
		.Padding(8,0)
		[
			SNew(STextBlock)
			.Text_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){return FText::FromString(bHovered? "Hovered!": "Not Hovered");} return FText{};})
		]

	];
	// clang-format on
};