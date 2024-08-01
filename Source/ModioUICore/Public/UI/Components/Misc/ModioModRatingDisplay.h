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
#include "UI/Components/ModioUIComponentBase.h"

#include "ModioModRatingDisplay.generated.h"

class IModioUIHasTextWidget;

/**
 * @default_impl_for Mod Rating Display
 * @brief A widget that displays the rating of a mod
 * @component_display_name Mod Rating Display
 */
UCLASS()
class MODIOUICORE_API UModioModRatingDisplay : public UModioUIComponentBase
{
	GENERATED_BODY()
protected:
	/**
	 * @brief Get the label widget that displays the rating percentage
	 * @return The label widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetRatingPercentageLabelWidget() const;

	/**
	 * @brief Get the rating description label widget
	 * @return The label widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetRatingDescriptionLabelWidget() const;

	virtual void NativeSetDataSource(UObject* InDataSource) override;

#if WITH_EDITORONLY_DATA
	/**
	 * @brief The data source to use for editor preview
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Data Binding|Editor Preview",
			  meta = (BlueprintProtected))
	TObjectPtr<UObject> EditorDataSource;
#endif

	virtual void NativePreConstruct() override;
};
