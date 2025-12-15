/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#if WITH_EDITOR

	#include "Factories/ModioModCollectionInfoUIFactory.h"
	#include "Algo/Transform.h"
	#include "AssetRegistry/AssetRegistryModule.h"
	#include "Core/ModioModCollectionInfoUI.h"
	#include "Core/ModioUIHelpers.h"
	#include "Libraries/ModioErrorConditionLibrary.h"
	#include "Libraries/ModioSDKLibrary.h"
	#include "ModioSubsystem.h"
	#include "Widgets/DeclarativeSyntaxSupport.h"
	#include "Widgets/Input/SButton.h"
	#include "Widgets/SWindow.h"
	#include "Widgets/Text/STextBlock.h"
	#include "Widgets/Views/SListView.h"
	#include "Widgets/Views/STableRow.h"
	#include "Framework/Notifications/NotificationManager.h"
	#include "Widgets/Notifications/SNotificationList.h"

	#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModCollectionInfoUIFactory)

UModioModCollectionInfoUIFactory::UModioModCollectionInfoUIFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UModioModCollectionInfoUI::StaticClass();
}

bool UModioModCollectionInfoUIFactory::CanCreateNew() const
{
	return true;
}

UObject* UModioModCollectionInfoUIFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
															EObjectFlags Flags,
												  UObject* Context, FFeedbackContext* Warn)
{
	UModioModCollectionInfoUI* NewModCollectionInfo =
		NewObject<UModioModCollectionInfoUI>(InParent, InClass, InName, Flags);
	checkf(SelectedItem, TEXT("SelectedItem should never be nullptr"));
	NewModCollectionInfo->Underlying = *SelectedItem;
	FAssetRegistryModule::AssetCreated(NewModCollectionInfo);
	UPackage* InPackage = Cast<UPackage>(InParent);

	FString InPackagePath = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetName()) / InName.ToString();
	ModioUIHelpers::SavePackage(InPackage, NewModCollectionInfo, RF_Standalone | RF_Public, *InPackagePath);
	return NewModCollectionInfo;
}

FText UModioModCollectionInfoUIFactory::GetDisplayName() const
{
	return FText::FromString("Preview ModioModCollectionInfoUI");
}

bool UModioModCollectionInfoUIFactory::DoesSupportClass(UClass* Class)
{
	return Class->IsChildOf(UModioModCollectionInfoUI::StaticClass());
}

bool UModioModCollectionInfoUIFactory::ConfigureProperties()
{
	bOperationInProgress = false;
	FNotificationInfo Info {FText::FromString("Fetching mod collections...")};
	Info.bUseThrobber = true;
	TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info);
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	if (Subsystem)
	{
		bOperationInProgress = true;
		Subsystem->InitializeAsync(
			UModioSDKLibrary::GetProjectInitializeOptions(),
			FOnErrorOnlyDelegateFast::CreateLambda([this, Subsystem](FModioErrorCode ec) {
				if (!ec)
				{
					bRequiresSDKShutdown = true;
					Subsystem->ListModCollectionsAsync(
						FModioFilterParams(),
						FOnListModCollectionsDelegateFast::CreateUObject(
							this, &UModioModCollectionInfoUIFactory::OnModCollectionListRetrieved));
				}
				else if (UModioErrorConditionLibrary::ErrorCodeMatches(ec, EModioErrorCondition::SDKAlreadyInitialized))
				{
					Subsystem->ListModCollectionsAsync(
						FModioFilterParams(),
						FOnListModCollectionsDelegateFast::CreateUObject(
							this, &UModioModCollectionInfoUIFactory::OnModCollectionListRetrieved));
				}
				bOperationInProgress = false;
			}));

		while (bOperationInProgress)
		{
			if (!UModioSDKLibrary::GetProjectInitializeOptions().bUseBackgroundThread)
			{
				Subsystem->RunPendingHandlers();
			}
			else
			{
				FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
	#if UE_VERSION_OLDER_THAN(5, 3, 0)
				FTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());
	#else
				FTSTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());
	#endif
				FSlateApplication::Get().PumpMessages();
				FSlateApplication::Get().Tick();
				FPlatformProcess::Sleep(0);
			}
		}
	}
	else
	{
		return false;
	}

	Notification->ExpireAndFadeout();
	Notification.Reset();


	// clang-format off
	SAssignNew(ImportWindow, SWindow)
	.Title(FText::FromString("Import Preview Mod Collection Info"))
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
				SAssignNew(AvailableModCollectionInfoList, SListView<TSharedPtr<FModioModCollectionInfo>>)
					.ListItemsSource(&ModCollectionInfos)
					.Visibility_UObject(this, &UModioModCollectionInfoUIFactory::IsListVisible)
					.OnSelectionChanged_UObject(this, &UModioModCollectionInfoUIFactory::OnSelectionChanged)
					.OnGenerateRow_UObject(this, &UModioModCollectionInfoUIFactory::OnGenerateRowForList)
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
					.OnClicked_UObject(this, &UModioModCollectionInfoUIFactory::OnConfirmClicked)
					.IsEnabled_UObject(this, &UModioModCollectionInfoUIFactory::IsModCollectionInfoSelected)
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

	return bConfirmClicked;
}

void UModioModCollectionInfoUIFactory::OnModCollectionListRetrieved(FModioErrorCode ec,
														  TOptional<FModioModCollectionInfoList> ModList)
{
	ModCollectionInfos.Empty();
	if (!ec)
	{
		Algo::Transform(ModList->GetRawList(), ModCollectionInfos, [](const FModioModCollectionInfo& Info) {
			return MakeShareable(new FModioModCollectionInfo(Info));
		});
		AvailableModCollectionInfoList->RequestListRefresh();
	}
	GEngine->GetEngineSubsystem<UModioSubsystem>()->ShutdownAsync(
		FOnErrorOnlyDelegateFast::CreateLambda([this](FModioErrorCode) { bOperationInProgress = false; }));
}

FReply UModioModCollectionInfoUIFactory::OnConfirmClicked()
{
	bConfirmClicked = true;
	ImportWindow->RequestDestroyWindow();
	return FReply::Handled();
}

EVisibility UModioModCollectionInfoUIFactory::IsListVisible() const
{
	return ModCollectionInfos.Num() ? EVisibility::Visible : EVisibility::Hidden;
}

bool UModioModCollectionInfoUIFactory::IsModCollectionInfoSelected() const
{
	return SelectedItem != nullptr;
}

void UModioModCollectionInfoUIFactory::OnSelectionChanged(TSharedPtr<FModioModCollectionInfo> Item, ESelectInfo::Type)
{
	SelectedItem = Item;
}

TSharedRef<ITableRow> UModioModCollectionInfoUIFactory::OnGenerateRowForList(
	TSharedPtr<FModioModCollectionInfo> Item,
																   const TSharedRef<class STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FModioModCollectionInfo>>, OwnerTable)
		.Content()[SNew(STextBlock).Text(Item ? FText::FromString(Item->ProfileName) : FText())];
}

UClass* FModioModCollectionInfoUIAssetActions::GetSupportedClass() const
{
	return UModioModCollectionInfoUI::StaticClass();
}

uint32 FModioModCollectionInfoUIAssetActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

#endif
