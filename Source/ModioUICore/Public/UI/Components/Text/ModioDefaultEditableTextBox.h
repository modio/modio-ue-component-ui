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

#include "Components/EditableTextBox.h"
#include "CoreMinimal.h"
#include "UI/Components/ComponentHelpers.h"
#include "UI/Interfaces/IModioUIHasTooltipWidget.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"
#include "UI/Interfaces/IModioUITextValidator.h"
#include "Widgets/Input/SEditableTextBox.h"

#include "ModioDefaultEditableTextBox.generated.h"

/**
 * @brief Editable text box that is the same as SEditableTextBox, but with an additional delegate that fires when focus changes
 */
class MODIOUICORE_API SModioEditableTextBox : public SEditableTextBox
{
	//~ Begin SWidget Interface
	virtual void OnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath,
	                             const FFocusEvent& InFocusEvent) override;
	//~ End SWidget Interface
public:
	/** Delegate that fires when the focus of the editable text box changes */
	DECLARE_DELEGATE_OneParam(FOnEditableTextBoxFocusChanged, bool);

	/**
	 * @brief Delegate that fires when the focus of the editable text box changes
	 */
	FOnEditableTextBoxFocusChanged OnFocusChanged;
};

/**
 * Default implementation of an editable text box that can be used as a component in mod.io UI
 * (implements IModioUIStringInputWidget, IModioUITextValidator and IModioUIHasTooltipWidget)
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioDefaultEditableTextBox : public UEditableTextBox,
													 public IModioUIStringInputWidget,
													 public IModioUITextValidator,
													 public IModioUIHasTooltipWidget
{
	GENERATED_BODY()

public:
	/** Delegate that fires when the focus of the editable text box changes */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEditableTextBoxFocusChanged, bool, bIsFocused);

	/**
	 * @brief Delegate that fires when the text in the editable text box changes
	 */
	UPROPERTY(BlueprintAssignable, Category = "Modio UI|Editable Text Box")
	FOnEditableTextBoxFocusChanged OnFocusChanged;

protected:
	//~ Begin IModioUIStringInputWidget Interface
	virtual FString NativeGatherInput() override;
	virtual void NativeSetHintText(FText InHintText) override;
	virtual void NativeSetInput(const FString& Input) override;
	//~ End IModioUIStringInputWidget Interface

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