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

#include "Types/ModioModInfo.h"
#include "UObject/Interface.h"

#include "IModioModTagUIDetails.generated.h"

/**
 * @brief Data binding interface indicating that the implementing object can be queried for the properties of an associated tag value
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding|Mod Tag", Meta = (DocGroup = "Data Binding Interfaces"))
class MODIOUICORE_API UModioModTagUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioModTagUIDetails : public IInterface
{
	GENERATED_BODY()

protected:
	virtual FString GetRawStringValue_Implementation()
	{
		return {};
	}

	virtual FText GetLocalizedText_Implementation()
	{
		return {};
	}

	virtual bool GetSelectionState_Implementation()
	{
		return false;
	}

	virtual void SetSelectionState_Implementation(bool bNewSelectionState) {}

public:
	/**
	 * @brief Queries the implementing object for the raw string value for the mod tag that the object represents
	 * @return raw string value for the mod tag
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Tag")
	FString GetRawStringValue();

	/**
	 * @brief Queries the implementing object for the localized text for the mod tag that the object represents
	 * @return localized text value for the mod tag
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Tag")
	FText GetLocalizedText();

	/**
	 * @brief Queries the implementing object for the selection state of the associated tag, if applicable
	 * @return The selection state for the tag
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Tag")
	bool GetSelectionState();

	/**
	 * @brief Sets the selection state of the associated tag
	 * @param bNewSelectionState The new selection state for the tag
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Tag")
	void SetSelectionState(bool bNewSelectionState);
};

/**
 * @modio_ui_component_interface
 * @brief Data binding interface indicating that the implementing object can be queried for an associated tag category
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding|Mod Tag Category")
class MODIOUICORE_API UModioModTagCategoryUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioModTagCategoryUIDetails : public IInterface
{
	GENERATED_BODY()

protected:
	virtual FString GetRawCategoryName_Implementation()
	{
		return {};
	}

	virtual FText GetLocalizedCategoryName_Implementation()
	{
		return {};
	}

	virtual bool GetAllowMultipleSelection_Implementation()
	{
		return false;
	}

	virtual TArray<TScriptInterface<UModioModTagUIDetails>> GetCategoryTags_Implementation()
	{
		return {};
	}

	virtual TArray<FString> GetSelectedTags_Implementation()
	{
		TArray<FString> SelectedTags;
		for (TScriptInterface<UModioModTagUIDetails>& CurrentTag : Execute_GetCategoryTags(Cast<UObject>(this)))
		{
			if (CurrentTag.GetObject() &&
				CurrentTag.GetObject()->GetClass()->ImplementsInterface(UModioModTagUIDetails::StaticClass()))
			{
				if (IModioModTagUIDetails::Execute_GetSelectionState(CurrentTag.GetObject()))
				{
					SelectedTags.Add(IModioModTagUIDetails::Execute_GetRawStringValue(CurrentTag.GetObject()));
				}
			}
		}
		return SelectedTags;
	}

public:
	/**
	 * @brief Queries the implementing object for the raw string name for the tag category that the object represents
	 * @return raw string name for the tag category
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Tag Category")
	FString GetRawCategoryName();

	/**
	 * @brief Queries the implementing object for the localized name for the tag category that the object represents
	 * @return localized name for the tag category
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Tag Category")
	FText GetLocalizedCategoryName();

	/**
	 * @brief Queries the implementing object for whether the associated tag category allows multiple selection
	 * @return True if the category permits multiple values to be selected
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Tag Category")
	bool GetAllowMultipleSelection();

	/**
	 * @brief Gets the bindable tag objects for this category
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Tag Category")
	TArray<TScriptInterface<UModioModTagUIDetails>> GetCategoryTags();

	/**
	 * @brief Retrieves an array of strings representing tags which are selected for the associated tag category
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Tag Category")
	TArray<FString> GetSelectedTags();
};

/**
 * @modio_ui_component_interface
 * @brief Data binding interface indicating that the implementing object can be queried for an associated tag category
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding|Mod Tag Options")
class MODIOUICORE_API UModioModTagOptionsUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioModTagOptionsUIDetails : public IInterface
{
	GENERATED_BODY()

protected:
	virtual TArray<TScriptInterface<UModioModTagCategoryUIDetails>> GetCategories_Implementation()
	{
		return {};
	}

	virtual TArray<FString> GetSelectedTags_Implementation()
	{
		TArray<FString> SelectedTags;
		for (TScriptInterface<UModioModTagCategoryUIDetails>& CurrentCategory :
			 Execute_GetCategories(Cast<UObject>(this)))
		{
			if (CurrentCategory.GetObject() && CurrentCategory.GetObject()->GetClass()->ImplementsInterface(
												   UModioModTagCategoryUIDetails::StaticClass()))
			{
				for (const FString& CurrentTag :
					 IModioModTagCategoryUIDetails::Execute_GetSelectedTags(CurrentCategory.GetObject()))
				{
					SelectedTags.Add(CurrentTag);
				}
			}
		}
		return SelectedTags;
	}

	virtual void ClearSelectedTags_Implementation()
	{
		for (TScriptInterface<UModioModTagCategoryUIDetails>& CurrentCategory :
			 Execute_GetCategories(Cast<UObject>(this)))
		{
			if (CurrentCategory.GetObject() && CurrentCategory.GetObject()->GetClass()->ImplementsInterface(
												   UModioModTagCategoryUIDetails::StaticClass()))
			{
				for (const TScriptInterface<UModioModTagUIDetails>& CurrentTag :
					 IModioModTagCategoryUIDetails::Execute_GetCategoryTags(CurrentCategory.GetObject()))
				{
					IModioModTagUIDetails::Execute_SetSelectionState(CurrentTag.GetObject(), false);
				}
			}
		}
	}

public:
	/**
	 * @brief Retrieves an array of objects representing the tag categories this game has configured
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Tag Options")
	TArray<TScriptInterface<UModioModTagCategoryUIDetails>> GetCategories();

	/**
	 * @brief Retrieves an array of strings representing tags which are selected across all categories
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Tag Options")
	TArray<FString> GetSelectedTags();

	/**
	 * @brief Clears all selected tags across all categories
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Mod Tag Options")
	void ClearSelectedTags();
};
