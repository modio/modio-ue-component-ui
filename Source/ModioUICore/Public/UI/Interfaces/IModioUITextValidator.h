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

#include "Internationalization/Regex.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/EngineVersionComparison.h"
#include "UObject/Interface.h"
#include "UObject/ScriptInterface.h"

#if UE_VERSION_OLDER_THAN(5, 2, 0)
	PRAGMA_DISABLE_OPTIMIZATION
#else
	UE_DISABLE_OPTIMIZATION
#endif

#include "IModioUITextValidator.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FModioOnTextValidationCompleted, bool, bIsValid);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioOnTextValidationCompletedMulticast, bool, bIsValid);

/**
 * @brief Enumeration for the possible validation rules
 * ScriptName meta specifier suppresses a duplicate name LogPython warning
 */
UENUM(BlueprintType, meta = (ScriptName = "ModioTextRuleForValidation"), Category = "mod.io|UI|Rules")
enum class EModioTextValidationRule : uint8
{
	/** Validate as an email  **/
	ETVR_ValidateEmailAddress,

	/** Validate that every character is a letter **/
	ETVR_ValidateAllLetters,

	/** Validate that every character is a number **/
	ETVR_ValidateAllNumeric,

	/** Validate characters as numbers or letters **/
	ETVR_ValidateAlphanumeric,

	/** Validate the length of the stream **/
	ETVR_ValidateLength,

	/** Validate that has some content **/
	ETVR_ValidateNotEmpty,

	/** Validate the lack of spaces **/
	ETVR_ValidateNoWhitespace
};

/**
 * @brief Strong type struct with the necessary details to create a text validation rule
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|Rules")
struct MODIOUICORE_API FModioTextValidationRule
{
	GENERATED_BODY()

	/**
	 * @brief The base rule to use in this instance. One of the many listed in EModioTextValidationRule
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "mod.io|UI|Rules")
	EModioTextValidationRule RuleToUse {};

	/**
	 * @brief Message to display if validation fails.Use "{InputText} as a placeholder for the text being validated"
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "mod.io|UI|Rules")
	FText ValidationMessage;

	/**
	 * @brief SApply the validation rule to at least minimum lenght
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "RuleToUse==EModioTextValidationRule::ETVR_ValidateLength", EditConditionHides),
			  category = "mod.io|UI|Rules")
	int32 MinimumLength {};

	/**
	 * @brief SApply the validation rule to a maximum lenght
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere,
			  meta = (EditCondition = "RuleToUse==EModioTextValidationRule::ETVR_ValidateLength", EditConditionHides),
			  category = "mod.io|UI|Rules")
	int32 MaximumLength {};
};

/**
 * @brief Interface indicating that the implementing widget can be configured with validation rules to check text.
 * The widget can optionally visualize a validation error if one occurs, or requested to show a specific error from and
 * external source.
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Text Validator",Meta = (DocGroup = "UI",Comment = "@brief Interface indicating that the implementing widget can be configured with validation rules to check text. The widget can optionally visualize a validation error if one occurs, or requested to show a specific error from and external source."))
class MODIOUICORE_API UModioUITextValidator : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUITextValidator : public IInterface
{
	GENERATED_BODY()

protected:
	// Default interface method implementations. Will only be called for native C++ classes implementing
	// IModioUITextValidator.

	virtual void GetTextValidationRules_Implementation(TArray<FModioTextValidationRule>& Rules);
	virtual bool ValidateText_Implementation(const FText& InputText, FText& ValidationMessageText);
	virtual void SetValidationError_Implementation(const FText& ErrorText);

public:
	/**
	 * @brief SChange the validation error with new text
	 * @param ErrorText The new string to set on this instance
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, category = "mod.io|UI|Text Validator")
	void SetValidationError(const FText& ErrorText);

	/**
	 * @brief SRetrieve the validation rules stored in this instance
	 * @param Rules Reference array where the rules will be stored
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, category = "mod.io|UI|Text Validator")
	void GetTextValidationRules(TArray<FModioTextValidationRule>& Rules);

	/**
	 * @brief Performs validation on the text passed in, storing the validation message of the first rule that failed
	 * validation in ValidationMessageText if validation failed
	 * @param InputText the text to validate
	 * @param ValidationMessageText FText containing the localised failure message
	 * @returns true if the text passed all the validation rules
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, category = "mod.io|UI|Events|Text Validator")
	bool ValidateText(const FText& InputText, FText& ValidationMessageText);

	/**
	 * @brief SRegisters a delegate to receive callbacks when text validation completes
	 * @param Handler Delegate to invoke on a text validation event
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Text Validator")
	void AddTextValidationCompletedHandler(const FModioOnTextValidationCompleted& Handler);

	/**
	 * @brief Unregisters a delegate so it no longer receives callbacks
	 * @param Handler Delegate that will no longer receive text validation events
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Text Validator")
	void RemoveTextValidationCompletedHandler(const FModioOnTextValidationCompleted& Handler);
};

/**
 * @brief Static library of functions to validate text input
 */
