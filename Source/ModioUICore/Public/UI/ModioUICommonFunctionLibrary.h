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

#include "Components/Widget.h"
#include "Core/ModioEnumEntryUI.h"
#include "Core/ModioTagOptionsUI.h"
#include "CoreMinimal.h"
#include "Interfaces/IModioModInfoUIDetails.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioCommonTypes.h"
#include "Types/ModioModDependencyList.h"
#include "Types/ModioModTagOptions.h"
#include "Types/ModioReportParams.h"
#include "Engine.h"

#include "ModioUICommonFunctionLibrary.generated.h"

/**
 * @brief Common utility functions intended for use with the mod.io UI Components
 */
UCLASS()
class MODIOUICORE_API UModioUICommonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

protected:
	static TArray<UObject*> BPCastInterfaceToObjectArrayHelper(void* TargetArray, const FArrayProperty* ArrayProp)
	{
		TArray<UObject*> OutArray;
		if (TargetArray)
		{
			FScriptArrayHelper ArrayHelper(ArrayProp, TargetArray);
			for (int32 CurrentIndex = 0; CurrentIndex < ArrayHelper.Num(); ++CurrentIndex)
			{
				FScriptInterface* CurrentElement =
					reinterpret_cast<FScriptInterface*>(ArrayHelper.GetRawPtr(CurrentIndex));
				OutArray.Add(CurrentElement->GetObject());
			}
		}
		return OutArray;
	}

