/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Interfaces/IModioUICommandMenu.h"
#include "Core/ModioAssetPaths.h"

#include "Brushes/SlateNoResource.h"
#include "Engine/DataTable.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUICommandMenu)

const FModioUICommandAssetEntry& UModioUICommandLibrary::GetCommandAssetEntry(const FString& CommandID, bool& bResolved)
{
	static const FModioUICommandAssetEntry DefaultEntry {};
	bResolved = false;
	FSoftObjectPath DefaultCommandTablePath = ModioUI::AssetPaths::GetUiCommandsTablePath();
	UDataTable* DefaultCommandTable = Cast<UDataTable>(DefaultCommandTablePath.TryLoad());
	if (DefaultCommandTable)
	{
		const UScriptStruct* CommandTableEntryStruct = DefaultCommandTable->GetRowStruct();
		if (CommandTableEntryStruct)
		{
			void* RowPtr = DefaultCommandTable->FindRowUnchecked(FName(CommandID));

			if (RowPtr != nullptr)
			{
				bResolved = true;
				return *reinterpret_cast<FModioUICommandAssetEntry*>(RowPtr);
			}
		}
	}
	return DefaultEntry;
}

const FText& UModioUICommandLibrary::GetCommandText(const FString& CommandID, const FName& TextName, bool& bResolved)
{
	bResolved = false;

	bool bWasCommandEntryResolved = false;
	const FModioUICommandAssetEntry& Entry = GetCommandAssetEntry(CommandID, bWasCommandEntryResolved);
	if (bWasCommandEntryResolved)
	{
		const FText* ResolvedText = Entry.CommandTexts.Find(TextName);
		if (ResolvedText != nullptr)
		{
			bResolved = true;
			return *ResolvedText;
		}
	}

	return FText::GetEmpty();
}

const FSlateBrush& UModioUICommandLibrary::GetCommandBrush(const FString& CommandID, const FName& BrushName,
														   bool& bResolved)
{
	static const FSlateNoResource EmptyBrush {};
	bResolved = false;

	bool bWasCommandEntryResolved = false;
	const FModioUICommandAssetEntry& Entry = GetCommandAssetEntry(CommandID, bWasCommandEntryResolved);
	if (bWasCommandEntryResolved)
	{
		const FSlateBrush* ResolvedBrush = Entry.CommandIconBrushes.Find(BrushName);
		if (ResolvedBrush != nullptr)
		{
			bResolved = true;
			return *ResolvedBrush;
		}
	}

	return EmptyBrush;
}
