/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "Core/ModioEnumEntryUI.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioEnumEntryUI)

void UModioEnumEntryUI::GetEnumEntryDetails_Implementation(uint8& EnumValue,
														   FText& EnumValueDisplayText)
{
	EnumValue = UnderlyingEnumValue;
	EnumValueDisplayText = UnderlyingEnumValueDisplayText;
}
