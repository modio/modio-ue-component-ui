/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Containers/Map.h"
#include "Delegates/Delegate.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/UnrealNames.h"

#include "ModioForceFeedbackRef.generated.h"

class UForceFeedbackEffect;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FModioForceFeedbackRefChangeDelegate);
DECLARE_DYNAMIC_DELEGATE(FModioForceFeedbackRefChangeHandler);

/**
 * @brief Struct representing a force feedback entry in a data table
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|ForceFeedbackRef")
struct MODIOUICORE_API FModioUIForceFeedbackEntry : public FTableRowBase
{
	GENERATED_BODY();

	/**
	 * @brief The force feedback effect to use for the entry
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, category = "mod.io UI Force Feedback Entry")
	TObjectPtr<UForceFeedbackEffect> ForceFeedback = nullptr;
};

/**
 * @brief Blueprint-compatible struct representing a force feedback reference
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|ForceFeedbackRef")
struct MODIOUICORE_API FModioForceFeedbackRef
{
	GENERATED_BODY();

	/**
	 * @brief The name of the force feedback effect to reference
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "mod.io|UI|ForceFeedbackRef")
	FName ReferencedForceFeedbackName;

	/**
	 * @brief The force feedback effect to use if the reference is not found
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ForceFeedbackRef", AdvancedDisplay)
	TObjectPtr<UForceFeedbackEffect> FallbackForceFeedback = nullptr;

	bool IsValid() const
	{
		return ReferencedForceFeedbackName != NAME_None;
	}
	FModioForceFeedbackRef() : ReferencedForceFeedbackName(NAME_None), FallbackForceFeedback(nullptr) {};
	FModioForceFeedbackRef(FName ForceFeedbackName)
		: ReferencedForceFeedbackName(ForceFeedbackName), FallbackForceFeedback(nullptr) {};

protected:
	friend class UModioForceFeedbackRefLibrary;

	/**
	 * @brief Handler for force feedback table change events
	 */
	static FModioForceFeedbackRefChangeDelegate ForceFeedbackChangeHandlers;
};

/**
 * @brief Blueprint-compatible struct representing a force feedback reference override
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|ForceFeedbackRef Override")
struct FModioForceFeedbackRefOverride
{
	GENERATED_BODY()

	/**
	 * @brief Whether the force feedback reference is overridden
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle),
			  category = "mod.io|UI|ForceFeedbackRef Override")
	bool bOverride = false;

	/**
	 * @brief The force feedback to use if the reference is overridden
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bOverride", ShowOnlyInnerProperties),
			  category = "mod.io|UI|ForceFeedbackRef Override")
	FModioForceFeedbackRef ForceFeedback;
};

/**
 * @brief Library of functions for working with force feedback references
 */
UCLASS()
class MODIOUICORE_API UModioForceFeedbackRefLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Resolves a force feedback reference to a force feedback effect
	 * @param Target The force feedback reference to resolve
	 * @param bResolved True if the force feedback reference was resolved, false if the fallback effect was used
	 * @return The resolved force feedback effect
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|ForceFeedbackRef")
	static UForceFeedbackEffect* Resolve(const FModioForceFeedbackRef& Target, bool& bResolved);

	/**
	 * @brief Resolves a force feedback reference override to a force feedback effect
	 * @param Target The force feedback reference override to resolve
	 * @param bOverrideSet True if the force feedback reference was overridden, false otherwise
	 * @return The resolved force feedback effect, or null if the override is not set
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|ForceFeedbackRef",
			  meta = (DisplayName = "Resolve Force Feedback Override"))
	static UForceFeedbackEffect* ResolveOverride(const FModioForceFeedbackRefOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Resolves a force feedback reference override to a force feedback effect
	 * @param Target The force feedback reference override to resolve
	 * @param bOverrideSet True if the force feedback reference was overridden, false otherwise
	 * @return The resolved force feedback effect, or null if the override is not set
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ForceFeedbackRef",
			  meta = (DisplayName = "Resolve Force Feedback Override As Exec", ExpandEnumAsExecs = "bOverrideSet"))
	static UForceFeedbackEffect* ResolveOverrideAsExec(const FModioForceFeedbackRefOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Gets the list of valid force feedback keys
	 * @return The list of valid force feedback keys
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|ForceFeedbackRef")
	static TArray<FName> GetValidForceFeedbackKeys();

	/**
	 * @brief Adds a handler to be called when the force feedback table changes
	 * @param Handler The handler to add
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|ForceFeedbackRef")
	static void AddForceFeedbackChangeHandler(const FModioForceFeedbackRefChangeHandler& Handler);

	/**
	 * @brief Removes a handler from the list of handlers to be called when the force feedback table changes
	 * @param Handler The handler to remove
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|ForceFeedbackRef")
	static void RemoveForceFeedbackChangeHandler(const FModioForceFeedbackRefChangeHandler& Handler);
};
