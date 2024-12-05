/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "ModioUISettings.h"

#include "Internationalization/StringTableRegistry.h"
#include "Types/ModioFilterParams.h"

UModioUISettings::UModioUISettings()
{
	FModioPresetFilterParams SortTrending;
	SortTrending.PresetName = LOCTABLE("/ModioComponentUI/UI/Data/ST_ModioModBrowserDefaultText.ST_ModioModBrowserDefaultText", "SortTrending");
	SortTrending.Direction = EModioSortDirection::Descending;
	SortTrending.SortField = EModioSortFieldType::DownloadsToday;
	SortTrending.Count = 100;
	PresetNamedFilters.Add(MoveTemp(SortTrending));

	FModioPresetFilterParams SortRecent;
	SortRecent.PresetName = LOCTABLE("/ModioComponentUI/UI/Data/ST_ModioModBrowserDefaultText.ST_ModioModBrowserDefaultText", "SortRecent");
	SortRecent.Direction = EModioSortDirection::Descending;
	SortRecent.SortField = EModioSortFieldType::DateMarkedLive;
	SortRecent.Count = 100;
	PresetNamedFilters.Add(MoveTemp(SortRecent));
}