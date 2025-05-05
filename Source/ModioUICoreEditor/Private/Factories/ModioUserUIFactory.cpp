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

	#include "Factories/ModioUserUIFactory.h"
	#include "AssetRegistry/AssetRegistryModule.h"
	#include "Core/ModioUserUI.h"
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
	#include "Framework/Notifications/NotificationManager.h"
	#include "Widgets/Notifications/SNotificationList.h"
	#include "IStructureDetailsView.h"
	#include "PropertyEditorModule.h"
	#include "Framework/Application/SlateApplication.h"

	#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioUserUIFactory)

UModioUserUIFactory::UModioUserUIFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UModioUserUI::StaticClass();
}

bool UModioUserUIFactory::CanCreateNew() const
{
	return true;
}

UObject* UModioUserUIFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
											   UObject* Context, FFeedbackContext* Warn)
{
	UModioUserUI* NewUserObject = NewObject<UModioUserUI>(InParent, InClass, InName, Flags);
	checkf(CurrentUser, TEXT("SelectedItem should never be nullptr"));
	NewUserObject->Underlying = *CurrentUser;
	FAssetRegistryModule::AssetCreated(NewUserObject);
	UPackage* InPackage = Cast<UPackage>(InParent);

	FString InPackagePath = FPackageName::GetLongPackagePath(InParent->GetOutermost()->GetName()) / InName.ToString();
	ModioUIHelpers::SavePackage(InPackage, NewUserObject, RF_Standalone | RF_Public, *InPackagePath);
	return NewUserObject;
}

FText UModioUserUIFactory::GetDisplayName() const
{
	return FText::FromString("Preview ModioUserUI");
}

bool UModioUserUIFactory::DoesSupportClass(UClass* Class)
{
	return Class->IsChildOf(UModioUserUI::StaticClass());
}

bool UModioUserUIFactory::ConfigureProperties()
{
	bool bOperationInProgress = false;
	FNotificationInfo Info {FText::FromString("Fetching current authenticated user...")};
	Info.bUseThrobber = true;

	TSharedPtr<SNotificationItem> Notification = FSlateNotificationManager::Get().AddNotification(Info);
	UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
	FModioInitializeOptions InitOptions =
		UModioSDKLibrary::GetProjectInitializeOptionsForSessionId("ModioUnrealEditor");

	if (Subsystem)
	{
		bOperationInProgress = true;
		Subsystem->InitializeAsync(
			InitOptions,
			FOnErrorOnlyDelegateFast::CreateLambda([this, Subsystem, &bOperationInProgress](FModioErrorCode ec) {
				if (!ec)
				{
					bRequiresSDKShutdown = true;
					CurrentUser = Subsystem->QueryUserProfile();
				}
				else if (UModioErrorConditionLibrary::ErrorCodeMatches(ec, EModioErrorCondition::SDKAlreadyInitialized))
				{
					CurrentUser = Subsystem->QueryUserProfile();
				}
				bOperationInProgress = false;
			}));

		while (bOperationInProgress)
		{
			if (!InitOptions.bUseBackgroundThread)
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
	if (!CurrentUser.IsSet())
	{
		CurrentUser = FModioUser {};
		CurrentUser->UserId = FModioUserID(0);
		CurrentUser->Username = "Default User";
	}
	// Use 4 hours ago as the default 'last seen' date;
	CurrentUser->DateOnline = FDateTime::Now() - FTimespan::FromHours(4);

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	UserStructWrapper = MakeShared<FStructOnScope>(FModioUser::StaticStruct(), (uint8*) CurrentUser.GetPtrOrNull());
	FDetailsViewArgs DetailArgs;
	DetailArgs.bUpdatesFromSelection = false;
	DetailArgs.bLockable = false;
	DetailArgs.NameAreaSettings = FDetailsViewArgs::ComponentsAndActorsUseNameArea;
	DetailArgs.bCustomNameAreaLocation = false;
	DetailArgs.bCustomFilterAreaLocation = true;
	DetailArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Show;
	DetailArgs.bForceHiddenPropertyVisibility = true;

	// clang-format off
	SAssignNew(ImportWindow, SWindow)
	.Title(FText::FromString("Import Preview User Object"))
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
				PropertyModule.CreateStructureDetailView(
						{}, {}, UserStructWrapper,
						FText::FromString("User Info"))->GetWidget().ToSharedRef()
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
					.OnClicked_UObject(this, &UModioUserUIFactory::OnConfirmClicked)
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

FReply UModioUserUIFactory::OnConfirmClicked()
{
	bConfirmClicked = true;
	ImportWindow->RequestDestroyWindow();
	return FReply::Handled();
}

UClass* FModioUserUIAssetActions::GetSupportedClass() const
{
	return UModioUserUI::StaticClass();
}

uint32 FModioUserUIAssetActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}

#endif
