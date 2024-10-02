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

#include "Types/SlateEnums.h"
#include "UObject/Interface.h"

#include "IModioUIStringInputWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FModioOnTextCommitted, UObject*, Context, const FText&, Text, ETextCommit::Type,
									 CommitMethod);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FModioOnTextCommittedMulticast, UObject*, Context, const FText&, Text,
											   ETextCommit::Type, CommitMethod);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FModioOnTextChanged, UObject*, Context, const FText&, Text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioOnTextChangedMulticast, UObject*, Context, const FText&, Text);

/**
 * @modio_ui_component_interface
 * @brief Interface indicating that the implementing widget can be queried to return a user-specified string input. The
 * input value can be configured externally (for example, to set the default).
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|String Input Widget", Meta = (DocGroup = "UI"))
class MODIOUICORE_API UModioUIStringInputWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIStringInputWidget : public IInterface
{
	GENERATED_BODY()
protected:
	virtual FString NativeGatherInput()
	{
		return FString {};
	}
	FString GatherInput_Implementation()
	{
		return NativeGatherInput();
	}

	virtual void NativeSetHintText(FText InHintText) {}
	void SetHint_Implementation(const FText& HintText)
	{
		NativeSetHintText(HintText);
	}

	virtual void NativeSetInput(const FString& Input) {}
	void SetInput_Implementation(const FString& Input)
	{
		NativeSetInput(Input);
	}

	virtual void NativeAddTextCommittedHandler(const FModioOnTextCommitted& Handler) {}
	void AddTextCommittedHandler_Implementation(const FModioOnTextCommitted& Handler)
	{
		NativeAddTextCommittedHandler(Handler);
	}

	virtual void NativeRemoveTextCommittedHandler(const FModioOnTextCommitted& Handler) {}
	void RemoveTextCommittedHandler_Implementation(const FModioOnTextCommitted& Handler)
	{
		NativeRemoveTextCommittedHandler(Handler);
	}

	virtual void NativeAddTextChangedHandler(const FModioOnTextChanged& Handler) {}
	void AddTextChangedHandler_Implementation(const FModioOnTextChanged& Handler)
	{
		NativeAddTextChangedHandler(Handler);
	}

	virtual void NativeRemoveTextChangedHandler(const FModioOnTextChanged& Handler) {}
	void RemoveTextChangedHandler_Implementation(const FModioOnTextChanged& Handler)
	{
		NativeRemoveTextChangedHandler(Handler);
	}

public:
	/**
	 * @brief Sets the hint text for the implementing widget
	 * @param HintText Text to display
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|String Input Widget")
	void SetHint(const FText& HintText);

	/**
	 * @brief Retrieves the user's input from the implementing widget
	 * @return The string that the user provided
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|String Input Widget")
	FString GatherInput();

	/**
	 * @brief Directly sets/overrides the user input on the implementing widget
	 * @param Input The input string to set
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|String Input Widget")
	void SetInput(const FString& Input);

	/**
	 * @brief Registers a delegate to receive callbacks when the user commits text in the implementing widget
	 * @param Handler Delegate to invoke on text committed event
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|String Input Widget")
	void AddTextCommittedHandler(const FModioOnTextCommitted& Handler);

	/**
	 * @brief Unregisters a delegate so it will no longer receive callbacks when the user commits text in the
	 * implementing widget
	 * @param Handler Delegate to unsubscribe
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|String Input Widget")
	void RemoveTextCommittedHandler(const FModioOnTextCommitted& Handler);

	/**
	 * @brief Registers a delegate to receive callbacks when the text in the implementing widget changes
	 * @param Handler Delegate to invoke on text changed event
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|String Input Widget")
	void AddTextChangedHandler(const FModioOnTextChanged& Handler);

	/**
	 * @brief Unregisters a delegate so it will no longer receive callbacks when the text in the implementing widget
	 * changes
	 * @param Handler Delegate to unsubscribe
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|String Input Widget")
	void RemoveTextChangedHandler(const FModioOnTextChanged& Handler);
};
