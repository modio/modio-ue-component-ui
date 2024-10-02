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

#include "Components/Widget.h"
#include "CoreMinimal.h"
#include "Framework/Text/TextLayout.h"
#include "UI/Interfaces/IModioUIHasTooltipWidget.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"
#include "UI/Interfaces/IModioUITextValidator.h"
#include "Widgets/Input/IVirtualKeyboardEntry.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"

#include "ModioDefaultCodeInputTextBox.generated.h"

enum class EVirtualKeyboardTrigger : uint8;
enum class EVirtualKeyboardDismissAction : uint8;
class STextBlock;
class SUniformGridPanel;
class SEditableText;

USTRUCT(BlueprintType)
struct FModioDefaultCodeInputTextBoxStyle : public FTextBlockStyle
{
	GENERATED_BODY()

	FModioDefaultCodeInputTextBoxStyle();

	/**
	 * Brush to use for the box around each character element, when not focused
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Widget|Border Brush")
	FSlateBrush CharacterBorderBrushBase;

	/**
	 * Brush to use for the box around each character element, when focused
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Widget|Border Brush")
	FSlateBrush CharacterBorderBrushFocused;

	/**
	 * Padding to use for the box around each character element
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Widget|Border Brush")
	FMargin CharacterBorderPadding;

	/**
	 * Fake caret brush (displayed when the input field is focused, at the location of the next character to be entered)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Widget|FakeCaret")
	FSlateBrush FakeCaretBrush;

	/**
	 * Horizontal alignment of the fake caret
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Widget|FakeCaret")
	TEnumAsByte<EHorizontalAlignment> FakeCaretHorizontalAlignment = EHorizontalAlignment::HAlign_Fill;

	/**
	 * Vertical alignment of the fake caret
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Widget|FakeCaret")
	TEnumAsByte<EVerticalAlignment> FakeCaretVerticalAlignment = EVerticalAlignment::VAlign_Fill;

	/**
	 * Character padding around each character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Widget")
	FMargin CharacterPadding;

	/**
	 * Character spacing between each character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Widget")
	FMargin CharacterSpacing;

	/**
	 * Minimum width of each character
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Widget")
	float MinimumCharacterWidth = 0;
};

/**
 * Code Input Text Box Slate widget
 */
class MODIOUICORE_API SModioDefaultCodeInputTextBox : public SCompoundWidget
{
protected:
	/**
	 * Creates a character widget for the code input text box
	 * @param WidgetIndex The index of the widget to create
	 * @return The created widget
	 */
	virtual TSharedRef<SWidget> CreateCharacterWidget(int32 WidgetIndex);

	/**
	 * Called when the user presses a key down
	 * @param MyGeometry The geometry of the code input text box
	 * @param InKeyEvent The key event
	 * @return Whether the key event was handled
	 */
	virtual FReply OnKeyDownHandler(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);

	/** The style of the code input text box */
	FModioDefaultCodeInputTextBoxStyle CodeInputStyle;

	/** The number of children in the code input text box */
	TOptional<ETextFlowDirection> TextFlowDirection;

	/** The brush to use for the character border */
	mutable FSlateBrush CachedCharacterBrush;

	/**
	 * Rebuilds the children of the code input text box (the characters)
	 * @param NumChildren The number of children to rebuild
	 */
	virtual void RebuildChildren(uint32 NumChildren);

	//~ Begin SWidget Interface
	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	virtual bool SupportsKeyboardFocus() const override;
	//~ End SWidget Interface

	/**
	 * Called when the text in the input field changes
	 * @param NewText The new text
	 */
	virtual void OnInputTextChanged(const FText& NewText);

	/**
	 * Gets the character at the specified index
	 * @param Index The index of the character to get
	 * @return The character at the specified index
	 */
	FText GetCharacterAtIndex(int32 Index) const;

	/**
	 * Gets the border brush for the character
	 * @return The border brush for the character
	 */
	virtual const FSlateBrush* GetCharacterBorder() const;

	/**
	 * Gets the visibility of the caret
	 * @param Index The index of the character to get the caret visibility for
	 * @return The visibility of the caret
	 */
	virtual EVisibility GetCaretVisibility(uint32 Index) const;

	/**
	 * Gets the number of characters in the code input text box
	 * @return The number of characters in the code input text box
	 */
	virtual int32 GetNumOfCharacters() const;

public:
	/** Static delegate for when the user submits the code */
	DECLARE_DELEGATE(FOnCodeSubmit);

	/**
	 * Called when the user submits the code
	 */
	FOnCodeSubmit OnCodeSubmit;

	/**
	 * Called when the text in the input field changes
	 */
	FOnTextChanged OnTextChanged;

	SLATE_BEGIN_ARGS(SModioDefaultCodeInputTextBox) : _NumChildren(5) {}

	/** The style of the code input text box */
	SLATE_ARGUMENT(FModioDefaultCodeInputTextBoxStyle, Style);

	/** The number of characters to display in the code input text box */
	SLATE_ARGUMENT(int32, NumChildren);

