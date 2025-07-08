/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/TagDisplay/ModioModTagDisplay.h"

#include "Core/ModioTagOptionsUI.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Interfaces/IModioUIObjectListWidget.h"
#include "UI/ModioUICommonFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModTagDisplay)

TScriptInterface<IModioUIObjectListWidget> UModioModTagDisplay::GetTagContainerWidget_Implementation() const
{
	return nullptr;
}

void UModioModTagDisplay::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	if (InDataSource && InDataSource->Implements<UModioModInfoUIDetails>())
	{
		FModioModInfo UnderlyingMod = IModioModInfoUIDetails::Execute_GetFullModInfo(InDataSource);
		TArray<UObject*> BoundTags;

		for (UModioModTagUI* BoundTag : UModioUICommonFunctionLibrary::CreateBindableModTagArray(UnderlyingMod.Tags))
		{
			BoundTags.Add(BoundTag);
		}

		if (GetTagContainerWidget().GetObject())
		{
			IModioUIObjectListWidget::Execute_SetObjects(GetTagContainerWidget().GetObject(), BoundTags);
		}
	}
}

void UModioModTagDisplay::NativePreConstruct()
{
	Super::NativePreConstruct();
#if WITH_EDITORONLY_DATA
	if (EditorDataSource && IsDesignTime())
	{
		IModioUIDataSourceWidget::Execute_SetDataSource(this, EditorDataSource);
	}
#endif
}
