/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Text/ModioDefaultCodeInputTextBox.h"
#include "Core/ModioAssetPaths.h"

#include "Brushes/SlateBorderBrush.h"
#include "Framework/Application/SlateApplication.h"
#include "ModioUICore.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/SOverlay.h"

#include "Loc/BeginModioLocNamespace.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioDefaultCodeInputTextBox)

FModioDefaultCodeInputTextBoxStyle::FModioDefaultCodeInputTextBoxStyle()
{
	SetFont(FCoreStyle::GetDefaultFontStyle("Bold", 18));
	SetColorAndOpacity(FLinearColor::White);
	SetShadowOffset(FVector2D(1, 1));
	SetShadowColorAndOpacity(FLinearColor::Transparent);
	TransformPolicy = ETextTransformPolicy::None;

	UObject* UnderlineMaterial = ModioUI::AssetPaths::GetUnderlineMaterialPath().TryLoad();
	UObject* FakeCaretMaterial = ModioUI::AssetPaths::GetFakeCaretMaterialPath().TryLoad();

	if (UnderlineMaterial)
	{
		CharacterBorderBrushBase.SetResourceObject(UnderlineMaterial);
		CharacterBorderBrushFocused.SetResourceObject(UnderlineMaterial);
	}

	if (FakeCaretMaterial)
	{
		FakeCaretBrush.SetResourceObject(FakeCaretMaterial);
	}

	CharacterBorderBrushBase.TintColor = FLinearColor::White;
	CharacterBorderBrushFocused.TintColor = FLinearColor::Blue;
	FakeCaretBrush.TintColor = FLinearColor::Blue;

	CharacterPadding = FMargin(0, 0, 0, 8);
	CharacterSpacing = FMargin(6, 0, 6, -16);
}

TSharedRef<SWidget> SModioDefaultCodeInputTextBox::CreateCharacterWidget(int32 WidgetIndex)
{
	TSharedRef<STextBlock> Widget = SNew(STextBlock)
										.Justification(ETextJustify::Center)
										.TextStyle(&CodeInputStyle)
										.MinDesiredWidth(CodeInputStyle.MinimumCharacterWidth)
										.Margin(CodeInputStyle.CharacterPadding)
										.Text(TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateSP(
											this, &SModioDefaultCodeInputTextBox::GetCharacterAtIndex, WidgetIndex)));
	CharacterWidgets.Add(Widget);
	return Widget;
}

FReply SModioDefaultCodeInputTextBox::OnKeyDownHandler(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Enter || InKeyEvent.GetKey() == EKeys::Virtual_Accept)
	{
		OnCodeSubmit.ExecuteIfBound();
	}

	return FReply::Unhandled();
}

void SModioDefaultCodeInputTextBox::RebuildChildren(uint32 NumChildren)
{
	CharacterWidgets.Empty();
	MyCharacterGrid->ClearChildren();

	// Display widgets need to be looking at the reverse index when we are in a culture which is RTL
	const bool bIsRightToLeft = TextFlowDirection.IsSet() && TextFlowDirection == ETextFlowDirection::RightToLeft;

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ChildIndex++)
	{
		const int32 ColumnIndex = int32(bIsRightToLeft ? NumChildren - ChildIndex - 1U : ChildIndex);
		SUniformGridPanel::FSlot& CurrentSlot = *MyCharacterGrid->AddSlot(ColumnIndex, 0).GetSlot();
		CurrentSlot.SetHorizontalAlignment(HAlign_Center);
		CurrentSlot.SetVerticalAlignment(VAlign_Center);
		CurrentSlot.AttachWidget(
			SNew(SBorder).BorderImage(this, &SModioDefaultCodeInputTextBox::GetCharacterBorder)
				[SNew(SOverlay) +
				 SOverlay::Slot()[SNew(SImage)
									  .Image(&CodeInputStyle.FakeCaretBrush)
									  .Visibility(this, &SModioDefaultCodeInputTextBox::GetCaretVisibility,
												  static_cast<uint32>(ChildIndex))]
					 .HAlign(CodeInputStyle.FakeCaretHorizontalAlignment)
					 .VAlign(CodeInputStyle.FakeCaretVerticalAlignment) +
				 SOverlay::Slot()[CreateCharacterWidget(int32(ChildIndex))]]);
	}

	Invalidate(EInvalidateWidgetReason::Layout);
}

FReply SModioDefaultCodeInputTextBox::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	FSlateApplication::Get().SetAllUserFocus(HiddenInputField);
	return FReply::Handled();
}

bool SModioDefaultCodeInputTextBox::SupportsKeyboardFocus() const
{
	return true;
}

void SModioDefaultCodeInputTextBox::OnInputTextChanged(const FText& NewText)
{
	if (!HiddenInputField.IsValid())
	{
		UE_LOG(
			ModioUICore, Error,
			TEXT(
				"Unable to process input text change in SModioDefaultCodeInputTextBox: HiddenInputField is not valid"));
		return;
	}

	const FString TruncatedText = NewText.ToString().Mid(0, CharacterWidgets.Num()).ToUpper();
	if (HiddenInputField->GetText().ToString() != TruncatedText)
	{
		HiddenInputField->SetText(FText::FromString(TruncatedText));
	}
	else if (NewText.ToString().Len() > CharacterWidgets.Num())
	{
		HiddenInputField->SetText(FText::FromString(TruncatedText));
	}

	OnTextChanged.ExecuteIfBound(NewText);
}

