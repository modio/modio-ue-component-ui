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

#include "UObject/Interface.h"

#include "IModioUIStringInputWidget.generated.h"

/**
 * @modio_ui_component_interface
 * @brief Interface indicating that the implementing widget can be queried to return a user-specified string input. The input value can be configured externally (for example, to set the default).
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
};
