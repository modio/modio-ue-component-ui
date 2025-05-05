/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "Core/ModioStackedBool.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioStackedBool)

void UModioStackedBoolLibrary::PushBoolValue(UPARAM(Ref) FModioStackedBool& Target, bool bNewValue) 
{
	Target.Push(bNewValue);
}
bool UModioStackedBoolLibrary::PopBoolValue(UPARAM(Ref) FModioStackedBool& Target, bool& bDidPopSuccessfully)
{
	bDidPopSuccessfully = false;
	if (Target.Count() > 0)
	{
		bDidPopSuccessfully = true;
		return Target.Pop();
	}
	return false;
}

bool UModioStackedBoolLibrary::PeekBoolValue(const FModioStackedBool& Target, bool& bDidPeekSuccessfully)
{
	bDidPeekSuccessfully = false;
	if (Target.Count() > 0)
	{
		bDidPeekSuccessfully = true;
		return Target.Peek();
	}
	return false;
}