FText SModioDefaultCodeInputTextBox::GetCharacterAtIndex(int32 Index) const
{
	if (!HiddenInputField.IsValid())
	{
		UE_LOG(
			ModioUICore, Error,
			TEXT("Unable to get character at index %d in SModioDefaultCodeInputTextBox: HiddenInputField is not valid"),
			Index);
		return FText::GetEmpty();
	}

	const FString CurrentText = HiddenInputField->GetText().ToString();
	if (Index < CurrentText.Len())
	{
		return FText::FromString(CurrentText.Mid(Index, 1));
	}
	return FText::GetEmpty();
}

const FSlateBrush* SModioDefaultCodeInputTextBox::GetCharacterBorder() const
{
	if (FSlateApplication::Get().HasFocusedDescendants(AsShared()))
	{
		CachedCharacterBrush = CodeInputStyle.CharacterBorderBrushFocused;
	}
	else
	{
		CachedCharacterBrush = CodeInputStyle.CharacterBorderBrushBase;
	}
	CachedCharacterBrush.Margin = CodeInputStyle.CharacterBorderPadding;
	return &CachedCharacterBrush;
}

EVisibility SModioDefaultCodeInputTextBox::GetCaretVisibility(uint32 Index) const
{
	if (!HiddenInputField.IsValid())
	{
		UE_LOG(ModioUICore, Error,
			   TEXT("Unable to get caret visibility at index %d in SModioDefaultCodeInputTextBox: HiddenInputField is "
					"not valid"),
			   Index);
		return EVisibility::Hidden;
	}

	if (Index == HiddenInputField->GetText().ToString().Len())
	{
		if (FSlateApplication::Get().HasFocusedDescendants(AsShared()))
		{
			return EVisibility::Visible;
		}
	}
	return EVisibility::Hidden;
}

int32 SModioDefaultCodeInputTextBox::GetNumOfCharacters() const
{
	return CharacterWidgets.Num();
}

void SModioDefaultCodeInputTextBox::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	#ifdef _MSC_VER
	#pragma warning(push)
	// warning C4866: compiler may not enforce left-to-right evaluation order for call
	#pragma warning(disable : 4866)
	#endif
	// clang-format off
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(MyCharacterGrid, SUniformGridPanel)
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.Padding(FMargin(0,0,0,24))
		[
			SNew(SBox)
			.WidthOverride(1)
			.HeightOverride(1)
			[
				SAssignNew(HiddenInputField, SEditableText)
				.CaretImage(TAttribute<const FSlateBrush*>())
				.OnKeyDownHandler(this, &SModioDefaultCodeInputTextBox::OnKeyDownHandler)
				.OnTextChanged(this, &SModioDefaultCodeInputTextBox::OnInputTextChanged)
				.VirtualKeyboardOptions(InArgs._VirtualKeyboardOptions)
				.VirtualKeyboardTrigger(InArgs._VirtualKeyboardTrigger)
				.VirtualKeyboardDismissAction(InArgs._VirtualKeyboardDismissAction)
			]
		]
	];
	#ifdef _MSC_VER
	#pragma warning(pop)
	#endif
	// clang-format on

	CodeInputStyle = InArgs._Style;
	MyCharacterGrid->SetSlotPadding(CodeInputStyle.CharacterSpacing);
	OnCodeSubmit = InArgs._OnCodeSubmit;
	OnTextChanged = InArgs._OnTextChanged;
	
	TextFlowDirection = InArgs._TextFlowDirection;

	RebuildChildren(uint32(InArgs._NumChildren));
}

void SModioDefaultCodeInputTextBox::SetStyle(const FModioDefaultCodeInputTextBoxStyle& NewStyle)
{
	CodeInputStyle = NewStyle;
	if (MyCharacterGrid)
	{
		MyCharacterGrid->SetSlotPadding(CodeInputStyle.CharacterSpacing);
		for (const TSharedPtr<STextBlock>& Child : CharacterWidgets)
		{
			if (Child)
			{
				Child->SetTextStyle(&CodeInputStyle);
			}
		}
	}
	Invalidate(EInvalidateWidgetReason::Layout | EInvalidateWidgetReason::Paint);
}

void SModioDefaultCodeInputTextBox::SetNumChildren(int32 NewNumChildren)
{
	if (NewNumChildren != GetNumOfCharacters())
	{
		RebuildChildren(uint32_t(NewNumChildren));
	}
}

FString SModioDefaultCodeInputTextBox::GetCodeString() const
{
	if (!HiddenInputField.IsValid())
	{
		UE_LOG(ModioUICore, Error,
			   TEXT("Unable to get code string in SModioDefaultCodeInputTextBox: HiddenInputField is not valid"));
		return FString();
	}
	const FString CurrentText = HiddenInputField->GetText().ToString();
	UE_LOG(ModioUICore, Warning, TEXT("Returning CurrentText '%s' from SModioDefaultCodeInputTextBox::GetCodeString"),
		   *CurrentText);
	return CurrentText;
}

