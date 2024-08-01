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
	if (!NewWidgetPath.ContainsWidget(EditableText.Get()))
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

void UModioDefaultEditableTextBox::SetTooltipEnabledState_Implementation(bool bNewEnabledState)
{
	SetToolTipText(bNewEnabledState ? GetToolTipText() : FText::GetEmpty());
}

void UModioDefaultEditableTextBox::ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText, const FText& TagText)
{
	SetToolTipText(TitleText);
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
		.OnTextChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, HandleOnTextChanged))
		.OnTextCommitted(BIND_UOBJECT_DELEGATE(FOnTextCommitted, HandleOnTextCommitted))
		.VirtualKeyboardType(EVirtualKeyboardType::AsKeyboardType(KeyboardType.GetValue()))
		.VirtualKeyboardOptions(VirtualKeyboardOptions)
		.VirtualKeyboardTrigger(VirtualKeyboardTrigger)
		.VirtualKeyboardDismissAction(VirtualKeyboardDismissAction)
		.Justification(Justification)
		.OverflowPolicy(OverflowPolicy);

	if (TSharedPtr<SModioEditableTextBox> ModioEditableTextBox = StaticCastSharedPtr<SModioEditableTextBox>(MyEditableTextBlock))
	{
		ModioEditableTextBox->OnFocusChanged.BindWeakLambda(this, [this](bool bIsFocused) {
			OnFocusChanged.Broadcast(bIsFocused);
		});
	}
	return MyEditableTextBlock.ToSharedRef();
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS