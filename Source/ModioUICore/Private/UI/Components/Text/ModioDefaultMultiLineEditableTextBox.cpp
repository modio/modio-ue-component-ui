/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Text/ModioDefaultMultiLineEditableTextBox.h"

#include "Widgets/Input/SMultiLineEditableTextBox.h"

FReply SModioCommonMultiLineEditableTextBox::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Reply = SMultiLineEditableTextBox::OnKeyDown(MyGeometry, InKeyEvent);
	if (Reply.IsEventHandled())
	{
		return Reply;
	}
	// SWidget::OnKeyDown will handle navigation with gamepad by FReply::Handled().SetNavigation if needed
	return SWidget::OnKeyDown(MyGeometry, InKeyEvent);
}

FString UModioDefaultMultiLineEditableTextBox::NativeGatherInput()
{
	return GetText().ToString();
}

void UModioDefaultMultiLineEditableTextBox::NativeSetHintText(FText InHintText)
{
	SetHintText(InHintText);
}

void UModioDefaultMultiLineEditableTextBox::NativeSetInput(const FString& Input)
{
	SetText(FText::FromString(Input));
}

void UModioDefaultMultiLineEditableTextBox::NativeAddTextCommittedHandler(const FModioOnTextCommitted& Handler)
{
	OnModioTextCommitted.AddUnique(Handler);
}

void UModioDefaultMultiLineEditableTextBox::NativeRemoveTextCommittedHandler(const FModioOnTextCommitted& Handler)
{
	OnModioTextCommitted.Remove(Handler);
}

void UModioDefaultMultiLineEditableTextBox::NativeAddTextChangedHandler(const FModioOnTextChanged& Handler)
{
	OnModioTextChanged.AddUnique(Handler);
}

void UModioDefaultMultiLineEditableTextBox::NativeRemoveTextChangedHandler(const FModioOnTextChanged& Handler)
{
	OnModioTextChanged.Remove(Handler);
}

void UModioDefaultMultiLineEditableTextBox::SetTooltipEnabledState_Implementation(bool bNewEnabledState)
{
	SetToolTipText(bNewEnabledState ? GetToolTipText() : FText::GetEmpty());
}

void UModioDefaultMultiLineEditableTextBox::ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText, const FText& TagText)
{
	SetToolTipText(TitleText);
}

TSharedRef<SWidget> UModioDefaultMultiLineEditableTextBox::RebuildWidget()
{
	// Create MyEditableTextBlock manually to pass extra parameters (such as VirtualKeyboardTrigger for consoles to show the virtual keyboard)
	// And to support navigation with gamepad
	MyEditableTextBlock = SNew(SModioCommonMultiLineEditableTextBox)
		.Style(&WidgetStyle)
		.IsCaretMovedWhenGainFocus(IsCaretMovedWhenGainFocus)
		.SelectAllTextWhenFocused(SelectAllTextWhenFocused)
		.RevertTextOnEscape(RevertTextOnEscape)
		.ClearKeyboardFocusOnCommit(ClearKeyboardFocusOnCommit)
		.SelectAllTextOnCommit(SelectAllTextOnCommit)
		.AllowContextMenu(AllowContextMenu)
		.OnTextChanged(FOnTextChanged::CreateWeakLambda(this, [this](const FText& InText)
		{
			HandleOnTextChanged(InText);
			OnModioTextChanged.Broadcast(this, InText);
		}))
		.OnTextCommitted(FOnTextCommitted::CreateWeakLambda(this, [this](const FText& InText, ETextCommit::Type CommitMethod)
		{
			HandleOnTextCommitted(InText, CommitMethod);
			OnModioTextCommitted.Broadcast(this, InText, CommitMethod);
		}))
		//.VirtualKeyboardType(EVirtualKeyboardType::AsKeyboardType(KeyboardType.GetValue()))
		.VirtualKeyboardOptions(VirtualKeyboardOptions)
		.VirtualKeyboardTrigger(VirtualKeyboardTrigger)
		.VirtualKeyboardDismissAction(VirtualKeyboardDismissAction)
		.Justification(Justification)
		.OverflowPolicy(OverflowPolicy)
		.AlwaysShowScrollbars(AlwaysShowScrollbars)
		.AllowMultiLine(AllowMultiLine);
	return MyEditableTextBlock.ToSharedRef();
}
