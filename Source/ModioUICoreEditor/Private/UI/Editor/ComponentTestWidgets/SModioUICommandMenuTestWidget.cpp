/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Editor/ComponentTestWidgets/SModioUICommandMenuTestWidget.h"
#include "UI/Editor/ModioWeakThis.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SModioUICommandMenuTestWidget)

void SModioUICommandMenuTestWidget::ToggleHandler()
{
	if (TargetObject.IsValid())
	{
		if (bHandlerAdded)
		{
			IModioUICommandMenu::Execute_RemoveCommandListBuilder(TargetObject.Get(), CommandListBuilderHandler);
		}
		else
		{
			IModioUICommandMenu::Execute_AddCommandListBuilder(TargetObject.Get(), CommandListBuilderHandler);
		}
		bHandlerAdded = !bHandlerAdded;
	}
}

void SModioUICommandMenuTestWidget::Construct(const FArguments& InArgs)
{
	TargetObject = InArgs._TargetObject.Get();
	DelegateBridge = TStrongObjectPtr<UModioUICommandMenuBridge>(NewObject<UModioUICommandMenuBridge>());
	DelegateBridge->OnBuild.BindSP(this, &SModioUICommandMenuTestWidget::HandleCommentListBuild);
	CommandListBuilderHandler.BindUFunction(DelegateBridge.Get(), FName("HandleCommentListBuild"));

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
		.VAlign(EVerticalAlignment::VAlign_Center)
		.Padding(8,0)
		[
			SNew(STextBlock)
			.Text_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){return FText::FromString(bBuilt? "List Built!": "List Not Built");} return FText{};})
		]

	];
	// clang-format on
};
