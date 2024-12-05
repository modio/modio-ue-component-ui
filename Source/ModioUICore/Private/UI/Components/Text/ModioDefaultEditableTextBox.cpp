/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Components/Text/ModioDefaultEditableTextBox.h"

#include "Widgets/Input/SEditableTextBox.h"
#include "Layout/WidgetPath.h"

void SModioEditableTextBox::OnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath,
	const FFocusEvent& InFocusEvent)
{
	SEditableTextBox::OnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);
	if (NewWidgetPath.IsValid())
	{
		OnFocusChanged.ExecuteIfBound(NewWidgetPath.ContainsWidget(this));
	}
}

FString UModioDefaultEditableTextBox::NativeGatherInput()
{
	return GetText().ToString();
}

void UModioDefaultEditableTextBox::NativeSetHintText(FText InHintText)
{
	SetHintText(InHintText);
}

void UModioDefaultEditableTextBox::NativeSetInput(const FString& Input)
{
	SetText(FText::FromString(Input));
}

void UModioDefaultEditableTextBox::NativeAddTextCommittedHandler(const FModioOnTextCommitted& Handler)
{
	OnModioTextCommitted.AddUnique(Handler);
}

void UModioDefaultEditableTextBox::NativeRemoveTextCommittedHandler(const FModioOnTextCommitted& Handler)
{
	OnModioTextCommitted.Remove(Handler);
}

void UModioDefaultEditableTextBox::NativeAddTextChangedHandler(const FModioOnTextChanged& Handler)
{
	OnModioTextChanged.AddUnique(Handler);
}

void UModioDefaultEditableTextBox::NativeRemoveTextChangedHandler(const FModioOnTextChanged& Handler)
{
	OnModioTextChanged.Remove(Handler);
}

void UModioDefaultEditableTextBox::SetTooltipEnabledState_Implementation(bool bNewEnabledState)
{
	SetToolTipText(bNewEnabledState ? GetToolTipText() : FText::GetEmpty());
}

void UModioDefaultEditableTextBox::ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText, const FText& TagText)
{
	SetToolTipText(TitleText);
}

UWidget* UModioDefaultEditableTextBox::NativeGetWidgetToFocus(EUINavigation NavigationType) const
{
	return const_cast<UModioDefaultEditableTextBox*>(this);
}

void UModioDefaultEditableTextBox::NativeAddFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler)
{
	OnModioFocusPathChanged.AddUnique(Handler);
}

void UModioDefaultEditableTextBox::NativeRemoveFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler)
{
	OnModioFocusPathChanged.Remove(Handler);
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS

TSharedRef<SWidget> UModioDefaultEditableTextBox::RebuildWidget()
{
	// We need to create our own editable text block here because the default editable text block does not expose the focus changed delegate
	MyEditableTextBlock = SNew(SModioEditableTextBox)
		.Style(&WidgetStyle)
		.MinDesiredWidth(MinimumDesiredWidth)
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
		.VirtualKeyboardType(EVirtualKeyboardType::AsKeyboardType(KeyboardType.GetValue()))
		.VirtualKeyboardOptions(VirtualKeyboardOptions)
		.VirtualKeyboardTrigger(VirtualKeyboardTrigger)
		.VirtualKeyboardDismissAction(VirtualKeyboardDismissAction)
		.Justification(Justification)
		.OverflowPolicy(OverflowPolicy);

	if (TSharedPtr<SModioEditableTextBox> ModioEditableTextBox = StaticCastSharedPtr<SModioEditableTextBox>(MyEditableTextBlock))
	{
		ModioEditableTextBox->OnFocusChanged.BindWeakLambda(this, [this](bool bIsFocused) {
			OnModioFocusPathChanged.Broadcast(this, FFocusEvent(), bIsFocused);
		});
	}
	return MyEditableTextBlock.ToSharedRef();
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS