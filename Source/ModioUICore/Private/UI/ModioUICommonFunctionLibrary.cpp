/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/ModioUICommonFunctionLibrary.h"
#include "Core/ModioModDependencyUI.h"
#include "Core/ModioPropertyOverrides.h"
#include "Core/ModioUserUI.h"
#include "Internationalization/StringTable.h"
#include "Libraries/ModioModTagOptionsLibrary.h"
#include "ModioSettings.h"
#include "ModioSubsystem.h"
#include "Components/TextBlock.h"
#include "Libraries/ModioSDKLibrary.h"
#include "UObject/UnrealType.h"
#include "Loc/ModioEnumLocalizationHelpers.h"
#include "UI/Interfaces/IModioModDependencyUIDetails.h"

FText UModioUICommonFunctionLibrary::FormatTimespan(const FTimespan& Timespan, const FText& YearFormat,
                                                    const FText& MonthFormat, const FText& DaysFormat,
                                                    const FText& TodayFormat)
{
	int32 Minutes = Timespan.GetMinutes();
	int32 Hours = Timespan.GetHours();
	int32 Days = Timespan.GetDays();
	int32 Months = Timespan.GetDays() / 30;
	int32 Years = Timespan.GetDays() / 365;

	if (Years > 0)
	{
		return FText::FormatOrdered(YearFormat, Years, Months, Days, Hours, Minutes);
	}
	else if (Months > 0)
	{
		return FText::FormatOrdered(MonthFormat, Years, Months, Days, Hours, Minutes);
	}
	else if (Days > 0)
	{
		return FText::FormatOrdered(DaysFormat, Years, Months, Days, Hours, Minutes);
	}

	return FText::FormatOrdered(TodayFormat, Years, Months, Days, Hours, Minutes);
}

FText UModioUICommonFunctionLibrary::FormatNumberWithSIUnit(int64 Value, const FText& FormatText)
{
	FFormatOrderedArguments Args;

	static const TCHAR* Prefixes = TEXT("KMGTPEZY");
	int32 Prefix = 0;
	if (Value < 1000)
	{
		Args.Add(FText::AsNumber(Value));
		Args.Add(FText());
		return FText::Format(FormatText, Args);
	}
	while (Value >= (1000000))
	{
		Value /= 1000;
		++Prefix;
	}
	FNumberFormattingOptions Options {};
	Options.MaximumFractionalDigits = 1;
	const double FractionalValue = (double) Value / (double) 1000;
	Args.Add(FText::AsNumber(FractionalValue, &Options));
	Args.Add(FText::FromString(FString(1, &Prefixes[Prefix])));
	return FText::Format(FormatText, Args);
}

FText UModioUICommonFunctionLibrary::GetLocalizedTagCategory(const FModioModTagInfo& InTagInfo)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		const FString LanguageCode = UModioSDKLibrary::GetLanguageCodeString(Subsystem->GetLanguage());
		if (const FText* LocalizedText = InTagInfo.TagGroupNameLocalizationData.Find(LanguageCode))
		{
			return *LocalizedText;
		}
	}
	return FText::FromString(InTagInfo.TagGroupName);
}

bool UModioUICommonFunctionLibrary::CheckLogoDownloadEventForDataSource(FModioModID EventModID,
																		EModioLogoSize EventLogoSize,
																		UObject* DataSource,
																		EModioLogoSize RequestedSize)
{
	if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
	{
		FModioModID RequestedID = IModioModInfoUIDetails::Execute_GetModID(DataSource);
		return (EventModID == RequestedID) && (EventLogoSize == RequestedSize);
	}
	return false;
}

bool UModioUICommonFunctionLibrary::CheckLogoDownloadEvent(FModioModID EventModID, EModioLogoSize EventLogoSize,
														   FModioModID RequestedModID, EModioLogoSize RequestedSize)
{
	return (EventModID == RequestedModID) && (EventLogoSize == RequestedSize);
}

bool UModioUICommonFunctionLibrary::MapValueIntoRangeIndex(const TArray<FInt32Range>& ValueRanges, int32 Value,
														   int32& OutIndex)
{
	OutIndex = -1;
	for (int32 RangeIndex = 0; RangeIndex < ValueRanges.Num(); RangeIndex++)
	{
		if (ValueRanges[RangeIndex].Contains(Value))
		{
			OutIndex = RangeIndex;
			return true;
		}
	}
	return false;
}

UModioModTagOptionsUI* UModioUICommonFunctionLibrary::CreateBindableModTagOptions(
	const FModioModTagOptions& InRawOptions)
{
	UModioModTagOptionsUI* BindableOptions = NewObject<UModioModTagOptionsUI>();
	BindableOptions->Underlying = InRawOptions;
	TArray<UModioModTagInfoUI*> BoundTagCategories;
	for (const FModioModTagInfo& CurrentTagCategory : UModioModTagOptionsLibrary::GetTags(InRawOptions))
	{
		BoundTagCategories.Add(CreateBindableModTagInfo(CurrentTagCategory));
	}
	BindableOptions->BindableTagCategories = BoundTagCategories;
	return BindableOptions;
}

UModioModTagInfoUI* UModioUICommonFunctionLibrary::CreateBindableModTagInfo(const FModioModTagInfo& InRawTagInfo)
{
	UModioModTagInfoUI* BindableTagInfo = NewObject<UModioModTagInfoUI>();
	BindableTagInfo->Underlying = InRawTagInfo;
	BindableTagInfo->LocalizedCategoryName = GetLocalizedTagCategory(InRawTagInfo);
	TArray<UModioModTagUI*> BoundTagValues;
	for (const FModioModTagLocalizationData& CurrentTag : InRawTagInfo.TagGroupValueLocalizationData)
	{
		BoundTagValues.Add(CreateBindableModTag(CurrentTag));
	}
	BindableTagInfo->BindableTags = BoundTagValues;
	return BindableTagInfo;
}

UModioModTagUI* UModioUICommonFunctionLibrary::CreateBindableModTag(const FModioModTagLocalizationData& InTagLocalizationData)
{
	UModioModTagUI* BindableTag = NewObject<UModioModTagUI>();
	BindableTag->bSelectionState = false;
	BindableTag->Underlying = InTagLocalizationData.Tag;
	BindableTag->LocalizedText = GetLocalizedTagValue(InTagLocalizationData);
	return BindableTag;
}

UModioModTagUI* UModioUICommonFunctionLibrary::CreateBindableModTagRawValue(const FModioModTag& InTag)
{
	UModioModTagUI* NewWrappedTag = NewObject<UModioModTagUI>();
	NewWrappedTag->Underlying = InTag.Tag;
	NewWrappedTag->LocalizedText = InTag.TagLocalized;
	// If your project uses runtime localization for mod tags, do it here - will likely need to do a lookup to get the
	// category of the tag and pass that to GetLocalizedTagValue
	return NewWrappedTag;
}

TArray<UModioModTagUI*> UModioUICommonFunctionLibrary::CreateBindableModTagArray(const TArray<FModioModTag>& InTags)
{
	TArray<UModioModTagUI*> OutBoundTags;
	Algo::Transform(InTags, OutBoundTags, &CreateBindableModTagRawValue);
	return OutBoundTags;
}

UModioUserUI* UModioUICommonFunctionLibrary::CreateBindableUser(const FModioUser& InRawUser)
{
	UModioUserUI* NewUserObject = NewObject<UModioUserUI>();
	NewUserObject->Underlying = InRawUser;
	return NewUserObject;
}

