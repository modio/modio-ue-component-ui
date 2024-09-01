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

#include "Blueprint/IUserObjectListEntry.h"
#include "Core/ModioPropertyOverrides.h"
#include "CoreMinimal.h"
#include "Internationalization/Text.h"

#include "ModioDialogButtonInfo.generated.h"

/**
 * @brief Data structure representing a dialog button
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|Dialog")
struct MODIOUICORE_API FModioDialogButtonInfo
{
	GENERATED_BODY()

	/**
	 * @brief Label for the button
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Dialog")
	FText ButtonLabel;

	/**
	 * @brief Style to apply to the button
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Dialog", meta = (MetaClass = "CommonButtonStyle"))
	FClassPathOverride StyleOverride;

	/**
	 * @brief Action to apply for the button click
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Dialog")
	FInputActionOverride InputActionOverride;

	/**
	 * @brief Whether to show a tooltip for the button
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Dialog")
	bool bShowTooltip = false;

	/**
	 * @brief Title for the tooltip, if shown
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Dialog",
			  meta = (EditConditionHides, EditCondition = "bShowToolTip"))
	FText TooltipTitleText;

	/**
	 * @brief Info text for the tooltip, if shown
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Dialog",
			  meta = (EditConditionHides, EditCondition = "bShowToolTip"))
	FText TooltipInfoText;

	/**
	 * @brief Tag text for the tooltip, if shown
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Dialog",
			  meta = (EditConditionHides, EditCondition = "bShowToolTip"))
	FText TooltipTagText;

	/**
	 * @brief Whether the button should be focused by default
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Dialog")
	bool bFocusedByDefault = false;
};

/**
 * @brief Data binding interface indicating that the implementing object can be queried for an associated FModioDialogButtonInfo
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding|Dialog")
class MODIOUICORE_API UModioDialogButtonUIDetails : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioDialogButtonUIDetails : public IInterface
{
	GENERATED_BODY()
	virtual FModioDialogButtonInfo GetDialogButtonInfo_Implementation()
	{
		return FModioDialogButtonInfo();
	};

public:
	/**
	 * @brief Queries the implementing object for the dialog button parameters that the object represents
	 * @return DialogButtonInfo associated with the object
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding|Dialog")
	FModioDialogButtonInfo GetDialogButtonInfo();
};

/**
 * @brief Wrapper around FModioDialogButtonInfo for data binding to UListView etc
 */
UCLASS(BlueprintType, Category = "mod.io|UI|Dialog")
class MODIOUICORE_API UModioModBrowserDialogButtonInfoUI : public UObject,
														   public IUserObjectListEntry,
														   public IModioDialogButtonUIDetails
{
	GENERATED_BODY()
protected:
	//~ Begin IModioDialogButtonUIDetails Interface
	virtual FModioDialogButtonInfo GetDialogButtonInfo_Implementation() override
	{
		return Underlying;
	}
	//~ End IModioDialogButtonUIDetails Interface

public:
	/**
	 * @brief Underlying data for the dialog button
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Dialog", meta = (ExposeOnSpawn = true))
	FModioDialogButtonInfo Underlying;
};
