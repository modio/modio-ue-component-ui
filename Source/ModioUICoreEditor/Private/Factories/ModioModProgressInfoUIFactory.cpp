/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#if WITH_EDITOR

	#include "Factories/ModioModProgressInfoUIFactory.h"
	#include "AssetRegistry/AssetRegistryModule.h"
	#include "Core/ModioModProgressInfoUI.h"
	#include "Core/ModioUIHelpers.h"
	#include "Widgets/DeclarativeSyntaxSupport.h"
	#include "Widgets/SWindow.h"
	#include "ModioSubsystem.h"
	#include "Libraries/ModioErrorConditionLibrary.h"
	#include "Libraries/ModioSDKLibrary.h"
	#include "Algo/Transform.h"
	#include "Widgets/Input/SButton.h"
	#include "Widgets/Text/STextBlock.h"
	#include "Widgets/Views/STableRow.h"
	#include "Widgets/Views/SListView.h"

	#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModProgressInfoUIFactory)

UModioModProgressInfoUIFactory::UModioModProgressInfoUIFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UModioModProgressInfoUI::StaticClass();
}

bool UModioModProgressInfoUIFactory::CanCreateNew() const
{
	return true;
}

UObject* UModioModProgressInfoUIFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
												  UObject* Context, FFeedbackContext* Warn)
{
	UModioModProgressInfoUI* NewProgressInfo = NewObject<UModioModProgressInfoUI>(InParent, InClass, InName, Flags);
	FAssetRegistryModule::AssetCreated(NewProgressInfo);
	UPackage* InPackage = Cast<UPackage>(InParent);

	FString InPackagePath = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetName()) / InName.ToString();
	ModioUIHelpers::SavePackage(InPackage, NewProgressInfo, RF_Standalone | RF_Public, *InPackagePath);
	return NewProgressInfo;
}

FText UModioModProgressInfoUIFactory::GetDisplayName() const
{
	return FText::FromString("Preview ModioModProgressInfoUI");
}

bool UModioModProgressInfoUIFactory::DoesSupportClass(UClass* Class)
{
	return Class->IsChildOf(UModioModProgressInfoUI::StaticClass());
}

bool UModioModProgressInfoUIFactory::ConfigureProperties()
{
	/*
	// clang-format off
	SAssignNew(ImportWindow, SWindow)
	.Title(FText::FromString("Import Preview Mod Info"))
	.ClientSize(FVector2D(500, 600))
	.HasCloseButton(true)
	.SupportsMinimize(false)
	.SupportsMaximize(false)
	[
		SNew(SBorder)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SAssignNew(AvailableModInfoList, SListView<TSharedPtr<FModioModInfo>>)
					.ListItemsSource(&ModInfos)
					.Visibility_UObject(this, &UModioModProgressInfoUIFactory::IsListVisible)
					.OnSelectionChanged_UObject(this, &UModioModProgressInfoUIFactory::OnSelectionChanged)
					.OnGenerateRow_UObject(this, &UModioModProgressInfoUIFactory::OnGenerateRowForList)
			] 
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Bottom)
					.OnClicked_UObject(this, &UModioModProgressInfoUIFactory::OnConfirmClicked)
					.IsEnabled_UObject(this, &UModioModProgressInfoUIFactory::IsModInfoSelected)
					[
						SNew(STextBlock)
						.Text(FText::FromString("OK"))
					]
				]
			]
		]
	];
	// clang-format on
	GEditor->EditorAddModalWindow(ImportWindow.ToSharedRef());
	ImportWindow.Reset();

	return bConfirmClicked;*/
	return true;
}


UClass* FModioModProgressInfoUIAssetActions::GetSupportedClass() const
{
	return UModioModProgressInfoUI::StaticClass();
}

uint32 FModioModProgressInfoUIAssetActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

#endif