UCLASS()
class MODIOUICORE_API UModioUIInputValidationLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/**
	 * @brief SConfirm the text provided matches an email address
	 * @param TextToValidate String to check against email pattern
	 * @return True when string contains a valid email, false otherwise
	 */
	UFUNCTION(BlueprintCallable, category = "mod.io|UI|ModioUIInputValidationLibrary")
	static bool ValidateEmailAddress(const FText& TextToValidate);

	/**
	 * @brief SConfirm the text provided matches an all letters rule
	 * @param TextToValidate String to check against all letters rule
	 * @return True when string validation passes, false otherwise
	 */
	UFUNCTION(BlueprintCallable, category = "mod.io|UI|ModioUIInputValidationLibrary")
	static bool ValidateAllLetters(const FText& TextToValidate);

	/**
	 * @brief SConfirm the text provided matches an all numeric rule
	 * @param TextToValidate String to check against all numeric rule
	 * @return True when string validation passes, false otherwise
	 */
	UFUNCTION(BlueprintCallable, category = "mod.io|UI|ModioUIInputValidationLibrary")
	static bool ValidateAllNumeric(const FText& TextToValidate);

	/**
	 * @brief SConfirm the text provided matches the alphanumeric rule
	 * @param TextToValidate String to check against the alphanumeric rule
	 * @return True when string validation passes, false otherwise
	 */
	UFUNCTION(BlueprintCallable, category = "mod.io|UI|ModioUIInputValidationLibrary")
	static bool ValidateAlphanumeric(const FText& TextToValidate);

	/**
	 * @brief SConfirm the text provided matches the length rule
	 * @param TextToValidate String to check against the length rule
	 * @return True when string validation passes, false otherwise
	 */
	UFUNCTION(BlueprintCallable, category = "mod.io|UI|ModioUIInputValidationLibrary")
	static bool ValidateLength(const FText& TextToValidate, int32 DesiredMaximumLength, int32 DesiredMinimumLength);

	/**
	 * @brief SConfirm the text provided matches the not-empty rule
	 * @param TextToValidate String to check against the not-empty rule
	 * @return True when string validation passes, false otherwise
	 */
	UFUNCTION(BlueprintCallable, category = "mod.io|UI|ModioUIInputValidationLibrary")
	static bool ValidateNotEmpty(const FText& TextToValidate);

	/**
	 * @brief SConfirm the text provided matches the lack of whitespace rule
	 * @param TextToValidate String to check against the lack of whitespace rule
	 * @return True when string validation passes, false otherwise
	 */
	UFUNCTION(BlueprintCallable, category = "mod.io|UI|ModioUIInputValidationLibrary")
	static bool ValidateNoWhitespace(const FText& TextToValidate);

	/**
	 * @brief SConvenience function to check a rule against a text and returns a validation text when the rule was not
	 * matched
	 * @param Rule The text validation rule to apply
	 * @param TextToValidate The string to apply a rule to
	 * @param ValidationMessageText A string that receives a formatted value when rule checks fail
	 * @return True when string validation passes, false otherwise
	 */
	UFUNCTION(BlueprintCallable, category = "mod.io|UI|ModioUIInputValidationLibrary")
	static bool ValidateUsingRule(const FModioTextValidationRule& Rule, const FText& TextToValidate,
								  FText& ValidationMessageText);

	/**
	 * @brief Helper function that contains the default implementation of ValidateText for widgets implementing
	 * IModioUITextValidator. This implmentation is called by default for native classes that implement
	 * IModioUITextValidator, but requires an explicit call for classes that implement that interface via Blueprint only
	 * @param TargetObject Object that implements IModioUITextValidator and will receive `GetTextValidationRules` and
	 * `SetValidationError` interface calls as necessary
	 * @param InputText The text to validate
	 * @param ValidationMessageText FText that will be initialized by this function to an appropriate error message if
	 * validation fails
	 * @return True if validation succeeded, false if it failed
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|ModioUITextValidator", Meta = (DefaultToSelf = "TargetObject"))
	static bool DefaultValidateText(TScriptInterface<IModioUITextValidator> TargetObject, const FText& InputText,
									FText& ValidationMessageText);
};

#if UE_VERSION_OLDER_THAN(5, 2, 0)
	PRAGMA_ENABLE_OPTIMIZATION
#else
	UE_ENABLE_OPTIMIZATION
#endif