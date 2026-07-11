/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Interfaces/IModioUIModCollectionListViewInterface.h"

#include "Algo/Transform.h"
#include "Core/ModioModCollectionInfoUI.h"
#include "Types/ModioModCollection.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUIModCollectionListViewInterface)

// Add default functionality here for any IIModioUIModCollectionListViewInterface functions that are not pure virtual.
void IModioUIModCollectionListViewInterface::SetModCollectionFromModCollectionInfoList_Implementation(
	const FModioModCollectionInfoList& InList, bool bAddToExisting)
{
	TArray<UObject*> WrappedCollectionList;
	Algo::Transform(InList.GetRawList(), WrappedCollectionList, [this](const FModioModCollectionInfo& In) {
		UModioModCollectionInfoUI* WrappedCollection = NewObject<UModioModCollectionInfoUI>();
		WrappedCollection->Underlying = In;
		return WrappedCollection;
	});
	NativeSetListItems(WrappedCollectionList, bAddToExisting);
}

void IModioUIModCollectionListViewInterface::SetModCollectionFromModCollectionInfoArray_Implementation(
	const TArray<FModioModCollectionInfo>& InArray, bool bAddToExisting)
{
	TArray<UObject*> WrappedCollectionList;
	Algo::Transform(InArray, WrappedCollectionList, [this](const FModioModCollectionInfo& In) {
		UModioModCollectionInfoUI* WrappedCollection = NewObject<UModioModCollectionInfoUI>();
		WrappedCollection->Underlying = In;
		return WrappedCollection;
	});
	NativeSetListItems(WrappedCollectionList, bAddToExisting);
}

void IModioUIModCollectionListViewInterface::SetModCollectionSelectionByID_Implementation(
	FModioModCollectionID ModCollectionID)
{
	NativeSetModCollectionSelectionByID(ModCollectionID);
}