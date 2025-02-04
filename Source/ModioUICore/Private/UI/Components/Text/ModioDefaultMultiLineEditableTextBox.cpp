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
#include "Framework/Application/SlateApplication.h"

#include "Widgets/Input/SMultiLineEditableTextBox.h"

FReply SModioCommonMultiLineEditableTextBox::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (bIgnoreOnKeyDown)
	{
		return FReply::Unhandled();
	}

	const FKey Key = InKeyEvent.GetKey();

	if (Key == EKeys::Up || Key == EKeys::Gamepad_RightStick_Up || Key == EKeys::Gamepad_LeftStick_Up || Key == EKeys::Gamepad_DPad_Up)
	{
		return HandleNavigation(MyGeometry, EUINavigation::Up);
	}
	if (Key == EKeys::Down || Key == EKeys::Gamepad_RightStick_Down || Key == EKeys::Gamepad_LeftStick_Down || Key == EKeys::Gamepad_DPad_Down)
	{
		return HandleNavigation(MyGeometry, EUINavigation::Down);
	}
	if (Key == EKeys::Left || Key == EKeys::Gamepad_RightStick_Left || Key == EKeys::Gamepad_LeftStick_Left || Key == EKeys::Gamepad_DPad_Left)
	{
		return HandleNavigation(MyGeometry, EUINavigation::Left);
	}
	if (Key == EKeys::Right || Key == EKeys::Gamepad_RightStick_Right || Key == EKeys::Gamepad_LeftStick_Right || Key == EKeys::Gamepad_DPad_Right)
	{
		return HandleNavigation(MyGeometry, EUINavigation::Right);
	}

	return SMultiLineEditableTextBox::OnKeyDown(MyGeometry, InKeyEvent);
}

FReply SModioCommonMultiLineEditableTextBox::HandleNavigation(const FGeometry& MyGeometry, EUINavigation Navigation)
{
	TSharedPtr<SWidget> EditableTextWidget = StaticCastSharedPtr<SWidget>(EditableText);
	if (!EditableTextWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to handle '%s' navigation for the multi line text box: EditableText widget is null"), *UEnum::GetValueAsString(Navigation));
		return FReply::Unhandled();
	}

	UE_LOG(LogTemp, Log, TEXT("Sending '%s' navigation to the multiline editable text"), *UEnum::GetValueAsString(Navigation));
	
	FKey NavigationKey = [Navigation]()
	{
		switch (Navigation)
		{
		case EUINavigation::Previous:
		case EUINavigation::Left: return EKeys::Left;
		case EUINavigation::Next:
		case EUINavigation::Right: return EKeys::Right;
		case EUINavigation::Up: return EKeys::Up;
		case EUINavigation::Down: return EKeys::Down;
		default:
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid navigation direction '%s'"), *UEnum::GetValueAsString(Navigation));
				return EKeys::Invalid;
			}
		}
	}();
	const FTextLocation PriorCursorPosition = EditableText->GetCursorLocation();
	FKeyEvent NavigationEvent(NavigationKey, FSlateApplication::Get().GetModifierKeys(), 0, false, 0, 0);
	bIgnoreOnKeyDown = true;
	FReply HandledEvent = EditableTextWidget->OnKeyDown(MyGeometry, NavigationEvent);
	bIgnoreOnKeyDown = false;
	const FTextLocation NewCursorPosition = EditableText->GetCursorLocation();

	// If the cursor changed position, the editable text handled the event by moving the cursor
	if (PriorCursorPosition != NewCursorPosition)
	{
		return HandledEvent;
	}

	// Otherwise, simulate navigation using the gamepad to go beyond the widget's bounds
	const float AnalogValue = [Navigation]()
	{
		switch (Navigation)
		{
		case EUINavigation::Previous:
		case EUINavigation::Left: return -1.0f;
		case EUINavigation::Next:
		case EUINavigation::Right: return 1.0f;
		case EUINavigation::Up: return 1.0f;
		case EUINavigation::Down: return -1.0f;
		default:
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid navigation direction '%s'"), *UEnum::GetValueAsString(Navigation));
				return 0.0f;
			}
		}
	}();

	const FKey GamepadKey = [Navigation]()
	{
		switch (Navigation)
		{
		case EUINavigation::Previous:
		case EUINavigation::Left: return EKeys::Gamepad_LeftX;
		case EUINavigation::Next:
		case EUINavigation::Right: return EKeys::Gamepad_LeftX;
		case EUINavigation::Up: return EKeys::Gamepad_LeftY;
		case EUINavigation::Down: return EKeys::Gamepad_LeftY;
		default:
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid navigation direction '%s'"), *UEnum::GetValueAsString(Navigation));
				return EKeys::Invalid;
			}
		}
	}();
	FAnalogInputEvent GamepadEvent(GamepadKey, FSlateApplication::Get().GetModifierKeys(), 0, false, 0, 0, AnalogValue);
	FSlateApplication::Get().ProcessAnalogInputEvent(GamepadEvent);
	return FReply::Handled();
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

	MyEditableTextBlock->SetOnKeyDownHandler(FOnKeyDown::CreateWeakLambda(this, [this](const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) -> FReply
	{
		if (TSharedPtr<SWidget> WidgetToHandle = StaticCastSharedPtr<SWidget>(MyEditableTextBlock))
		{
			return WidgetToHandle->OnKeyDown(MyGeometry, InKeyEvent);
		}
		return FReply::Unhandled();
	}));

	return MyEditableTextBlock.ToSharedRef();
}
