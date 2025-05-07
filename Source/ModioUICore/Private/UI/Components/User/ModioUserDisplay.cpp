/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "UI/Components/User/ModioUserDisplay.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioUserDisplay)

void UModioUserDisplay::NativePreConstruct()
{
	
#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		IModioUIDataSourceWidget::Execute_SetDataSource(this, EditorDataSource);
	}
#endif
}
