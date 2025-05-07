/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Templates/Default/ModioModBrowser.h"

#include "ModioSubsystem.h"
#include "ModioUISubsystem.h"
#include "UI/Templates/Default/Dialogs/ModioModDetailsDialog.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModBrowser)

void UModioModBrowser::NativePreConstruct()
{
	Super::NativePreConstruct();

	IModioUIModInfoReceiver::Register<UModioModBrowser>(EModioUIModInfoEventType::ListAllMods);
	IModioUIDialogDisplayEventReceiver::Register<UModioModBrowser>();
	IModioUIWalletBalanceUpdatedEventReceiver::Register<UModioModBrowser>();
	IModioUISubscriptionsChangedReceiver::Register<UModioModBrowser>();
}

void UModioModBrowser::NativeOnListAllModsRequestCompleted(FString RequestIdentifier, FModioErrorCode ErrorCode,
														   TOptional<FModioModInfoList> List)
{
	IModioUIModInfoReceiver::NativeOnListAllModsRequestCompleted(RequestIdentifier, ErrorCode, List);

	if (CurrentView == EModioModBrowserState::LibraryView)
	{
		UE_LOG(ModioUICore, Verbose,
			   TEXT("Library view, used to display local mods, is not expected to be"
					" populated by the ListAllMods request. Ignoring."));
		return;
	}

	if (ErrorCode || !List.IsSet())
	{
		return;
	}
	if (ModioUI::GetInterfaceWidgetChecked(GetModTileViewWidget()))
	{
		IModioUIModListViewInterface::Execute_SetModsFromModInfoList(GetModTileViewWidget().GetObject(),
																	 List.GetValue(), false);
	}
}

void UModioModBrowser::NativeOnDialogDisplayEvent(EModioUIDialogType DialogRequested, UObject* InDataSource)
{
	IModioUIDialogDisplayEventReceiver::NativeOnDialogDisplayEvent(DialogRequested, InDataSource);
}

void UModioModBrowser::NativeOnWalletBalanceUpdated(uint64 NewBalance)
{
	IModioUIWalletBalanceUpdatedEventReceiver::NativeOnWalletBalanceUpdated(NewBalance);
}

void UModioModBrowser::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
}

void UModioModBrowser::InitializeTagData(UObject* InTagData)
{
	if (!InTagData || !(InTagData->GetClass()->ImplementsInterface(UModioUIModTagSelector::StaticClass())) ||
		!(InTagData->GetClass()->ImplementsInterface(UModioUIDataSourceWidget::StaticClass())))
	{
		checkf(false, TEXT("UObject passed to InitializeTagData should implement both IUModioUIModTagSelector and "
						   "IModioUIDataSourceWidget interfaces"));
		return;
	}
	if (!StoredTagData)
	{
		StoredTagData = InTagData;

		if (GetFilterButtonWidget().GetObject() && GetFilterButtonWidget().GetObject()->GetClass()->ImplementsInterface(
													   UModioUIDataSourceWidget::StaticClass()))
		{
			IModioUIDataSourceWidget::Execute_SetDataSource(GetFilterButtonWidget().GetObject(), StoredTagData);
		}
	}
}

void UModioModBrowser::InitializeLibraryTagData(UObject* InTagData)
{
	if (!InTagData || !(InTagData->GetClass()->ImplementsInterface(UModioUIModTagSelector::StaticClass())) ||
		!(InTagData->GetClass()->ImplementsInterface(UModioUIDataSourceWidget::StaticClass())))
	{
		checkf(false, TEXT("UObject passed to InitializeTagData should implement both IUModioUIModTagSelector and "
						   "IModioUIDataSourceWidget interfaces"));
		return;
	}
	if (!StoredLibraryTagData)
	{
		StoredLibraryTagData = InTagData;

		if (GetFilterButtonWidget().GetObject() && GetFilterButtonWidget().GetObject()->GetClass()->ImplementsInterface(
													   UModioUIDataSourceWidget::StaticClass()))
		{
			IModioUIDataSourceWidget::Execute_SetDataSource(GetFilterButtonWidget().GetObject(), StoredLibraryTagData);
		}
	}
}

TArray<FModioModInfo> UModioModBrowser::SearchLibraryWithStoredParams() const
{
	if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (!UISubsystem->IsUGCFeatureEnabled(EModioUIFeatureFlags::Monetization))
		{
			return SearchSubscriptionsAndInstallsWithStoredParams();
		}
	}
	TArray<FModioModInfo> SearchResults;
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		// Library View includes user subscriptions, system installations, and user purchases
		TMap<FModioModID, FModioModInfo> LibraryMods;
		TMap<FModioModID, FModioModCollectionEntry> SubscriptionsAndInstalls = Subsystem->QueryUserSubscriptions();
		SubscriptionsAndInstalls.Append(Subsystem->QuerySystemInstallations());
		for (auto& Entry : SubscriptionsAndInstalls)
		{
			LibraryMods.Add(Entry.Key, Entry.Value.GetModProfile());
		}
		LibraryMods.Append(Subsystem->QueryUserPurchasedMods());
		// Filter by stored search string and selected tags (if applicable)
		for (auto& Mod : LibraryMods)
		{
			if (!StoredSearchString.IsEmpty())
			{
				if (Mod.Value.ProfileName.Contains(StoredSearchString))
				{
					SearchResults.Add(Mod.Value);
				}
			}
			else
			{
				SearchResults.Add(Mod.Value);
			}
		}
		return FilterModArrayByTags(SearchResults, StoredLibraryTagData);
	}
	return SearchResults;
}

