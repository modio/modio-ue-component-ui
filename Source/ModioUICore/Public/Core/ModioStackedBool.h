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
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ModioStackedBool.generated.h"

/**
 * @brief A stack of boolean values
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|Stack")
struct FModioStackedBool
{
	GENERATED_BODY()

	/**
	 * @brief The stack of boolean values
	 */
	UPROPERTY()
	TArray<bool> ValueStack;

	FModioStackedBool(bool bInitialState)
	{
		ValueStack.Add(bInitialState);
	}
	FModioStackedBool() = default;

	/**
	 * @brief Returns the number of values on the stack
	 * @return The number of values on the stack
	 */
	int32 Count() const
	{
		return ValueStack.Num();
	}

	/**
	 * @brief Pushes a new value onto the stack
	 * @param bNewValue The value to push onto the stack
	 */
	void Push(bool bNewValue)
	{
		ValueStack.Push(bNewValue);
	}

	/**
	 * @brief Pops a value from the stack
	 * @return True if the pop was successful
	 */
	bool Pop()
	{
		return ValueStack.Pop();
	}

	/**
	 * @brief Peeks at the top value of the stack
	 * @return The peeked value
	 */
	bool Peek() const
	{
		return ValueStack.Top();
	}

	/**
	 * @brief Peeks at the top value of the stack
	 * @param bDefaultValue The default value to return if the stack is empty
	 * @return The peeked value
	 */
	bool Peek(bool bDefaultValue) const
	{
		if (ValueStack.Num())
		{
			return ValueStack.Top();
		}
		else
		{
			return bDefaultValue;
		}
	}
};

/**
 * @brief Library of functions for working with stacked bool values
 */
UCLASS()
class MODIOUICORE_API UModioStackedBoolLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Pushes a new value onto the stack
	 * @param Target The stack to push the value onto
	 * @param bNewValue The value to push onto the stack
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Stack<bool>", meta = (DisplayName = "Push Value on Stack(bool)"))
	static void PushBoolValue(UPARAM(Ref) FModioStackedBool& Target, bool bNewValue);

	/**
	 * @brief Pops a value from the stack
	 * @param Target The stack to pop the value from
	 * @param bDidPopSuccessfully True if the pop was successful
	 * @return The popped value
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Stack<bool>",
			  meta = (DisplayName = "Pop Value from Stack(bool)"))
	static bool PopBoolValue(UPARAM(Ref) FModioStackedBool& Target, bool& bDidPopSuccessfully);

	/**
	 * @brief Peeks at the top value of the stack
	 * @param Target The stack to peek at
	 * @param bDidPeekSuccessfully True if the peek was successful
	 * @return The peeked value
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Stack<bool>",
			  meta = (DisplayName = "Peek Value from Stack(bool)"))
	static bool PeekBoolValue(const FModioStackedBool& Target, bool& bDidPeekSuccessfully);
};
