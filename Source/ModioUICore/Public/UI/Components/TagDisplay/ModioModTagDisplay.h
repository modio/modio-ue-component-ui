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

#include "ModioModTagDisplay.generated.h"

/**
 * @default_impl_for Mod Tag Display
 * @brief A widget that displays the tags of a mod (e.g. "Action", "Adventure", "Singleplayer")
 * @component_display_name Mod Tag Display
 */
UCLASS()
class MODIOUICORE_API UModioModTagDisplay : public UModioUIComponentBase
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Get the widget that will contain the tags
	 * @return The widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIObjectListWidget> GetTagContainerWidget() const;

#if WITH_EDITORONLY_DATA
	/**
	 * @brief The data source to use for editor preview
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Data Binding|Editor Preview",
			  meta = (BlueprintProtected))
	TObjectPtr<UObject> EditorDataSource;
#endif

	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativeSetDataSource(UObject* InDataSource) override;
	//~ End IModioUIDataSourceWidget Interface

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface
};
