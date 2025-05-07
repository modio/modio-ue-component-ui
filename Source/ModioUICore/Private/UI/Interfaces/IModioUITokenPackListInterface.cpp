/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Interfaces/IModioUITokenPackListInterface.h"

#include "Core/ModioTokenPackUI.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUITokenPackListInterface)

void IModioUITokenPackListInterface::SetTokenPacksFromTokenPackList_Implementation(const FModioTokenPackList& InList,
                                                                                    bool bAddToExisting)
{
	TArray<UObject*> WrappedTokenPackList;
	Algo::Transform(InList.GetRawList(), WrappedTokenPackList, [this](const FModioTokenPack& In) {
		UModioTokenPackUI* WrappedMod = NewObject<UModioTokenPackUI>();
		WrappedMod->Underlying = In;
		return WrappedMod;
	});
	NativeSetListItems(WrappedTokenPackList, bAddToExisting);
}

void IModioUITokenPackListInterface::SetTokenPacksFromTokenPackArray_Implementation(
	const TArray<FModioTokenPack>& InArray, bool bAddToExisting)
{
	TArray<UObject*> WrappedModList;
	Algo::Transform(InArray, WrappedModList, [this](const FModioTokenPack& In) {
		UModioTokenPackUI* WrappedMod = NewObject<UModioTokenPackUI>();
		WrappedMod->Underlying = In;
		return WrappedMod;
	});
	NativeSetListItems(WrappedModList, bAddToExisting);
}

void IModioUITokenPackListInterface::SetTokenPackSelectionByID_Implementation(FModioTokenPackID TokenPackID)
{
	NativeSetTokenPackSelectionByID(TokenPackID);
}