public:
	/// @brief Formats the provided timespan to text based on the specified resolution and specified format texts. This
	/// function passes positional values to the format text in the following order: Years, Months, Days, Hours,
	/// Minutes.
	/// @param Timespan The timespan to format
	/// @param YearFormat Format text to use if the timespan is > 1 year
	/// @param MonthFormat Format text to use if the timespan is > 1 month
	/// @param DaysFormat Format text to use if the timespan is > 1 day
	/// @param TodayFormat Format text to use if the time is today
	/// @return The formatted text
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Localization")
	static FText FormatTimespan(const FTimespan& Timespan, const FText& YearFormat, const FText& MonthFormat,
								const FText& DaysFormat, const FText& TodayFormat);

	/// @brief Localizes the specified value using the supplied formatting options and format string. Will calculate the
	/// appropriate SI unit and pass a character indicating that unit to the format call.
	/// @param Value The raw value to localize
	/// @param FormatText FText specifying the formatted output pattern. This will be passed the value as placeholder
	/// 0 and the units as placeholder 1
	/// @return The formatted text
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Localization")
	static FText FormatNumberWithSIUnit(int64 Value, const FText& FormatText);

	/// @brief Localizes the specified tag category
	/// @param InTagInfo The tag info from which to localize the category
	/// @return The localized category name
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Localization")
	static FText GetLocalizedTagCategory(const FModioModTagInfo& InTagInfo);

	/// @brief Localizes the specified tag value
	/// @param InTagLocalizationData The tag localization data from which to localize the tag value
	/// @return The localized tag value
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Localization")
	static FText GetLocalizedTagValue(const FModioModTagLocalizationData& InTagLocalizationData);

	/// @brief Utility method to validate if a logo download event is relevant for the given data source and requested
	/// size
	/// @param EventModID The event's associated mod ID
	/// @param EventLogoSize The event's associated download size
	/// @param DataSource The data source containing the relevant mod ID
	/// @param RequestedSize the size that the widget requested
	/// @return
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool CheckLogoDownloadEventForDataSource(FModioModID EventModID, EModioLogoSize EventLogoSize,
													UObject* DataSource, EModioLogoSize RequestedSize);

	/// @brief Utility method to validate if a logo download event is relevant for the given mod ID and requested size
	/// @param EventModID The event's associated mod ID
	/// @param EventLogoSize The event's associated download size
	/// @param RequestedModID The relevant mod ID for the widget that is making the check
	/// @param RequestedSize The relevant size for the widget that is making the check
	/// @return
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool CheckLogoDownloadEvent(FModioModID EventModID, EModioLogoSize EventLogoSize, FModioModID RequestedModID,
									   EModioLogoSize RequestedSize);

	/// @brief Searches the specified value ranges and returns the index of the first range which contains the provided
	/// value
	/// @param ValueRanges The ranges to search through
	/// @param Value The value to locate
	/// @param OutIndex The index of the first range that contains Value
	/// @return True if the value was located
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities", meta = (ExpandBoolAsExecs = "ReturnValue"))
	static bool MapValueIntoRangeIndex(const TArray<FInt32Range>& ValueRanges, int32 Value, int32& OutIndex);

	/// @brief Creates a data-bindable UObject wrapper around a FModioModTagOptions struct, including child UObjects
	/// that are data-bindable wrappers for individual tag categories and tags
	/// @param InRawOptions The struct containing tag data to bind to
	/// @return UObject wrapper suitable for use as a DataSource
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities|Data Binding")
	static UModioModTagOptionsUI* CreateBindableModTagOptions(const FModioModTagOptions& InRawOptions);

	/// @brief Creates a data-bindable UObject wrapper around a FModioModTagInfo struct (aka a tag category), including
	/// child UObjects that are data-bindable wrappers for individual tags
	/// @param InRawTagInfo The struct containing tag data to bind to
	/// @return UObject wrapper suitable for use as a DataSource
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities|Data Binding")
	static UModioModTagInfoUI* CreateBindableModTagInfo(const FModioModTagInfo& InRawTagInfo);

	/// @brief Creates a data-bindable UObject wrapper around an individual mod tag value
	/// @param InTagLocalizationData The tag value to make bindable
	/// @return UObject suitable for use as a DataSource
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities|Data Binding")
	static UModioModTagUI* CreateBindableModTag(const FModioModTagLocalizationData& InTagLocalizationData);

	/// @brief Creates a data-bindable UObject wrapper around a mod tag sourced from a FModioModInfo
	/// @param InTag The tag to make bindable
	/// @return UObject suitable for use as a DataSource
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities|Data Binding")
	static UModioModTagUI* CreateBindableModTagRawValue(const FModioModTag& InTag);

	/// @brief Creates an array of data-bindable UObject wrappers around mod tags coming from a FModioModInfo, ie for
	/// display on the mod details
	/// @param InTags The array of tags to make bindable
	/// @return Array of UObjects suitable for use as a DataSource
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities|Data Binding")
	static TArray<UModioModTagUI*> CreateBindableModTagArray(const TArray<FModioModTag>& InTags);
	
	/// @brief Creates a data-bindable UObject wrapper around an individual mod dependency value
	/// @param InModDependency The raw string value of the tag
	/// @return UObject suitable for use as a DataSource
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities|Data Binding")
	static UModioModDependencyUI* CreateBindableModDependency(const FModioModDependency& InModDependency);

	/// @brief Creates an array of data-bindable UObject wrappers around mod dependencies coming from an array
	/// @param InModDependencyArray The array of tags to make bindable
	/// @return Array of UObjects suitable for use as a DataSource
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities|Data Binding")
	static TArray<UModioModDependencyUI*> CreateBindableModDependencyArrayFromArray(const TArray<FModioModDependency>& InModDependencyArray);

	/// @brief Creates an array of data-bindable UObject wrappers around mod dependencies coming from a FModioModDependencyList
	/// @param InModDependencyList The array of tags to make bindable
	/// @return Array of UObjects suitable for use as a DataSource
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities|Data Binding")
	static TArray<UModioModDependencyUI*> CreateBindableModDependencyArrayFromList(const FModioModDependencyList& InModDependencyList);

	/// @brief
	///	@param
	///	@return
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities|Data Binding")
	static UModioUserUI* CreateBindableUser(const FModioUser& InRawUser);

	/// @brief
	///	@param
	///	@return
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities|Data Binding")
	static UModioEnumEntryUI* CreateBindableEnum(const uint8 InValue, const FText InText);

	/// @brief
	///	@param
	///	@return
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities|Data Binding")
	static TArray<UModioEnumEntryUI*> CreateBindableReportTypeEnumArray();

	template<typename InterfaceType>
	static TArray<UObject*> NativeConvertInterfaceToObjectArray(const TArray<TScriptInterface<InterfaceType>>& InArray)
	{
		TArray<UObject*> OutObjArray;
		Algo::Transform(InArray, OutObjArray, [](const TScriptInterface<InterfaceType>& InInterfaceRef) {
			return InInterfaceRef.GetObject();
		});
		return OutObjArray;
	}

	/// @brief Converts an array of Interface references to an array of UObjects
	/// @param TargetArray The array containing Interface references
	/// @return Array of UObject*
	UFUNCTION(BlueprintCallable, CustomThunk, meta = (ArrayParm = "TargetArray"),
			  Category = "mod.io|UI|Utilities|Array")
	static TArray<UObject*> AsObjectArray(const TArray<int32>& TargetArray)
	{
		check(0); // should never execute
		return {};
	}
	DECLARE_FUNCTION(execAsObjectArray);

	UFUNCTION(BlueprintCallable, CustomThunk, Category = "mod.io|UI")
	static UWidget* ValidateWidget(TScriptInterface<UInterface> TargetWidget)
	{
		check(0); // should never execute
		return {};
	}
	DECLARE_FUNCTION(execValidateWidget);

	/// @brief Utility method to dynamically resolve an FText from the primary mod.io string table
	/// @param StringKey Name of the text to retrieve
	/// @return The resolved FText or empty FText if not found
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Localization")
	static FText GetModioText(FName StringKey);

	/// @brief Returns true if the widget is currently being viewed in the UMG editor
	/// @param Target The widget to check design-time for
	/// @return True if Target has design-time flags set
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|Utilities", meta = (DefaultToSelf = "Target"))
	static bool IsDesignTime(UWidget* Target);

	/// @brief Returns true if the widget is currently being viewed in the UMG editor
	/// @param Target The widget to check design-time for
	/// @return True if Target has design-time flags set
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities",
			  meta = (DefaultToSelf = "Target", ExpandEnumAsExecs = "ReturnValue"))
	static bool IsDesignTimeAsExec(UWidget* Target);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Utilities", meta = (DefaultToSelf = "Target"))
	static void ApplyTextPropertyOverridesHelper(const struct FSlateFontInfoOverride& FontPropertiesOverride,
												 const struct FHAlignOverride& TextJustificationOverride,
												 const struct FTextTransformOverride& TextTransformOverride,
												 UObject* Target);

#if WITH_EDITORONLY_DATA
private:
	static bool& GetLoadPreviewDataFlagMutable();

public:
	static void SetLoadPreviewDataOverride(bool bEnabled);
	static bool ShouldLoadPreviewData();
#endif
};