UModioEnumEntryUI* UModioUICommonFunctionLibrary::CreateBindableEnum(const uint8 InValue, const FText InText)
{
	UModioEnumEntryUI* NewEnumObject = NewObject<UModioEnumEntryUI>();
	NewEnumObject->UnderlyingEnumValue = InValue;
	NewEnumObject->UnderlyingEnumValueDisplayText = InText;
	return NewEnumObject;
}

TArray<UModioEnumEntryUI*> UModioUICommonFunctionLibrary::CreateBindableReportTypeEnumArray()
{
	TArray<UModioEnumEntryUI*> OutArray;

	for (EModioReportType ReportType : TEnumRange<EModioReportType>())
	{
		uint8 EnumValue = static_cast<uint8>(ReportType);
		FText EntryText = UModioUIEnumLocalizationLibrary::GetLocalizedTextForEnumByName(
			StaticEnum<EModioReportType>()->GetNameByValue(EnumValue));

		OutArray.Add(CreateBindableEnum(EnumValue, EntryText));
	}
	
	return OutArray;
}

UModioModDependencyUI* UModioUICommonFunctionLibrary::CreateBindableModDependency(const FModioModDependency& InModDependency)
{
	UModioModDependencyUI* BindableTag = NewObject<UModioModDependencyUI>();
	BindableTag->Underlying = InModDependency;
	return BindableTag;
}

TArray<UModioModDependencyUI*> UModioUICommonFunctionLibrary::CreateBindableModDependencyArrayFromArray(const TArray<FModioModDependency>& InModDependencyArray)
{
	TArray<UModioModDependencyUI*> OutBoundTags;
	Algo::Transform(InModDependencyArray, OutBoundTags, &CreateBindableModDependency);
	return OutBoundTags;
}

TArray<UModioModDependencyUI*> UModioUICommonFunctionLibrary::CreateBindableModDependencyArrayFromList(const FModioModDependencyList& InModDependencyList)
{
	TArray<UModioModDependencyUI*> OutBoundTags;
	Algo::Transform(InModDependencyList.InternalList, OutBoundTags, &CreateBindableModDependency);
	return OutBoundTags;
}

FText UModioUICommonFunctionLibrary::GetModioText(FName StringKey)
{
	if (const UModioSettings* PluginSettings = GetDefault<UModioSettings>())
	{
		UStringTable* ModioStringTableAsset = Cast<UStringTable>(PluginSettings->LocalizationStringTable.TryLoad());

		if (ModioStringTableAsset)
		{
			auto StaticLocDataTable = ModioStringTableAsset->GetMutableStringTable();
			return FText::FromStringTable(ModioStringTableAsset->GetStringTableId(), StringKey.ToString());
		}
	}
	return {};
}

bool UModioUICommonFunctionLibrary::IsDesignTime(UWidget* Target)
{
	return (Target && Target->IsDesignTime());
}

bool UModioUICommonFunctionLibrary::IsDesignTimeAsExec(UWidget* Target)
{
	return IsDesignTime(Target);
}

void UModioUICommonFunctionLibrary::ApplyTextPropertyOverridesHelper(
	const struct FSlateFontInfoOverride& FontPropertiesOverride,
	const struct FHAlignOverride& TextJustificationOverride, const struct FTextTransformOverride& TextTransformOverride,
	UObject* Target)
{
	// TODO: This should use a configuration interface rather than relying on the underlying type
	UTextBlock* UnderlyingWidget = Cast<UTextBlock>(Target);
	if (UnderlyingWidget)
	{
		bool bFontPropertiesSet = false;
		FSlateFontInfo ResolvedFontProps =
			UPropertyOverridesLibrary::ResolveFontInfoOverride(FontPropertiesOverride, bFontPropertiesSet);
		if (bFontPropertiesSet)
		{
			UnderlyingWidget->SetFont(ResolvedFontProps);
		}
		bool bTextJustificationSet = false;
		EHorizontalAlignment ResolvedAlignment =
			UPropertyOverridesLibrary::ResolveHAlignOverride(TextJustificationOverride, bTextJustificationSet);
		if (bTextJustificationSet)
		{
			switch (ResolvedAlignment)
			{
				case HAlign_Center:
				case HAlign_Fill:
					UnderlyingWidget->SetJustification(ETextJustify::Center);
					break;
				case HAlign_Left:
					UnderlyingWidget->SetJustification(ETextJustify::Left);
					break;
				case HAlign_Right:
					UnderlyingWidget->SetJustification(ETextJustify::Right);
					break;
			}
		}
		bool bTextTransformSet = false;
		ETextTransformPolicy ResolvedTransform =
			UPropertyOverridesLibrary::ResolveTextTransformOverride(TextTransformOverride, bTextTransformSet);
		if (bTextTransformSet)
		{
			UnderlyingWidget->SetTextTransformPolicy(ResolvedTransform);
		}
	}
}

