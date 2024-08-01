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
#include "Engine/DataTable.h"
#include "Fonts/SlateFontInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Templates/SubclassOf.h"
#include "Types/SlateEnums.h"

#include "ModioPropertyOverrides.generated.h"

class UObject;

/**
 * @brief Struct for conveniently handling integer overrides in UMG components.
 * Allows conditional editing of an integer value based on the bOverride flag that is displayed inline as an edit
 * condition toggle. See UPropertyOverridesLibrary for functions to resolve and apply overrides (ResolveIntOverride,
 * ApplyIntOverride).
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|Overrides")
struct FIntegerOverride
{
	GENERATED_BODY()

	/**
	 * @brief Whether the value is overridden
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle),
			  Category = "mod.io|UI|Integer Override")
	bool bOverride = false;

	/**
	 * @brief The overridden value
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bOverride"),
			  Category = "mod.io|UI|Integer Override")
	int32 Value;
};

/**
 * @brief Struct for conveniently handling float overrides in UMG components.
 * Allows conditional editing of a float value based on the bOverride flag that is displayed inline as an edit condition
 * toggle. See UPropertyOverridesLibrary for functions to resolve and apply overrides (ResolveFloatOverride,
 * ApplyFloatOverride).
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|Overrides")
struct FFloatOverride
{
	GENERATED_BODY()

	/**
	 * @brief Whether the value is overridden
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle),
			  Category = "mod.io|UI|Float Override")
	bool bOverride = false;

	/**
	 * @brief The overridden value
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bOverride"),
			  Category = "mod.io|UI|Float Override")
	float Value;
};

/**
 * @brief Struct for conveniently handling text overrides in UMG components.
 * Allows conditional editing of a text value based on the bOverride flag that is displayed inline as an edit condition
 * toggle. See UPropertyOverridesLibrary for functions to resolve and apply overrides (ResolveTextOverride,
 * ApplyTextOverride).
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|Overrides")
struct FTextOverride
{
	GENERATED_BODY()

	/**
	 * @brief Whether the value is overridden
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle), Category = "mod.io|UI|Text Override")
	bool bOverride = false;

	/**
	 * @brief The overridden value
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bOverride"),
			  Category = "mod.io|UI|Text Override")
	FText Value;
};

/**
 * @brief Struct for conveniently handling input action overrides in UMG components.
 * Allows conditional editing of an input action value based on the bOverride flag that is displayed inline as an edit
 * condition toggle. See UPropertyOverridesLibrary for functions to resolve and apply overrides
 * (ResolveInputActionOverride, ApplyInputActionOverride).
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|Overrides")
struct FInputActionOverride
{
	GENERATED_BODY()

	/**
	 * @brief Whether the value is overridden
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle),
			  Category = "mod.io|UI|Input Action Override")
	bool bOverride = false;

	/**
	 * @brief The overridden value
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere,
			  meta = (EditCondition = "bOverride", RowType = CommonInputActionDataBase),
			  Category = "mod.io|UI|Input Action Override")
	FDataTableRowHandle Value;
};

/**
 * @brief Struct for conveniently handling class path overrides in UMG components.
 * Allows conditional editing of a class path value based on the bOverride flag that is displayed inline as an edit
 * condition toggle. See UPropertyOverridesLibrary for functions to resolve and apply overrides
 * (ResolveClassPathOverride, ApplyClassPathOverride).
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|Overrides")
struct FClassPathOverride
{
	GENERATED_BODY()

	/**
	 * @brief Whether the value is overridden
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle),
			  Category = "mod.io|UI|Class Path Override")
	bool bOverride = false;

	/**
	 * @brief The overridden value
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bOverride"),
			  Category = "mod.io|UI|Class Path Override")
	FSoftClassPath Value;
};

/**
 * @brief Struct for conveniently handling horizontal alignment overrides in UMG components.
 * Allows conditional editing of a horizontal alignment value based on the bOverride flag that is displayed inline as an
 * edit condition toggle. See UPropertyOverridesLibrary for functions to resolve and apply overrides
 * (ResolveHAlignOverride, ApplyHAlignOverride).
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|Overrides")
struct FHAlignOverride
{
	GENERATED_BODY()

	/**
	 * @brief Whether the value is overridden
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle),
			  Category = "mod.io|UI|Horizontal Alignment Override")
	bool bOverride = false;

	/**
	 * @brief The overridden value
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bOverride"),
			  Category = "mod.io|UI|Horizontal Alignment Override")
	TEnumAsByte<EHorizontalAlignment> Value;
};

/**
 * @brief Struct for conveniently handling font info overrides in UMG components.
 * Allows conditional editing of a font info value based on the bOverride flag that is displayed inline as an edit
 * condition toggle. See UPropertyOverridesLibrary for functions to resolve and apply overrides
 * (ResolveFontInfoOverride, ApplyFontInfoOverride).
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|Overrides")
struct FSlateFontInfoOverride
{
	GENERATED_BODY()

	/**
	 * @brief Whether the value is overridden
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle),
			  Category = "mod.io|UI|Font Info Override")
	bool bOverride = false;

	/**
	 * @brief The overridden value
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bOverride"),
			  Category = "mod.io|UI|Font Info Override")
	FSlateFontInfo Value;
};

/**
 * @brief Struct for conveniently handling text transform overrides in UMG components.
 * Allows conditional editing of a text transform value based on the bOverride flag that is displayed inline as an edit
 * condition toggle. See UPropertyOverridesLibrary for functions to resolve and apply overrides
 * (ResolveTextTransformOverride, ApplyTextTransformOverride).
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|Overrides")
struct FTextTransformOverride
{
	GENERATED_BODY()

	/**
	 * @brief Whether the value is overridden
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle),
			  Category = "mod.io|UI|Text Transform Override")
	bool bOverride = false;

	/**
	 * @brief The overridden value
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "bOverride"),
			  Category = "mod.io|UI|Text Transform Override")
	ETextTransformPolicy Value;
};

/**
 * @brief Library of functions for resolving and applying property overrides in UMG components.
 * These functions are intended to be used in UMG components to allow conditional editing of properties based on an
 * override flag.
 */
