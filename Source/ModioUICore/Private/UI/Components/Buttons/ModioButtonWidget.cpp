/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Buttons/ModioButtonWidget.h"

#include "Blueprint/WidgetTree.h"
#include "CommonActionWidget.h"
#include "Framework/Application/SlateApplication.h"
#include "Misc/EngineVersionComparison.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Templates/UnrealTemplate.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioButtonWidget)

void UModioButtonWidget::SetCommonUIButtonFocus()
{
#if UE_VERSION_OLDER_THAN(5, 2, 0)
	if (bIsFocusable)
#else
	if (IsFocusable())
#endif
	{
		if (TSharedPtr<SButton> SlateButton = GetSlateButton())
		{
			if (SlateButton->SupportsKeyboardFocus())
			{
				FSlateApplication::Get().SetKeyboardFocus(SlateButton, EFocusCause::Mouse);
				UE_LOG(ModioUICore, Verbose, TEXT("Set focus on button '%s' (extended way)"), *GetName());
			}
			else
			{
				UE_LOG(ModioUICore, Warning,
					   TEXT("Trying to set focus on button '%s' but the button does not support keyboard focus"),
					   *GetName());
			}
		}
		else
		{
			UE_LOG(ModioUICore, Warning,
				   TEXT("Trying to set focus on button '%s' but the slate button could not be found"), *GetName());
		}
	}
	else
	{
		UE_LOG(ModioUICore, Warning, TEXT("Trying to set focus on button '%s' but the button is not focusable"),
			   *GetName());
	}
}

TSharedPtr<SButton> UModioButtonWidget::GetSlateButton() const
{
	if (WidgetTree && WidgetTree->RootWidget)
	{
		if (UButton* InternalButton = Cast<UButton>(WidgetTree->RootWidget))
		{
			// UCommonButtonInternalBase::RebuildWidget() creates a SBox wrapper for the button
			if (TSharedPtr<SBox> BoxButtonWrapper =
					StaticCastSharedPtr<SBox>(TSharedPtr<SWidget>(InternalButton->GetCachedWidget())))
			{
				if (BoxButtonWrapper->GetChildren() && BoxButtonWrapper->GetChildren()->Num() > 0)
				{
					if (TSharedPtr<SButton> InternalButtonSlate = StaticCastSharedPtr<SButton>(
							TSharedPtr<SWidget>(BoxButtonWrapper->GetChildren()->GetChildAt(0))))
					{
						return InternalButtonSlate;
					}
				}
			}
			// UButton::RebuildWidget() returns the button directly
			else if (TSharedPtr<SButton> InternalButtonSlate =
						 StaticCastSharedPtr<SButton>(InternalButton->GetCachedWidget()))
			{
				return InternalButtonSlate;
			}
			else
			{
				UE_LOG(ModioUICore, Error, TEXT("Could not find the Slate button widget for button '%s'"), *GetName());
			}
		}
	}
	return nullptr;
}

void UModioButtonWidget::NativeSetDataSource(UObject* InDataSource)
{
	DataSource = InDataSource;
}

UObject* UModioButtonWidget::NativeGetDataSource()
{
	return DataSource;
}

void UModioButtonWidget::AddClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnClicked.AddUnique(Handler);
}

void UModioButtonWidget::RemoveClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnClicked.Remove(Handler);
}

void UModioButtonWidget::EnableClick_Implementation()
{
	SetIsInteractionEnabled(true);
}

void UModioButtonWidget::DisableClick_Implementation()
{
	SetIsInteractionEnabled(false);
}

void UModioButtonWidget::SetBoundAction_Implementation(const FDataTableRowHandle& InputActionRow)
{
	StoredInputActionRow = InputActionRow;
	SetTriggeringInputAction(InputActionRow);
}

bool UModioButtonWidget::TryGetBoundAction_Implementation(FDataTableRowHandle& InputActionRow) const
{
	return GetInputAction(InputActionRow);
}

void UModioButtonWidget::ClearBoundAction_Implementation()
{
	// This check prevents a breakpoint in ensure(CommonInputSubsystem) from being hit, 
	// such as in UCommonActionWidget::UpdateActionWidget()
	if (const UCommonInputSubsystem* CommonInputSubsystem = GetInputSubsystem())
	{
		FDataTableRowHandle EmptyStateHandle;
		SetTriggeringInputAction(EmptyStateHandle);
	}
}

void UModioButtonWidget::AddActionProgressHandler_Implementation(const FModioBoundActionProgressed& Handler)
{
	if (!InputActionWidget)
	{
		return;
	}
	OnModioActionProgressed = Handler;
}

void UModioButtonWidget::RemoveActionProgressHandler_Implementation(const FModioBoundActionProgressed& Handler)
{
	if (!InputActionWidget)
	{
		return;
	}
	OnModioActionProgressed.Clear();
}

void UModioButtonWidget::AddActionCompletedHandler_Implementation(const FModioBoundActionCompleted& Handler)
{
	if (!InputActionWidget)
	{
		return;
	}
	OnModioActionCompleted = Handler;
}

void UModioButtonWidget::RemoveActionCompletedHandler_Implementation(const FModioBoundActionCompleted& Handler)
{
	if (!InputActionWidget)
	{
		return;
	}
	OnModioActionCompleted.Clear();
}

UWidget* UModioButtonWidget::NativeGetWidgetToFocus(EUINavigation NavigationType) const
{
	return const_cast<UModioButtonWidget*>(this);
}

void UModioButtonWidget::NativeAddFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler)
{
	OnModioFocusPathChanged.AddUnique(Handler);
}