#if WITH_EDITORONLY_DATA
bool& UModioUICommonFunctionLibrary::GetLoadPreviewDataFlagMutable()
{
	static bool InternalValue = false;
	return InternalValue;
}

void UModioUICommonFunctionLibrary::SetLoadPreviewDataOverride(bool bEnabled)
{
	GetLoadPreviewDataFlagMutable() = bEnabled;
}

bool UModioUICommonFunctionLibrary::ShouldLoadPreviewData()
{
	return GetLoadPreviewDataFlagMutable();
}
#endif

FText UModioUICommonFunctionLibrary::GetLocalizedTagValue(const FModioModTagLocalizationData& InTagLocalizationData)
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		const FString LanguageCode = UModioSDKLibrary::GetLanguageCodeString(Subsystem->GetLanguage());
		if (const FText* LocalizedText = InTagLocalizationData.Translations.Find(LanguageCode))
		{
			return *LocalizedText;
		}
	}
	return FText::FromString(InTagLocalizationData.Tag);
}

DEFINE_FUNCTION(UModioUICommonFunctionLibrary::execAsObjectArray)
{
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FArrayProperty>(NULL);
	void* ArrayAddr = Stack.MostRecentPropertyAddress;
	FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Stack.MostRecentProperty);
	if (!ArrayProperty)
	{
		Stack.bArrayContextFailed = true;
		return;
	}

	P_FINISH;
	P_NATIVE_BEGIN;
	*(TArray<UObject*>*) RESULT_PARAM = BPCastInterfaceToObjectArrayHelper(ArrayAddr, ArrayProperty);
	P_NATIVE_END;
}

DEFINE_FUNCTION(UModioUICommonFunctionLibrary::execValidateWidget)
{
	Stack.MostRecentProperty = nullptr;
	Stack.StepCompiledIn<FInterfaceProperty>(NULL);
	void* InterfaceAddr = Stack.MostRecentPropertyAddress;
	FInterfaceProperty* InterfaceProperty = CastField<FInterfaceProperty>(Stack.MostRecentProperty);
	if (!InterfaceProperty)
	{
		Stack.bAbortingExecution = true;
		return;
	}

	P_FINISH;
	P_NATIVE_BEGIN;

	// if InterfaceClass member is ever treated as private, get the name from the property and load the uclass by name
	// FString ActualClassName;
	// InterfaceProperty->GetCPPMacroType(ActualClassName);
	// ActualClassName.RightChopInline(1);
	// UClass::TryFindTypeSlow<UClass>(*ActualClassName);

	const FScriptInterface& ScriptInterface = InterfaceProperty->GetPropertyValue(InterfaceAddr);
	UObject* WidgetObject = ScriptInterface.GetObject();
	if (WidgetObject && WidgetObject->GetClass()->ImplementsInterface(InterfaceProperty->InterfaceClass))
	{
		*(UWidget**) RESULT_PARAM = Cast<UWidget>(WidgetObject);
	}
	else
	{
		*(UWidget**) RESULT_PARAM = nullptr;
	}
	P_NATIVE_END;
}