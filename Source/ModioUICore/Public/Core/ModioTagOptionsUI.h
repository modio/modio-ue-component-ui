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

#include "Algo/Transform.h"
#include "CoreMinimal.h"
#include "Types/ModioModTagOptions.h"
#include "UI/Interfaces/IModioModTagUIDetails.h"
#include "UObject/NoExportTypes.h"

#include "ModioTagOptionsUI.generated.h"

/// Unlike most of the wrappers in the mod.io plugin, these types contain some additional fields
/// Because tags and tag categories are free-form and specified by developers, they are not localized by the mod.io
/// backend. As a result these wrappers provide fields for storing the localized text provided by your game's
/// localization implementation alongside the actual underlying raw string that the REST API expects in search queries
/// and filters. In addition, because the most common implementation of a tag selector widget dynamically constructs
/// subwidgets for each tag, the selection state needs to be persisted outside of the widget itself, so Mod Tag UI
/// objects contain the selection state so that it persists even when tags are being shown in virtualized lists.

/**
 * @brief Wrapper around a Tag Entry (string) when consumer needs UObject-representable data (e.g. for UMG as a list item or data source)
 */
UCLASS(BlueprintType, Category = "mod.io|UI|ModioTagUI")
class MODIOUICORE_API UModioModTagUI : public UObject, public IModioModTagUIDetails
{
	GENERATED_BODY()

public:
	/**
	 * @brief Raw tag string used in the mod.io API
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioTagUI")
	FString Underlying;

	/**
	 * @brief Localized tag string
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioTagUI")
	FText LocalizedText;

	/**
	 * @brief Transient field storing selection state
	 */
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioTagUI")
	bool bSelectionState;

protected:
	//~ Begin IModioModTagUIDetails Interface
	virtual FString GetRawStringValue_Implementation() override
	{
		return Underlying;
	}
	virtual FText GetLocalizedText_Implementation() override
	{
		return LocalizedText;
	}
	virtual void SetSelectionState_Implementation(bool bNewSelectionState) override
	{
		bSelectionState = bNewSelectionState;
	}
	virtual bool GetSelectionState_Implementation() override
	{
		return bSelectionState;
	}
	//~ End IModioModTagUIDetails Interface
};

/**
 * @brief Wrapper around a FModioModTagInfo when consumer needs UObject-representable data (e.g. for UMG as a list item or data source)
 */
UCLASS(BlueprintType, Category = "mod.io|UI|ModioTagInfoUI")
class MODIOUICORE_API UModioModTagInfoUI : public UObject, public IModioModTagCategoryUIDetails
{
	GENERATED_BODY()

protected:
	//~ Begin IModioModTagCategoryUIDetails Interface
	virtual FString GetRawCategoryName_Implementation() override
	{
		return Underlying.TagGroupName;
	}
	virtual FText GetLocalizedCategoryName_Implementation() override
	{
		return LocalizedCategoryName;
	}
	virtual TArray<TScriptInterface<UModioModTagUIDetails>> GetCategoryTags_Implementation() override
	{
		TArray<TScriptInterface<UModioModTagUIDetails>> TagInterfaces;
		for (UModioModTagUI* CurrentTag : BindableTags)
		{
			TagInterfaces.Add(TScriptInterface<UModioModTagUIDetails>(CurrentTag));
		}
		return TagInterfaces;
	}
	virtual bool GetAllowMultipleSelection_Implementation() override
	{
		return Underlying.bAllowMultipleSelection;
	}
	virtual bool GetIsCategoryHidden_Implementation() override
	{
		return Underlying.bHidden;
	}
	virtual bool GetIsCategoryLocked_Implementation() override
	{
		return Underlying.bLocked;
	}
	//~ End IModioModTagCategoryUIDetails Interface

public:
	/**
	 * @brief Underlying tag info
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioTagInfoUI")
	FModioModTagInfo Underlying;

	/**
	 * @brief Localized tag category string
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioTagInfoUI")
	FText LocalizedCategoryName;

	/**
	 * @brief Transient set of bindable tag objects for use in the UI
	 */
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioTagInfoUI")
	TArray<TObjectPtr<UModioModTagUI>> BindableTags;
};

/**
 * @brief Wrapper around a FModioModTagOptions because UMG widgets expect UObjects for data sources or list items
 */
UCLASS(BlueprintType)
class MODIOUICORE_API UModioModTagOptionsUI : public UObject, public IModioModTagOptionsUIDetails
{
	GENERATED_BODY()

protected:
	//~ Begin IModioModTagOptionsUIDetails Interface
	virtual TArray<TScriptInterface<UModioModTagCategoryUIDetails>> GetCategories_Implementation() override
	{
		TArray<TScriptInterface<UModioModTagCategoryUIDetails>> CategoryInterfaces;
		for (UModioModTagInfoUI* CurrentCategory : BindableTagCategories)
		{
			CategoryInterfaces.Add(TScriptInterface<UModioModTagCategoryUIDetails>(CurrentCategory));
		}
		return CategoryInterfaces;
	}
	virtual TArray<FString> GetSelectedTags_Implementation() override
	{
		TArray<FString> SelectedTags;
		for (UModioModTagInfoUI* CurrentCategory : BindableTagCategories)
		{
			SelectedTags.Insert(IModioModTagCategoryUIDetails::Execute_GetSelectedTags(CurrentCategory),
								SelectedTags.Num());
		}
		return SelectedTags;
	}
	//~ End IModioModTagOptionsUIDetails Interface

public:
	/**
	 * @brief Underlying tag options
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioTagOptionsUI")
	FModioModTagOptions Underlying;

	/**
	 * @brief Transient set of bindable tag categories for use in the UI
	 */
	UPROPERTY(Transient, BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ModioModTagOptionsUI")
	TArray<TObjectPtr<UModioModTagInfoUI>> BindableTagCategories;
};
