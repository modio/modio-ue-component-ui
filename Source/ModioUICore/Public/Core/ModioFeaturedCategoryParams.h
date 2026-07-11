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
#include "Types/ModioFilterParams.h"
#include "UI/Interfaces/IModioModFilterUIDetails.h"

#include "ModioFeaturedCategoryParams.generated.h"

/**
 * @brief Struct wrapping common query settings to be visualized as a featured search
 */
USTRUCT(BlueprintType)
struct MODIOUICORE_API FModioFeaturedCategoryParams
{
	GENERATED_BODY()

	/**
	 * @brief Name of the category (e.g. "Popular Mods")
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "mod.io|UI|Featured Category Params")
	FText CategoryName;

	/**
	 * @brief Tags to include in the filter (e.g. "RPG", "Fantasy")
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "mod.io|UI|Featured Category Params")
	TArray<FString> Tags;

	/**
	 * @brief Tags to exclude from the filter (e.g. "RPG", "Fantasy")
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "mod.io|UI|Featured Category Params")
	TArray<FString> ExcludedTags;

	/**
	 * @brief Sort direction for the category
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "mod.io|UI|Featured Category Params")
	EModioSortDirection Direction = EModioSortDirection::Ascending;

	/**
	 * @brief Sort field for the category
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "mod.io|UI|Featured Category Params")
	EModioSortFieldType SortField = EModioSortFieldType::ID;

	/**
	 * @brief Number of mods to display in the category
	 */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "mod.io|UI|Featured Category Params")
	int64 Count = 20;

	FModioFilterParams ToFilterParams() const
	{
		return FModioFilterParams()
			.WithTags(Tags)
			.WithoutTags(ExcludedTags)
			.SortBy(SortField, Direction)
			.IndexedResults(0, uint64(Count));
	}
};

/**
 * @brief Wrapper around FModioCommonFeaturedCategoryParams for data binding to UListView etc
 */
UCLASS(BlueprintType, Category = "mod.io|UI|ModioFeaturedCategoryParamsUI")
class MODIOUICORE_API UModioFeaturedCategoryParamsUI : public UObject, public IModioModFilterUIDetails
{
	GENERATED_BODY()
protected:
	//~ Begin IModioModFilterUIDetails Interface
	virtual FModioFilterParams GetFilterParams_Implementation() override
	{
		return Underlying.ToFilterParams();
	}
	virtual bool GetFilterName_Implementation(FText& Name) override
	{
		Name = Underlying.CategoryName;
		return true;
	}
	//~ End IModioModFilterUIDetails Interface

public:
	/**
	 * @brief Underlying data for the category params
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioFeaturedCategoryParamsUI")
	FModioFeaturedCategoryParams Underlying;
};