UCLASS()
class MODIOUICORE_API UPropertyOverridesLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Resolve an integer override value.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved integer value, or 0 if no override is set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(Integer)"))
	static int32 ResolveIntOverride(const FIntegerOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Resolve an integer override value, with an exec pin for flow control.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved integer value, or 0 if no override is set.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(Integer) As Exec", ExpandEnumAsExecs = "bOverrideSet"))
	static int32 ResolveIntOverrideAsExec(const FIntegerOverride& Target, bool& bOverrideSet);

	/**
	 * Apply an integer override value.
	 *
	 * @param Target The override structure containing the value to apply.
	 * @param DefaultValue The default value to use if no override is set.
	 * @return The overridden integer value or the default value.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Apply Override to Value(Integer)"))
	static int32 ApplyIntOverride(const FIntegerOverride& Target, int32 DefaultValue);

	/**
	 * @brief Resolve a float override value.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved float value, or 0.0 if no override is set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(Float)"))
	static float ResolveFloatOverride(const FFloatOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Resolve a float override value, with an exec pin for flow control.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved float value, or 0.0 if no override is set.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(Float) As Exec", ExpandEnumAsExecs = "bOverrideSet"))
	static float ResolveFloatOverrideAsExec(const FFloatOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Apply a float override value.
	 *
	 * @param Target The override structure containing the value to apply.
	 * @param DefaultValue The default value to use if no override is set.
	 * @return The overridden float value or the default value.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Apply Override to Value(Float)"))
	static float ApplyFloatOverride(const FFloatOverride& Target, float DefaultValue);

	/**
	 * @brief Resolve an input action override value.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved input action row handle, or an empty handle if no override is set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(InputAction)"))
	static FDataTableRowHandle ResolveInputActionOverride(const FInputActionOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Resolve an input action override value, with an exec pin for flow control.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved input action row handle, or an empty handle if no override is set.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(InputAction) As Exec", ExpandEnumAsExecs = "bOverrideSet"))
	static FDataTableRowHandle ResolveInputActionOverrideAsExec(const FInputActionOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Apply an input action override value.
	 *
	 * @param Target The override structure containing the value to apply.
	 * @param DefaultValue The default value to use if no override is set.
	 * @return The overridden input action row handle or the default value.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Apply Override to Value(InputAction)"))
	static FDataTableRowHandle ApplyInputActionOverride(const FInputActionOverride& Target,
														FDataTableRowHandle DefaultValue);

	/**
	 * @brief Resolve a text override value.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved text value, or an empty text if no override is set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(Text)"))
	static FText ResolveTextOverride(const FTextOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Resolve a text override value, with an exec pin for flow control.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved text value, or an empty text if no override is set.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(Text) As Exec", ExpandEnumAsExecs = "bOverrideSet"))
	static FText ResolveTextOverrideAsExec(const FTextOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Apply a text override value.
	 *
	 * @param Target The override structure containing the value to apply.
	 * @param DefaultValue The default value to use if no override is set.
	 * @return The overridden text value or the default value.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Apply Override to Value(Text)"))
	static FText ApplyTextOverride(const FTextOverride& Target, FText DefaultValue);

	/**
	 * @brief Resolve a class path override value.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved class path value, or an empty path if no override is set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(ClassPath)"))
	static FSoftClassPath ResolveClassPathOverride(const FClassPathOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Resolve a class path override value, with an exec pin for flow control.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved class path value, or an empty path if no override is set.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(ClassPath) As Exec", ExpandEnumAsExecs = "bOverrideSet"))
	static FSoftClassPath ResolveClassPathOverrideAsExec(const FClassPathOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Apply a class path override value.
	 *
	 * @param Target The override structure containing the value to apply.
	 * @param DefaultValue The default value to use if no override is set.
	 * @return The overridden class path value or the default value.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Apply Override to Value(ClassPath)"))
	static FSoftClassPath ApplyClassPathOverride(const FClassPathOverride& Target, FSoftClassPath DefaultValue);

	/**
	 * @brief Resolve a class path override value, casting it to a specific class.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param BaseClass The base class to check the loaded class against.
	 * @param bOverrideSet Whether the override is set.
	 * @return The loaded class if it matches the BaseClass, otherwise nullptr.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get and Load Override Value(ClassPath)", DeterminesOutputType = "BaseClass",
					  DynamicOutputParam = "ReturnValue", BaseClass = "/Script/CoreUObject.Object"))
	static UClass* ResolveClassPathOverride_Casted(const FClassPathOverride& Target, TSubclassOf<UObject> BaseClass,
												   bool& bOverrideSet);

	/**
	 * @brief Resolve a class path override value, casting it to a specific class, with an exec pin for flow control.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param BaseClass The base class to check the loaded class against.
	 * @param bOverrideSet Whether the override is set.
	 * @return The loaded class if it matches the BaseClass, otherwise nullptr.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get and Load Override Value(ClassPath) As Exec",
					  ExpandEnumAsExecs = "bOverrideSet", DeterminesOutputType = "BaseClass",
					  DynamicOutputParam = "ReturnValue", BaseClass = "/Script/CoreUObject.Object"))
	static UClass* ResolveClassPathOverrideAsExec_Casted(const FClassPathOverride& Target,
														 TSubclassOf<UObject> BaseClass, bool& bOverrideSet);

	/**
	 * @brief Apply a class path override value, casting it to a specific class.
	 *
	 * @param Target The override structure containing the value to apply.
	 * @param BaseClass The base class to check the loaded class against.
	 * @param DefaultValue The default value to use if no override is set.
	 * @return The overridden class if it matches the BaseClass, otherwise the default value.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Apply and Load Override to Value(ClassPath)", DeterminesOutputType = "BaseClass",
					  DynamicOutputParam = "ReturnValue", BaseClass = "/Script/CoreUObject.Object"))
	static UClass* ApplyClassPathOverride_Casted(const FClassPathOverride& Target, TSubclassOf<UObject> BaseClass,
												 UClass* DefaultValue);

	/**
	 * @brief Resolve a horizontal alignment override value.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved horizontal alignment value, or empty if no override is set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(HAlign)"))
	static EHorizontalAlignment ResolveHAlignOverride(const FHAlignOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Resolve a horizontal alignment override value, with an exec pin for flow control.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved horizontal alignment value, or empty if no override is set.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(HAlign) As Exec", ExpandEnumAsExecs = "bOverrideSet"))
	static EHorizontalAlignment ResolveHAlignOverrideAsExec(const FHAlignOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Apply a horizontal alignment override value.
	 *
	 * @param Target The override structure containing the value to apply.
	 * @param DefaultValue The default value to use if no override is set.
	 * @return The overridden horizontal alignment value or the default value.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Apply Override to Value(HAlign)"))
	static EHorizontalAlignment ApplyHAlignOverride(const FHAlignOverride& Target, EHorizontalAlignment DefaultValue);

	/**
	 * @brief Resolve a font info override value.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved font info value, or an empty font info if no override is set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(FSlateFontInfo)"))
	static FSlateFontInfo ResolveFontInfoOverride(const FSlateFontInfoOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Resolve a font info override value, with an exec pin for flow control.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved font info value, or an empty font info if no override is set.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(FSlateFontInfo) As Exec", ExpandEnumAsExecs = "bOverrideSet"))
	static FSlateFontInfo ResolveFontInfoOverrideAsExec(const FSlateFontInfoOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Apply a font info override value.
	 *
	 * @param Target The override structure containing the value to apply.
	 * @param DefaultValue The default value to use if no override is set.
	 * @return The overridden font info value or the default value.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Apply Override to Value(FSlateFontInfo)"))
	static FSlateFontInfo ApplyFontInfoOverride(const FSlateFontInfoOverride& Target, FSlateFontInfo DefaultValue);

	/**
	 * @brief Resolve a text transform policy override value.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved text transform policy value, or ETextTransformPolicy::ToUpper if no override is set.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(TextTransform)"))
	static ETextTransformPolicy ResolveTextTransformOverride(const FTextTransformOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Resolve a text transform policy override value, with an exec pin for flow control.
	 *
	 * @param Target The override structure containing the value to resolve.
	 * @param bOverrideSet Whether the override is set.
	 * @return The resolved text transform policy value, or ETextTransformPolicy::ToUpper if no override is set.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Get Override Value(TextTransform) As Exec", ExpandEnumAsExecs = "bOverrideSet"))
	static ETextTransformPolicy ResolveTextTransformOverrideAsExec(const FTextTransformOverride& Target,
																   bool& bOverrideSet);

	/**
	 * @brief Apply a text transform policy override value.
	 *
	 * @param Target The override structure containing the value to apply.
	 * @param DefaultValue The default value to use if no override is set.
	 * @return The overridden text transform policy value or the default value.
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Overrides",
			  meta = (DisplayName = "Apply Override to Value(TextTransform)"))
	static ETextTransformPolicy ApplyTextTransformOverride(const FTextTransformOverride& Target,
														   ETextTransformPolicy DefaultValue);
};