	/** Called when the user submits the code */
	SLATE_EVENT(FOnCodeSubmit, OnCodeSubmit)

	SLATE_EVENT(FOnTextChanged, OnTextChanged)

	/** Text flow direction (how text is laid out) */
	SLATE_ARGUMENT(TOptional<ETextFlowDirection>, TextFlowDirection)

	/** Additional options to be used by the virtual keyboard summoned from this widget */
	SLATE_ARGUMENT(FVirtualKeyboardOptions, VirtualKeyboardOptions)

	/** The type of event that will trigger the display of the virtual keyboard */
	SLATE_ARGUMENT(EVirtualKeyboardTrigger, VirtualKeyboardTrigger)

	/** The message action to take when the virtual keyboard is dismissed by the user */
	SLATE_ARGUMENT(EVirtualKeyboardDismissAction, VirtualKeyboardDismissAction)

	SLATE_END_ARGS();

	void Construct(const FArguments& InArgs);

	/**
	 * Sets the style of the code input text box
	 * @param NewStyle The style to set
	 */
	void SetStyle(const FModioDefaultCodeInputTextBoxStyle& NewStyle);

	/**
	 * Sets the number of children in the code input text box
	 * @param NewNumChildren The number of children to set
	 */
	void SetNumChildren(int32 NewNumChildren);

	/**
	 * Gets the code string from the code input text box
	 * @return The code string
	 */
	FString GetCodeString() const;

	/**
	 * Sets the code string in the code input text box
	 * @param NewCodeString The code string to set
	 */
	void SetCodeString(const FString& NewCodeString);

protected:
	/** Character widgets */
	TArray<TSharedPtr<STextBlock>> CharacterWidgets;

	/** The grid panel that contains the character widgets */
	TSharedPtr<SUniformGridPanel> MyCharacterGrid;

	/** The hidden input field (used to capture keyboard input) */
	TSharedPtr<SEditableText> HiddenInputField;
};

/**
 * Default implementation of a code input text box that can be used as a component in mod.io UI
 * (implements IModioUIStringInputWidget, IModioUITextValidator and IModioUIHasTooltipWidget)
 * The code input text box is a text box that displays a number of characters, each in its own box. The user can enter a
 * code by entering the characters in the boxes
 */
UCLASS()
class MODIOUICORE_API UModioDefaultCodeInputTextBox : public UWidget,
													  public IModioUIStringInputWidget,
													  public IModioUITextValidator,
													  public IModioUIHasTooltipWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Widget Getters")
	TScriptInterface<IModioUIHasTextWidget> GetValidationErrorWidget() const;

	/** The style of the code input text box */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io|UI|Code Input")
	FModioDefaultCodeInputTextBoxStyle CodeInputStyle;

	/** Text flow direction (how text is laid out) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io|UI|Code Input")
	ETextFlowDirection TextFlowDirection = ETextFlowDirection::LeftToRight;

	/** Additional options to be used by the virtual keyboard summoned from this widget */
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "mod.io|UI|Code Input")
	FVirtualKeyboardOptions VirtualKeyboardOptions;

	/** The type of event that will trigger the display of the virtual keyboard */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "mod.io|UI|Code Input",
			  meta = (BlueprintProtected))
	EVirtualKeyboardTrigger VirtualKeyboardTrigger;

	/** What action should be taken when the virtual keyboard is dismissed? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay, Category = "mod.io|UI|Code Input",
			  meta = (BlueprintProtected))
	EVirtualKeyboardDismissAction VirtualKeyboardDismissAction;

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
	
	//~ Begin IModioUITextValidator Interface
	virtual void GetTextValidationRules_Implementation(TArray<FModioTextValidationRule>& Rules) override;
	virtual void SetValidationError_Implementation(const FText& ErrorText) override;
	//~ End IModioUITextValidator Interface

	//~ Begin IModioUIHasTooltipWidget Interface
	virtual void SetTooltipEnabledState_Implementation(bool bNewEnabledState) override;
	virtual void ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText,
												 const FText& TagText) override;
	//~ End IModioUIHasTooltipWidget Interface

	//~ Begin UWidget Interface
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

public:
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UWidget Interface

public:
	/** The number of characters to display in the code input text box */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Code Input")
	int32 NumberOfCharacters = 5;

	/** Static delegate for when the user submits the code. Suitable for C++ use */
	DECLARE_MULTICAST_DELEGATE(FOnCodeSubmit);
	/** Called when the user submits the code. Suitable for C++ use */
	FOnCodeSubmit OnCodeSubmit;

	/** Dynamic delegate for when the user submits the code. Suitable for Blueprint use */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCodeSubmitDynamic);
	/** Called when the user submits the code. Suitable for Blueprint use */
	UPROPERTY(BlueprintAssignable, Category = "mod.io|UI|Code Input")
	FOnCodeSubmitDynamic OnCodeSubmitDynamic;

protected:
	/** The code input text box */
	TSharedPtr<SModioDefaultCodeInputTextBox> MyInput;
};