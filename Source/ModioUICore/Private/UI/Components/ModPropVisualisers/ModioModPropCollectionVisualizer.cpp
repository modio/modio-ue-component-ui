/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/ModPropVisualizers/ModioModPropCollectionVisualizer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModPropCollectionVisualizer)

TArray<TScriptInterface<IModioUIDataSourceWidget>> UModioModPropCollectionVisualizer::
	GetModPropertyVisualizerWidgets_Implementation()
{
	return {};
}

void UModioModPropCollectionVisualizer::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	for (TScriptInterface<IModioUIDataSourceWidget> VisualizerWidget : GetModPropertyVisualizerWidgets())
	{
		// TScriptInterface is being used here to try to communicate to consumers about the interface the widgets should
		// implement, but for pure blueprint classes the interface reference in the TScriptInterface will be null, so we
		// get the raw object from the TScriptInterface and validate it, and never try to reference the native interface
		// pointer directly
		if (VisualizerWidget.GetObject() &&
			VisualizerWidget.GetObject()->GetClass()->ImplementsInterface(UModioUIDataSourceWidget::StaticClass()))
		{
			IModioUIDataSourceWidget::Execute_SetDataSource(VisualizerWidget.GetObject(), DataSource);
		}
	}
}
