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

#include "ModioSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"


#include "ModioRetryCounter.generated.h"

/**
 * @brief A simple retry counter that can be used to limit the number of retries for a given operation
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|Retry Counter")
struct FModioRetryCounter
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Number of retries remaining
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Transient, Category = "mod.io|UI|Retry Counter")
	int32 RetriesRemaining = 3;

#if WITH_EDITORONLY_DATA
	/**
	 * @brief Debug name for the retry counter
	 */
	UPROPERTY(EditAnywhere, Category = "mod.io|UI|Retry Counter")
	FName DebugName = "ModioRetryCounter";
#endif

	/**
	 * @brief Resets the retry counter
	 */
	void ResetImpl();

public:
	// TODO: Revise comments

	int32 GetRetriesRemaining() const;
	// Could probably have a version of Retry that takes a nullary functor to call if retry is permitted
	bool Retry();
	void Reset();
	FModioRetryCounter();
};

UCLASS()
class MODIOUICORE_API UModioRetryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Checks if the retry counter has retries remaining
	 * @param Target The retry counter to check
	 * @return True if the retry counter has retries remaining
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Retry")
	static bool CanRetry(const FModioRetryCounter& Target);

	/**
	 * @brief Checks if the retry counter has retries remaining, returning an exec enum
	 * @param Target The retry counter to check
	 * @param ReturnValue The return value
	 * @return True if the retry counter has retries remaining
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Retry", meta = (ExpandEnumAsExecs = "ReturnValue"))
	static bool CanRetryAsExec(const FModioRetryCounter& Target);

	/**
	 * @brief Attempts to retry the operation, decrementing the retry counter if successful
	 * @param Target The retry counter to use
	 * @return True if the operation should be retried
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Retry", meta = (ExpandEnumAsExecs = "ReturnValue"))
	static bool Retry(UPARAM(ref) FModioRetryCounter& Target);

	/**
	 * @brief Resets the retry counter
	 * @param Target The retry counter to reset
	 * @return The reset retry counter
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Retry",
			  meta = (DisplayName = "Reset Retry Count"))
	static FModioRetryCounter& ResetRetryCountPure(UPARAM(ref) FModioRetryCounter& Target);

	/**
	 * @brief Resets the retry counter
	 * @param Target The retry counter to reset
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Retry")
	static void ResetRetryCount(UPARAM(ref) FModioRetryCounter& Target);
};