void SModioDefaultCodeInputTextBox::SetCodeString(const FString& NewCodeString)
{
	OnInputTextChanged(FText::FromString(NewCodeString));
}

TScriptInterface<IModioUIHasTextWidget> UModioDefaultCodeInputTextBox::GetValidationErrorWidget_Implementation() const
{
	return nullptr;
}

FString UModioDefaultCodeInputTextBox::NativeGatherInput()
{
	if (MyInput)
	{
		return MyInput->GetCodeString();
	}
	UE_LOG(ModioUICore, Error,
		   TEXT("Unable to get code string in UModioDefaultCodeInputTextBox: MyInput is not valid"));
	return FString();
}

void UModioDefaultCodeInputTextBox::NativeSetHintText(FText InHintText)
{
	// That's enough for now
	SetToolTipText(InHintText);
}

void UModioDefaultCodeInputTextBox::NativeSetInput(const FString& Input)
{
	if (MyInput)
	{
		MyInput->SetCodeString(Input);
	}
	else
	{
		UE_LOG(ModioUICore, Error,
			   TEXT("Unable to set code string in UModioDefaultCodeInputTextBox: MyInput is not valid"));
	}
}

void UModioDefaultCodeInputTextBox::NativeAddTextCommittedHandler(const FModioOnTextCommitted& Handler)
{
	OnModioTextCommitted.AddUnique(Handler);
}

void UModioDefaultCodeInputTextBox::NativeRemoveTextCommittedHandler(const FModioOnTextCommitted& Handler)
{
	OnModioTextCommitted.Remove(Handler);
}

void UModioDefaultCodeInputTextBox::NativeAddTextChangedHandler(const FModioOnTextChanged& Handler)
{
	OnModioTextChanged.AddUnique(Handler);
}

void UModioDefaultCodeInputTextBox::NativeRemoveTextChangedHandler(const FModioOnTextChanged& Handler)
{
	OnModioTextChanged.Remove(Handler);
}

void UModioDefaultCodeInputTextBox::GetTextValidationRules_Implementation(TArray<FModioTextValidationRule>& Rules)
{
	FModioTextValidationRule Rule;
	Rule.RuleToUse = EModioTextValidationRule::ETVR_ValidateAllNumeric;
	Rule.MinimumLength = 0;
	Rule.MaximumLength = NumberOfCharacters;
	Rule.ValidationMessage = LOCTEXT("CodeInputValidationMessage", "Please enter a valid code (numbers only)");
}

void UModioDefaultCodeInputTextBox::SetValidationError_Implementation(const FText& ErrorText)
{
	if (GetValidationErrorWidget().GetObject() &&
		GetValidationErrorWidget().GetObject()->Implements<UModioUIHasTextWidget>())
	{
		IModioUIHasTextWidget::Execute_SetWidgetText(GetValidationErrorWidget().GetObject(), ErrorText);
	}
	else
	{
		UE_LOG(ModioUICore, Error,
			   TEXT("Unable to set validation error in '%s': Widget returned by GetValidationErrorWidget() is either "
					"not valid or does not implement IModioUIHasTextWidget"),
			   *GetName());
	}
}

void UModioDefaultCodeInputTextBox::SetTooltipEnabledState_Implementation(bool bNewEnabledState)
{
	SetToolTipText(bNewEnabledState ? GetToolTipText() : FText::GetEmpty());
}

void UModioDefaultCodeInputTextBox::ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText,
																	const FText& TagText)
{
	SetToolTipText(TitleText);
}

TSharedRef<SWidget> UModioDefaultCodeInputTextBox::RebuildWidget()
{
	return SAssignNew(MyInput, SModioDefaultCodeInputTextBox)
		.Style(CodeInputStyle)
		.NumChildren(NumberOfCharacters)
		.OnCodeSubmit(
			SModioDefaultCodeInputTextBox::FOnCodeSubmit::CreateWeakLambda(this,
																		   [this]() {
																			   OnCodeSubmit.Broadcast();
																			   OnCodeSubmitDynamic.Broadcast();
																			   OnModioTextCommitted.Broadcast(this, FText::FromString(Execute_GatherInput(this)), ETextCommit::OnEnter);
																		   }))
		.OnTextChanged(
			FOnTextChanged::CreateWeakLambda(this,
			                                 [this](const FText& InText) {
				                                 OnModioTextChanged.Broadcast(this, InText);
			                                 }))
		.TextFlowDirection(TextFlowDirection)
		.VirtualKeyboardOptions(VirtualKeyboardOptions)
		.VirtualKeyboardTrigger(VirtualKeyboardTrigger)
		.VirtualKeyboardDismissAction(VirtualKeyboardDismissAction);
}

void UModioDefaultCodeInputTextBox::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	MyInput.Reset();
}

#include "Loc/EndModioLocNamespace.h"