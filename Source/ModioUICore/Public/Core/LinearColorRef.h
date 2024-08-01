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

#include "Containers/Map.h"
#include "Delegates/Delegate.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Math/Color.h"
#include "UObject/UnrealNames.h"

#include "LinearColorRef.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLinearColorRefColorChangeDelegate);
DECLARE_DYNAMIC_DELEGATE(FLinearColorRefColorChangeHandler);

/**
 * @brief Struct representing a color entry in a data table
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|ColorRef")
struct MODIOUICORE_API FModioUIColorEntry : public FTableRowBase
{
	GENERATED_BODY();

	/**
	 * @brief The color to use for the entry
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, category = "mod.io UI Color Entry")
	FLinearColor Color;
};

/**
 * @brief Blueprint-compatible truct representing a color reference
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|ColorRef")
struct MODIOUICORE_API FLinearColorRef
{
	GENERATED_BODY();

	/**
	 * @brief The name of the color to reference
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "mod.io|UI|ColorRef")
	FName ReferencedColorName;

	/**
	 * @brief The color to use if the reference is not found
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ColorRef", AdvancedDisplay)
	FLinearColor FallbackColor;

	bool IsValid() const
	{
		return ReferencedColorName != NAME_None;
	}
	FLinearColorRef() : ReferencedColorName(NAME_None), FallbackColor(FLinearColor::White) {};
	FLinearColorRef(FName ColorName) : ReferencedColorName(ColorName), FallbackColor(FLinearColor::White) {};

protected:
	friend class ULinearColorRefLibrary;

	/**
	 * @brief Handler for color change events
	 */
	static FLinearColorRefColorChangeDelegate ColorChangeHandlers;
};

/**
 * @brief Blueprint-compatible struct representing a color reference override
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|ColorRef Override")
struct FLinearColorRefOverride
{
	GENERATED_BODY()

	/**
	 * @brief Whether the color reference is overridden
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle),
			  category = "mod.io|UI|ColorRef Override")
	bool bOverride = false;

	/**
	 * @brief The color to use if the reference is overridden
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bOverride", ShowOnlyInnerProperties),
			  category = "mod.io|UI|ColorRef Override")
	FLinearColorRef Color;
};

/**
 * @brief Library of functions for working with color references
 */
UCLASS()
class MODIOUICORE_API ULinearColorRefLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Resolves a color reference to a color value
	 * @param Target The color reference to resolve
	 * @param bResolved True if the color reference was resolved, false if the fallback color was used
	 * @return The resolved color value
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|ColorRef")
	static FLinearColor Resolve(const FLinearColorRef& Target, bool& bResolved);

	/**
	 * @brief Resolves a color reference to a color value
	 * @param Target The color reference to resolve
	 * @param bOverrideSet True if the color reference was overridden, false if the fallback color was used
	 * @return The resolved color value
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|ColorRef",
			  meta = (DisplayName = "Resolve Color Override"))
	static FLinearColor ResolveOverride(const FLinearColorRefOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Resolves a color reference to a color value
	 * @param Target The color reference to resolve
	 * @param bOverrideSet True if the color reference was overridden, false if the fallback color was used
	 * @return The resolved color value
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ColorRef",
			  meta = (DisplayName = "Resolve Color Override As Exec", ExpandEnumAsExecs = "bOverrideSet"))
	static FLinearColor ResolveOverrideAsExec(const FLinearColorRefOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Gets the list of valid color keys
	 * @return The list of valid color keys
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|ColorRef")
	static TArray<FName> GetValidColorKeys();

	/**
	 * @brief Adds a handler to be called when the color table changes
	 * @param Handler The handler to add
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|ColorRef")
	static void AddColorChangeHandler(const FLinearColorRefColorChangeHandler& Handler);

	/**
	 * @brief Removes a handler from the list of handlers to be called when the color table changes
	 * @param Handler The handler to remove
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|ColorRef")
	static void RemoveColorChangeHandler(const FLinearColorRefColorChangeHandler& Handler);
};
