

#include "UI/Interfaces/IModioUITextValidator.h"

bool IModioUITextValidator::ValidateText_Implementation(const FText& InputText, FText& ValidationMessageText)
{
	return UModioUIInputValidationLibrary::DefaultValidateText(Cast<UObject>(this), InputText, ValidationMessageText);
}

void IModioUITextValidator::GetTextValidationRules_Implementation(TArray<FModioTextValidationRule>& Rules)
{
	static TArray<FModioTextValidationRule> DefaultRules;
	DefaultRules.Add(FModioTextValidationRule {});
	Rules = DefaultRules;
}

void IModioUITextValidator::SetValidationError_Implementation(const FText& ErrorText) {}

bool UModioUIInputValidationLibrary::ValidateEmailAddress(const FText& TextToValidate)
{
	// Regex for validating email address found here: https://mylittledevblog.com/2018/02/15/ue4-email-validation/
	// I would prefer to use https://stackoverflow.com/a/201378/12018052, but that doesn't work
	const FRegexPattern Pattern(TEXT("^\\w+([-+.']\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*$"));
	FRegexMatcher Matcher(Pattern, TextToValidate.ToString());

	return Matcher.FindNext();
}

bool UModioUIInputValidationLibrary::ValidateAllLetters(const FText& TextToValidate)
{
	FString TmpString = TextToValidate.ToString();
	for (TCHAR CurrentCharacter : TmpString)
	{
		if (!FChar::IsAlpha(CurrentCharacter))
		{
			return false;
		}
	}
	return true;
}

bool UModioUIInputValidationLibrary::ValidateAllNumeric(const FText& TextToValidate)
{
	return TextToValidate.IsNumeric();
}

bool UModioUIInputValidationLibrary::ValidateAlphanumeric(const FText& TextToValidate)
{
	FString TmpString = TextToValidate.ToString();
	for (TCHAR CurrentCharacter : TmpString)
	{
		if (!FChar::IsAlnum(CurrentCharacter))
		{
			return false;
		}
	}
	return true;
}

bool UModioUIInputValidationLibrary::ValidateLength(const FText& TextToValidate, int32 DesiredMaximumLength,
													int32 DesiredMinimumLength)
{
	return TextToValidate.ToString().Len() <= DesiredMaximumLength &&
		   TextToValidate.ToString().Len() >= DesiredMinimumLength;
}

bool UModioUIInputValidationLibrary::ValidateNotEmpty(const FText& TextToValidate)
{
	FString testString = TextToValidate.ToString();
	testString.ReplaceInline(TEXT("\r\n"), TEXT(" "));
	testString.RemoveSpacesInline();
	return !testString.IsEmpty();
}

bool UModioUIInputValidationLibrary::ValidateNoWhitespace(const FText& TextToValidate)
{
	FString TmpString = TextToValidate.ToString();
	for (TCHAR CurrentCharacter : TmpString)
	{
		if (FChar::IsWhitespace(CurrentCharacter))
		{
			return false;
		}
	}
	return true;
}

bool UModioUIInputValidationLibrary::ValidateNotBlank(const FText& TextToValidate)
{
	FString TmpString = TextToValidate.ToString();
	for (TCHAR CurrentCharacter : TmpString)
	{
		if (!FChar::IsWhitespace(CurrentCharacter))
		{
			return true;
		}
	}
	return false;
}

bool UModioUIInputValidationLibrary::ValidateUsingRule(const FModioTextValidationRule& Rule,
                                                       const FText& TextToValidate, FText& ValidationMessageText)
{
	switch (Rule.RuleToUse)
	{
		case EModioTextValidationRule::ETVR_ValidateEmailAddress:
			if (!ValidateEmailAddress(TextToValidate))
			{
				ValidationMessageText =
					FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
				return false;
			}
			else
			{
				return true;
			}
			break;
		case EModioTextValidationRule::ETVR_ValidateAllLetters:
			if (!ValidateAllLetters(TextToValidate))
			{
				ValidationMessageText =
					FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
				return false;
			}
			else
			{
				return true;
			}
			break;
		case EModioTextValidationRule::ETVR_ValidateAllNumeric:
			if (!ValidateAllNumeric(TextToValidate))
			{
				ValidationMessageText =
					FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
				return false;
			}
			else
			{
				return true;
			}
			break;
		case EModioTextValidationRule::ETVR_ValidateAlphanumeric:
			if (!ValidateAlphanumeric(TextToValidate))
			{
				ValidationMessageText =
					FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
				return false;
			}
			else
			{
				return true;
			}
			break;
		case EModioTextValidationRule::ETVR_ValidateLength:
			if (!ValidateLength(TextToValidate, Rule.MaximumLength, Rule.MinimumLength))
			{
				ValidationMessageText = FText::Format(
					Rule.ValidationMessage,
					FFormatNamedArguments {{"InputText", FText::FromString(FString::FromInt(
															 TextToValidate.ToString().Len() - Rule.MaximumLength))}});
				return false;
			}
			else
			{
				return true;
			}
			break;
		case EModioTextValidationRule::ETVR_ValidateNotEmpty:
			if (!ValidateNotEmpty(TextToValidate))
			{
				ValidationMessageText =
					FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
				return false;
			}
			else
			{
				return true;
			}
			break;
		case EModioTextValidationRule::ETVR_ValidateNoWhitespace:
			if (!ValidateNoWhitespace(TextToValidate))
			{
				ValidationMessageText =
					FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
				return false;
			}
			else
			{
				return true;
			}
			break;
		case EModioTextValidationRule::ETVR_ValidateNotBlank:
			if (!ValidateNotBlank(TextToValidate))
			{
				ValidationMessageText =
					FText::Format(Rule.ValidationMessage, FFormatNamedArguments {{"InputText", TextToValidate}});
				return false;
			}
			else
			{
				return true;
			}
	}
	return true;
}

bool UModioUIInputValidationLibrary::DefaultValidateText(TScriptInterface<IModioUITextValidator> TargetObject,
														 const FText& InputText, FText& ValidationMessageText)
{
	TArray<FModioTextValidationRule> Rules;
	IModioUITextValidator::Execute_GetTextValidationRules(TargetObject.GetObject(), Rules);
	for (const FModioTextValidationRule& CurrentRule : Rules)
	{
		if (!UModioUIInputValidationLibrary::ValidateUsingRule(CurrentRule, InputText, ValidationMessageText))
		{
			IModioUITextValidator::Execute_SetValidationError(TargetObject.GetObject(), ValidationMessageText);
			return false;
		}
	}

	FText Empty;

	IModioUITextValidator::Execute_SetValidationError(TargetObject.GetObject(), ValidationMessageText);
	return true;
}
