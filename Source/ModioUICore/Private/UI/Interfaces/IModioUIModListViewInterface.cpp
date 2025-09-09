/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Interfaces/IModioUIModListViewInterface.h"

#include "Algo/Transform.h"
#include "Core/ModioModCollectionEntryUI.h"
#include "Core/ModioModInfoUI.h"
#include "Engine/Engine.h"
#include "ModioSubsystem.h"
#include "Core/ModioModDependencyUI.h"
#include "Types/ModioModInfoList.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUIModListViewInterface)

void IModioUIModListViewInterface::SetModSelectionByID_Implementation(FModioModID ModID)
{
	NativeSetModSelectionByID(ModID);
}

void IModioUIModListViewInterface::SetModsFromModInfoList_Implementation(const FModioModInfoList& InList,
																		 bool bAddToExisting)
{
	TArray<UObject*> WrappedModList;
	Algo::Transform(InList.GetRawList(), WrappedModList, [](const FModioModInfo& In) {
		UModioModInfoUI* WrappedMod = NewObject<UModioModInfoUI>();
		WrappedMod->Underlying = In;
		return WrappedMod;
	});
	NativeSetListItems(WrappedModList, bAddToExisting);
}

void IModioUIModListViewInterface::SetModsFromModInfoArray_Implementation(const TArray<FModioModInfo>& InArray,
																		  bool bAddToExisting)
{
	TArray<UObject*> WrappedModList;
	Algo::Transform(InArray, WrappedModList, [](const FModioModInfo& In) {
		UModioModInfoUI* WrappedMod = NewObject<UModioModInfoUI>();
		WrappedMod->Underlying = In;
		return WrappedMod;
	});
	NativeSetListItems(WrappedModList, bAddToExisting);
}

void IModioUIModListViewInterface::SetModsFromModCollectionEntryArray_Implementation(
	const TArray<FModioModCollectionEntry>& InArray, bool bAddToExisting)
{
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (!Subsystem)
	{
		return;
	}

	const TMap<FModioModID, FModioModCollectionEntry> UserSubscriptions = Subsystem->QueryUserSubscriptions();

	TArray<UObject*> CachedCollection;
	Algo::Transform(InArray, CachedCollection, [&UserSubscriptions](const FModioModCollectionEntry& In) {
		UModioModCollectionEntryUI* WrappedModCollectionEntry = NewObject<UModioModCollectionEntryUI>();
		WrappedModCollectionEntry->Underlying = In;
		WrappedModCollectionEntry->bCachedSubscriptionStatus = UserSubscriptions.Contains(In.GetID());
		return WrappedModCollectionEntry;
	});
	NativeSetListItems(CachedCollection, bAddToExisting);
}