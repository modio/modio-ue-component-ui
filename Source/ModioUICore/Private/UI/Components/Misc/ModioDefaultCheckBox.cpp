/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioDefaultCheckBox.h"

#include "ModioUICore.h"
#include "UI/Components/Text/ModioDefaultTextBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Misc/EngineVersionComparison.h"
#include "UI/Interfaces/IModioUIHoverableWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioDefaultCheckBox)

UModioDefaultCheckBox::UModioDefaultCheckBox()
	: LabelClass(UModioDefaultTextBlock::StaticClass())
{
	OnCheckStateChanged.AddDynamic(this, &UModioDefaultCheckBox::HandleOnCheckStateChanged);
}

void UModioDefaultCheckBox::SetLabel(const FText& InLabelText)
{
	LabelText = InLabelText;
	if (MyCheckbox)
	{
		MyCheckbox->Invalidate(EInvalidateWidgetReason::Layout);
	}
	SynchronizeProperties();
}

FText UModioDefaultCheckBox::GetLabel() const
{
	return LabelText;
}

void UModioDefaultCheckBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (MyCheckbox)
	{
		MyCheckbox->SetContent(GetContentWidget());
	}

	if (LabelWidget && LabelWidget->Implements<UModioUIHasTextWidget>())
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(LabelWidget, LabelText);
	}
}

FText UModioDefaultCheckBox::GetLabelTextBlockText() const
{
	return LabelText;
}

TSharedRef<SWidget> UModioDefaultCheckBox::GetContentWidget()
{
	LabelWidget = nullptr;

	if (GetContentSlot() && GetContentSlot()->Content)
	{
		return GetContentSlot()->Content->TakeWidget();
	}
	else if (!LabelText.IsEmpty())
	{
		if (LabelClass)
		{
			LabelWidget = NewObject<UWidget>(this, LabelClass);
			if (LabelWidget && LabelWidget->Implements<UModioUIHasTextWidget>())
			{
				IModioUIHasTextWidget::Execute_SetWidgetText(LabelWidget, LabelText);
				return LabelWidget->TakeWidget();
			}
			else
			{
				UE_LOG(ModioUICore, Error, TEXT("Failed to create label text block for check box"));
			}
		}
		else
		{
			UE_LOG(ModioUICore, Error, TEXT("Label class is not set for check box"));
		}
	}
	return SNullWidget::NullWidget;
}

void UModioDefaultCheckBox::SetHovered_Implementation(bool bHovered)
{
	
}

TSharedRef<SWidget> UModioDefaultCheckBox::RebuildWidget()
{
	TSharedRef<SWidget> RebuiltCheckBox = SAssignNew(MyCheckbox, SModioDefaultCheckBox)
		.OnCheckStateChanged(BIND_UOBJECT_DELEGATE(FOnCheckStateChanged, SlateOnCheckStateChangedCallback))
		.Style(&GetWidgetStyle())
		.HAlign(HorizontalAlignment)
		.IsFocusable(
#if UE_VERSION_NEWER_THAN(5, 2, 0)
		GetIsFocusable()
#else
		IsFocusable
#endif
		)
		.Padding_UObject(this, &UModioDefaultCheckBox::GetCheckboxContentPadding)[GetContentWidget()];

	if (TSharedPtr<SModioDefaultCheckBox> ModioCommonCheckBox = StaticCastSharedPtr<SModioDefaultCheckBox>(MyCheckbox))
	{
		ModioCommonCheckBox->OnHoverChanged.AddWeakLambda(this, [this](bool bHovered) {
			SetHovered(bHovered);
		});
	}

	return RebuiltCheckBox;
}

void UModioDefaultCheckBox::AddSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	FDelegateHandle AddedHandler = OnModioDefaultCheckBoxStateChanged.AddWeakLambda(this, [this, Handler](bool bToggled) {
		Handler.ExecuteIfBound(this, bToggled);
	});
	ToggleableBoundHandlers.Add(Handler, MoveTemp(AddedHandler));
}

void UModioDefaultCheckBox::RemoveSelectedStateChangedHandler_Implementation(const FModioSelectableOnSelectionChanged& Handler)
{
	if (ToggleableBoundHandlers.Contains(Handler))
	{
		OnModioDefaultCheckBoxStateChanged.Remove(ToggleableBoundHandlers[Handler]);
		ToggleableBoundHandlers.Remove(Handler);
	}
}

void UModioDefaultCheckBox::SetSelectable_Implementation(bool bNewSelectableState)
{
	SetIsEnabled(bNewSelectableState);
}

void UModioDefaultCheckBox::GetSelectable_Implementation(bool& bIsSelectable)
{
	bIsSelectable = GetIsEnabled();
}

void UModioDefaultCheckBox::SetSelectedState_Implementation(bool bNewSelectedState)
{
	SetCheckedState(bNewSelectedState ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void UModioDefaultCheckBox::ToggleSelectedState_Implementation()
{
	SetIsEnabled(!GetIsEnabled());
}

void UModioDefaultCheckBox::GetSelectedState_Implementation(bool& bCurrentSelectedState)
{
	bCurrentSelectedState = GetCheckedState() == ECheckBoxState::Checked;
}

void UModioDefaultCheckBox::SetToggleable_Implementation(bool bNewToggleableState)
{
	SetIsEnabled(bNewToggleableState);
}

void UModioDefaultCheckBox::GetToggleable_Implementation(bool& bIsToggleable)
{
	bIsToggleable = GetIsEnabled();
}

void UModioDefaultCheckBox::NativeSetText(const FText& DisplayText)
{
	SetLabel(DisplayText);
}

FText UModioDefaultCheckBox::NativeGetText()
{
	return GetLabel();
}

void UModioDefaultCheckBox::NativeSetTextFont(FSlateFontInfo FontInfo)
{
	if (LabelWidget && LabelWidget->Implements<UModioUIHasTextWidget>())
	{
		IModioUIHasTextWidget::Execute_SetWidgetTextFont(LabelWidget, FontInfo);
	}
}

FSlateFontInfo UModioDefaultCheckBox::NativeGetTextFont()
{
	if (LabelWidget && LabelWidget->Implements<UModioUIHasTextWidget>())
	{
		return IModioUIHasTextWidget::Execute_GetWidgetTextFont(LabelWidget);
	}
	return FSlateFontInfo();
}

void UModioDefaultCheckBox::SetTooltipEnabledState_Implementation(bool bNewEnabledState)
{
	SetToolTipText(bNewEnabledState ? GetToolTipText() : FText::GetEmpty());
}

void UModioDefaultCheckBox::ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText, const FText& TagText)
{
	SetToolTipText(TitleText);
}

void UModioDefaultCheckBox::ReleaseSlateResources(bool bReleaseChildren)
{
	LabelWidget = nullptr;
	Super::ReleaseSlateResources(bReleaseChildren);
}

FMargin UModioDefaultCheckBox::GetCheckboxContentPadding_Implementation() const
{
	return ContentPadding;
}

void UModioDefaultCheckBox::HandleOnCheckStateChanged_Implementation(bool bIsChecked)
{
	if (OnModioDefaultCheckBoxStateChanged.IsBound())
	{
		OnModioDefaultCheckBoxStateChanged.Broadcast(bIsChecked);
	}
}