TArray<FModioModInfo> UModioModBrowser::SearchSubscriptionsAndInstallsWithStoredParams() const
{
	TArray<FModioModInfo> SearchResults;
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		TMap<FModioModID, FModioModCollectionEntry> SubscriptionsAndInstalls = Subsystem->QueryUserSubscriptions();
		SubscriptionsAndInstalls.Append(Subsystem->QuerySystemInstallations());

		for (auto& Mod : SubscriptionsAndInstalls)
		{
			if (!StoredSearchString.IsEmpty())
			{
				if (Mod.Value.GetModProfile().ProfileName.Contains(StoredSearchString))
				{
					SearchResults.Add(Mod.Value.GetModProfile());
				}
			}
			else
			{
				SearchResults.Add(Mod.Value.GetModProfile());
			}
		}
		return FilterModArrayByTags(SearchResults, StoredLibraryTagData);
	}
	return SearchResults;
}

TArray<FModioModInfo> UModioModBrowser::SearchPurchasesWithStoredParams() const
{
	TArray<FModioModInfo> SearchResults;
	if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
	{
		if (!UISubsystem->IsUGCFeatureEnabled(EModioUIFeatureFlags::Monetization))
		{
			return SearchResults;
		}
	}
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		for (auto& Mod : Subsystem->QueryUserPurchasedMods())
		{
			if (!StoredSearchString.IsEmpty())
			{
				if (Mod.Value.ProfileName.Contains(StoredSearchString))
				{
					SearchResults.Add(Mod.Value);
				}
			}
			else
			{
				SearchResults.Add(Mod.Value);
			}
		}
		return FilterModArrayByTags(SearchResults, StoredLibraryTagData);
	}
	return SearchResults;
}

TArray<FModioModInfo> UModioModBrowser::FilterModArrayByTags(const TArray<FModioModInfo>& ModArray,
																	 const TObjectPtr<UObject>& Tags) const
{
	if (Tags && Tags->GetClass()->ImplementsInterface(UModioUIModTagSelector::StaticClass()))
	{
		TArray<FString> SelectedTags = IModioUIModTagSelector::Execute_GetSelectedTags(Tags);

		// Early out if there are no selected tags to filter on
		if (SelectedTags.IsEmpty())
		{
			return ModArray;
		}

		TArray<FModioModInfo> ModListByTag;
		for (const FModioModInfo& Mod : ModArray)
		{
			// All selected tags must be present for the mod to be included
			bool bAllTagsMatch = true;
			for (FString& SelectedTag : SelectedTags)
			{
				if (!ModIncludesTag(Mod, SelectedTag))
				{
					bAllTagsMatch = false;
					break;
				}
			}
			if (bAllTagsMatch)
			{
				ModListByTag.Add(Mod);
			}
		}
		return ModListByTag;
	}
	return ModArray;
}

bool UModioModBrowser::ModIncludesTag(const FModioModInfo& Mod, const FString& Tag)
{
	for (const FModioModTag& ModTag : Mod.Tags)
	{
		if (ModTag.Tag == Tag)
		{
			return true;
		}
	}
	return false;
}

bool UModioModBrowser::DoFilterParamsIncludeUserId(const FModioFilterParams& Params)
{
	return !Params.AuthorUserIds.IsEmpty();
}

void UModioModBrowser::DecrementPresetFilterSelection()
{
	if (ModioUI::GetInterfaceWidgetChecked(GetPresetFilterSelectorWidget()))
	{
		int32 CurrentSelection =
			IModioUIObjectSelector::Execute_GetSingleSelectionIndex(GetPresetFilterSelectorWidget().GetObject());

		if (CurrentSelection > 0)
		{
			IModioUIObjectSelector::Execute_SetSingleSelectionByIndex(GetPresetFilterSelectorWidget().GetObject(),
																	  CurrentSelection - 1, true);
		}
	}
}

void UModioModBrowser::IncrementPresetFilterSelection()
{
	if (ModioUI::GetInterfaceWidgetChecked(GetPresetFilterSelectorWidget()))
	{
		int32 CurrentSelection =
			IModioUIObjectSelector::Execute_GetSingleSelectionIndex(GetPresetFilterSelectorWidget().GetObject());

		if (CurrentSelection <
			IModioUIObjectSelector::Execute_GetNumEntries(GetPresetFilterSelectorWidget().GetObject()) - 1)
		{
			IModioUIObjectSelector::Execute_SetSingleSelectionByIndex(GetPresetFilterSelectorWidget().GetObject(),
																	  CurrentSelection + 1, true);
		}
	}
}

TScriptInterface<IModioUIImageDisplayWidget> UModioModBrowser::GetViewDescriptionImageWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIHasTextWidget> UModioModBrowser::GetViewDescriptionTextWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModBrowser::GetLibraryViewButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIObjectSelector> UModioModBrowser::GetPresetFilterSelectorWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModBrowser::GetSearchButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIStringInputWidget> UModioModBrowser::GetSearchEditableTextBoxWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModBrowser::GetFilterButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModBrowser::GetWalletButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIModListViewInterface> UModioModBrowser::GetModTileViewWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIHasTextWidget> UModioModBrowser::GetSearchTextWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModBrowser::GetClearSearchButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModBrowser::GetTabLeftButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModBrowser::GetTabRightButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModBrowser::GetCloseBrowserButtonWidget_Implementation() const
{
	return nullptr;
}