void UModioButtonWidget::NativeRemoveFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler)
{
	OnModioFocusPathChanged.Remove(Handler);
}

void UModioButtonWidget::NativeOnClicked()
{
	Super::NativeOnClicked();
	OnClicked.Broadcast(this);
}

void UModioButtonWidget::NativeOnSelected(bool bBroadcast)
{
	Super::NativeOnSelected(bBroadcast);
	if (bBroadcast)
	{
		OnSelected.Broadcast(this, true);
	}
}

void UModioButtonWidget::NativeOnDeselected(bool bBroadcast)
{
	Super::NativeOnDeselected(bBroadcast);
	if (bBroadcast)
	{
		OnSelected.Broadcast(this, false);
	}
}

void UModioButtonWidget::NativeOnHovered()
{
	// Dont broadcast up into blueprint if we are not hoverable
	// Dont broadcast external facing event for hover if we are not hoverable
	if (bIsHoverable)
	{
		Super::NativeOnHovered();
		if (bFocusOnHover)
		{
			SetCommonUIButtonFocus();
		}
		OnHoverStateChanged.Broadcast(this, true);
	}
}

void UModioButtonWidget::NativeOnUnhovered()
{
	if (bIsHoverable)
	{
		Super::NativeOnUnhovered();
		OnHoverStateChanged.Broadcast(this, false);
	}
}

void UModioButtonWidget::HandleFocusReceived()
{
	Super::HandleFocusReceived();
	SetCommonUIButtonFocus();
}

void UModioButtonWidget::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	if (bEnableKeyboardHover)
	{
		if (bIsHoverProcessing) return;
		TGuardValue<bool> RecursionGuard(bIsHoverProcessing, true);
		if (GetIsEnabled() && IsInteractionEnabled())
		{
			NativeOnHovered();
		}
	}
	OnModioFocusPathChanged.Broadcast(this, InFocusEvent, true);
}

void UModioButtonWidget::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (bEnableKeyboardHover)
	{
		if (bIsHoverProcessing) return;
		TGuardValue<bool> RecursionGuard(bIsHoverProcessing, false);
		NativeOnUnhovered();
	}
	OnModioFocusPathChanged.Broadcast(this, InFocusEvent, false);
}

void UModioButtonWidget::NativeOnActionComplete()
{
	Super::NativeOnActionComplete();
	OnModioActionCompleted.ExecuteIfBound(StoredInputActionRow);
}

void UModioButtonWidget::NativeOnActionProgress(float HeldPercent)
{
	Super::NativeOnActionProgress(HeldPercent);
	OnModioActionProgressed.ExecuteIfBound(StoredInputActionRow, HeldPercent);
}

void UModioButtonWidget::GetToggleable_Implementation(bool& bIsToggleable)
{
	bIsToggleable = bToggleable;
}

void UModioButtonWidget::AddHoverStateChangedHandler_Implementation(const FModioHoverableOnHoverStateChanged& Handler)
{
	OnHoverStateChanged.AddUnique(Handler);
}

void UModioButtonWidget::RemoveHoverStateChangedHandler_Implementation(
	const FModioHoverableOnHoverStateChanged& Handler)
{
	OnHoverStateChanged.Remove(Handler);
}

void UModioButtonWidget::EnableHoverEvents_Implementation()
{
	bIsHoverable = true;
}

void UModioButtonWidget::DisableHoverEvents_Implementation()
{
	bIsHoverable = false;
}

void UModioButtonWidget::SetTooltipEnabledState_Implementation(bool bNewEnabledState)
{
	SetToolTipText(bNewEnabledState ? GetToolTipText() : FText::GetEmpty());
}

void UModioButtonWidget::ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText,
														 const FText& TagText)
{
	SetToolTipText(TitleText);
}

void UModioButtonWidget::SetToggleable_Implementation(bool bNewToggleableState)
{
	SetIsToggleable(bNewToggleableState);
}

void UModioButtonWidget::GetSelectedState_Implementation(bool& bCurrentSelectedState)
{
	bCurrentSelectedState = GetSelected();
}

void UModioButtonWidget::ToggleSelectedState_Implementation()
{
	if (bSelectable)
	{
		SetIsSelected(!GetSelected());
	}
}

void UModioButtonWidget::SetSelectedState_Implementation(bool bNewSelectedState)
{
	if (bSelectable && GetInputSubsystem())
	{
		// Set the state directly, no mucking about with extra logic, if we want to clear selection on a
		// non-toggleable widget
		if (!bNewSelectedState && !bToggleable)
		{
			SetSelectedInternal(false);
		}

		SetIsSelected(bNewSelectedState);
	}
}

void UModioButtonWidget::GetSelectable_Implementation(bool& bIsSelectable)
{
	bIsSelectable = bSelectable;
}

void UModioButtonWidget::SetSelectable_Implementation(bool bNewSelectableState)
{
	SetIsSelectable(bNewSelectableState);
	SetIsSelected(false);
}

void UModioButtonWidget::RemoveSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	OnSelected.Remove(Handler);
}

void UModioButtonWidget::AddSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	OnSelected.AddUnique(Handler);
}

FReply UModioButtonWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (bShouldTriggerButtonClickOnEnterOrGamepadAccept &&
		(InKeyEvent.GetKey() == EKeys::Enter || InKeyEvent.GetKey() == EKeys::Virtual_Accept))
	{
		UE_LOG(ModioUICore, Log, TEXT("The key '%s' was pressed and handled by the button '%s' as a click"), *InKeyEvent.GetKey().ToString(), *GetName());
		HandleButtonClicked();
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}