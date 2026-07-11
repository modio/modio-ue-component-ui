/*
 *  Copyright (C) 2024-2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "ModioUISettings.h"

#include "Internationalization/StringTableRegistry.h"
#include "ModioUISubsystem.h"
#include "Types/ModioFilterParams.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioUISettings)

UModioUISettings::UModioUISettings()
{
	const FString FeaturedLargeCarouselWidgetClassPath =
		TEXT("/ModioComponentUI/UI/Templates/Default/ModViews/"
			 "WBP_ModView_CarouselFeaturedLarge_Looping.WBP_ModView_CarouselFeaturedLarge_Looping_C");
	const FString FeaturedCarouselWidgetClassPath =
		TEXT("/ModioComponentUI/UI/Templates/Default/ModViews/"
			 "WBP_ModView_CarouselFeatured_Looping.WBP_ModView_CarouselFeatured_Looping_C");
	const FString RegularCarouselWidgetClassPath =
		TEXT("/ModioComponentUI/UI/Templates/Default/ModViews/"
			 "WBP_ModView_CarouselRegular_Looping.WBP_ModView_CarouselRegular_Looping_C");
	const FString RegularGridWidgetClassPath = TEXT("/ModioComponentUI/UI/Templates/Default/ModViews/"
													"WBP_ModView_GridRegular.WBP_ModView_GridRegular_C");

	FModioPresetFilterParams SortTopRated;
	SortTopRated.PresetName =
		LOCTABLE("/ModioComponentUI/UI/Data/ST_ModioModBrowserDefaultText.ST_ModioModBrowserDefaultText", "Rating");
	SortTopRated.Direction = EModioSortDirection::Descending;
	SortTopRated.SortField = EModioSortFieldType::Rating;
	SortTopRated.RevenueField = EModioRevenueFilterType::FreeAndPaid;
	SortTopRated.TileWidgetClass = TSoftClassPtr<UWidget>(FSoftObjectPath(FeaturedLargeCarouselWidgetClassPath));
	SortTopRated.Count = 10;

	FModioPresetFilterParams SortTopSellers;
	SortTopSellers.PresetName =
		LOCTABLE("/ModioComponentUI/UI/Data/ST_ModioModBrowserDefaultText.ST_ModioModBrowserDefaultText", "TopSellers");
	SortTopSellers.Direction = EModioSortDirection::Descending;
	SortTopSellers.SortField = EModioSortFieldType::DownloadsToday;
	SortTopSellers.RevenueField = EModioRevenueFilterType::Paid;
	SortTopSellers.TileWidgetClass = TSoftClassPtr<UWidget>(FSoftObjectPath(FeaturedCarouselWidgetClassPath));
	SortTopSellers.Count = 20;

	FModioPresetFilterParams SortRecentlyAdded;
	SortRecentlyAdded.PresetName =
		LOCTABLE("/ModioComponentUI/UI/Data/ST_ModioModBrowserDefaultText.ST_ModioModBrowserDefaultText", "SortRecent");
	SortRecentlyAdded.Direction = EModioSortDirection::Descending;
	SortRecentlyAdded.SortField = EModioSortFieldType::DateUpdated;
	SortRecentlyAdded.RevenueField = EModioRevenueFilterType::FreeAndPaid;
	SortRecentlyAdded.TileWidgetClass = TSoftClassPtr<UWidget>(FSoftObjectPath(RegularCarouselWidgetClassPath));
	SortRecentlyAdded.Count = 20;

	FModioPresetFilterParams SortRecentlyAddedFeatured = SortRecentlyAdded;
	SortRecentlyAddedFeatured.TileWidgetClass =
		TSoftClassPtr<UWidget>(FSoftObjectPath(FeaturedCarouselWidgetClassPath));

	FModioPresetFilterParams SortTrending;
	SortTrending.PresetName = LOCTABLE(
		"/ModioComponentUI/UI/Data/ST_ModioModBrowserDefaultText.ST_ModioModBrowserDefaultText", "SortTrending");
	SortTrending.Direction = EModioSortDirection::Descending;
	SortTrending.SortField = EModioSortFieldType::DownloadsToday;
	SortTrending.RevenueField = EModioRevenueFilterType::FreeAndPaid;
	SortTrending.TileWidgetClass = TSoftClassPtr<UWidget>(FSoftObjectPath(RegularCarouselWidgetClassPath));
	SortTrending.Count = 20;

	FModioPresetFilterParams SortMostPopular;
	SortMostPopular.PresetName = LOCTABLE(
		"/ModioComponentUI/UI/Data/ST_ModioModBrowserDefaultText.ST_ModioModBrowserDefaultText", "SortPopular");
	SortMostPopular.Direction = EModioSortDirection::Descending;
	SortMostPopular.SortField = EModioSortFieldType::DownloadsTotal;
	SortMostPopular.RevenueField = EModioRevenueFilterType::FreeAndPaid;
	SortMostPopular.TileWidgetClass = TSoftClassPtr<UWidget>(FSoftObjectPath(RegularCarouselWidgetClassPath));
	SortMostPopular.Count = 20;

	PresetNamedFilters = {SortTopRated, SortRecentlyAddedFeatured, SortTrending, SortMostPopular};
	MarketplaceEnabledPresetNamedFilters = {SortTopRated, SortTopSellers, SortRecentlyAdded, SortTrending,
											SortMostPopular};

	DefaultCollectionsFilter.PresetName = LOCTABLE(
		"/ModioComponentUI/UI/Data/ST_ModioModBrowserDefaultText.ST_ModioModBrowserDefaultText", "Page_Collections");
	DefaultCollectionsFilter.Direction = EModioSortDirection::Descending;
	DefaultCollectionsFilter.SortField = EModioSortFieldType::SubscriberCount;
	DefaultCollectionsFilter.Count = 100;
}

FModioPresetFilterParams UModioUISettings::GetDefaultCollectionsFilter()
{
	UModioUISettings* Settings = GetMutableDefault<UModioUISettings>();
	return Settings->DefaultCollectionsFilter;
}

const TArray<FModioPresetFilterParams>& UModioUISettings::GetPresetNamedFilters()
{
	UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
	const bool bMarketplaceEnabled =
		UISubsystem && UISubsystem->IsUGCFeatureEnabled(EModioUIFeatureFlags::Monetization);

	const UModioUISettings* Settings = GetDefault<UModioUISettings>();
	return bMarketplaceEnabled ? Settings->MarketplaceEnabledPresetNamedFilters : Settings->PresetNamedFilters;
}

const FModioPresetFilterParams& UModioUISettings::GetPresetFilterByName(const FText& PresetFilterName, bool& bSuccess)
{
	for (const FModioPresetFilterParams& FilterParam : GetPresetNamedFilters())
	{
		if (FilterParam.PresetName.EqualTo(PresetFilterName))
		{
			bSuccess = true;
			return FilterParam;
		}
	}

	bSuccess = false;
	static FModioPresetFilterParams Invalid;
	return Invalid;
}
