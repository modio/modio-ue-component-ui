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

#include "ModioModPropCollectionVisualizer.generated.h"

/**
 * @default_impl_for Mod Property Collection Visualizer
 * @brief A widget that manages the visualization of a collection of mod properties
 * @component_display_name Mod Property Collection Visualizer
 */
UCLASS()
class MODIOUICORE_API UModioModPropCollectionVisualizer : public UModioUIComponentBase
{
	GENERATED_BODY()

protected:
	virtual TArray<TScriptInterface<IModioUIDataSourceWidget>> GetModPropertyVisualizerWidgets_Implementation();

	/**
	 * Queried by the base implementation to get sub-widgets to use to visualize mod properties
	 * @return Array of widgets implementing UModioUIDataSourceWidget that will visualize individual properties
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,
			  Category = "mod.io|UI|Components|Mod Property Collection Visualizer", meta = (BlueprintProtected))
	TArray<TScriptInterface<IModioUIDataSourceWidget>> GetModPropertyVisualizerWidgets();

	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativeSetDataSource(UObject* InDataSource) override;
	//~ End IModioUIDataSourceWidget Interface
};
