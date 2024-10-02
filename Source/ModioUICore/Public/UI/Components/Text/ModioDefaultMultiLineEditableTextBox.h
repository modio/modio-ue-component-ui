/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Components/MultiLineEditableTextBox.h"
#include "CoreMinimal.h"
#include "UI/Components/ComponentHelpers.h"
#include "UI/Interfaces/IModioUIHasTooltipWidget.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"
#include "UI/Interfaces/IModioUITextValidator.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

#include "ModioDefaultMultiLineEditableTextBox.generated.h"


/**
 * Multi Line Editable Text Box which supports gamepad navigation
 */
class MODIOUICORE_API SModioCommonMultiLineEditableTextBox : public SMultiLineEditableTextBox
{
public:
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
};

/**
 * Default implementation of an editable multi line text box that can be used as a component in mod.io UI
 * (implements IModioUIStringInputWidget, IModioUITextValidator and IModioUIHasTooltipWidget)
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioDefaultMultiLineEditableTextBox : public UMultiLineEditableTextBox,
															  public IModioUIStringInputWidget,
															  public IModioUITextValidator,
															  public IModioUIHasTooltipWidget
{
	GENERATED_BODY()

protected:
	/** Workaround as we lose focus when the auto completion closes. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behavior, AdvancedDisplay)
	bool IsCaretMovedWhenGainFocus = true;

	/** Whether to select all text when the user clicks to give focus on the widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behavior, AdvancedDisplay)
	bool SelectAllTextWhenFocused;

	/** Whether to allow the user to back out of changes when they press the escape key */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behavior, AdvancedDisplay)
	bool RevertTextOnEscape;

	/** Whether to clear keyboard focus when pressing enter to commit changes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behavior, AdvancedDisplay)
	bool ClearKeyboardFocusOnCommit = true;

	/** Whether to select all text when pressing enter to commit changes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Behavior, AdvancedDisplay)
	bool SelectAllTextOnCommit;

	/** Whether to show scroll bars */
	UPROPERTY(EditAnywhere, Category = Behavior, AdvancedDisplay)
	bool AlwaysShowScrollbars;

	/** Whether to allow multi-line text */
	UPROPERTY(EditAnywhere, Category = Behavior, AdvancedDisplay)
	bool AllowMultiLine = true;

	/** The type of event that will trigger the display of the virtual keyboard */
	UPROPERTY(EditAnywhere, Category = Behavior, AdvancedDisplay)
	EVirtualKeyboardTrigger VirtualKeyboardTrigger;

	/** Sets what should happen when text is clipped because the block does not have enough space */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance)
	ETextOverflowPolicy OverflowPolicy;

protected:
	//~ Begin IModioUIStringInputWidget Interface
	virtual FString NativeGatherInput() override;
	virtual void NativeSetHintText(FText InHintText) override;
	virtual void NativeSetInput(const FString& Input) override;
	virtual void NativeAddTextCommittedHandler(const FModioOnTextCommitted& Handler) override;
	virtual void NativeRemoveTextCommittedHandler(const FModioOnTextCommitted& Handler) override;
	virtual void NativeAddTextChangedHandler(const FModioOnTextChanged& Handler) override;
	virtual void NativeRemoveTextChangedHandler(const FModioOnTextChanged& Handler) override;
	//~ End IModioUIStringInputWidget Interface

	/**
	 * @brief Passes `this` as Context, `Text` as the new text, `CommitMethod` as the method used to commit the text
	 * @default_component_event FModioOnTextCommitted
	 */
	UPROPERTY()
	FModioOnTextCommittedMulticast OnModioTextCommitted;

	/**
	 * @brief Passes `this` as Context, `Text` as the new text
	 * @default_component_event FModioOnTextChanged
	 */
	UPROPERTY()
	FModioOnTextChangedMulticast OnModioTextChanged;

	//~ Begin IModioUIHasTooltipWidget Interface
	virtual void SetTooltipEnabledState_Implementation(bool bNewEnabledState) override;
	virtual void ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText,
												 const FText& TagText) override;
	//~ End IModioUIHasTooltipWidget Interface

	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	MODIO_WIDGET_PALETTE_OVERRIDE();
};
