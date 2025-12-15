/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Templates/Default/Dialogs/ModioModCollectionDetailsDialog.h"

#include "UI/Interfaces/IModioModCollectionInfoUIDetails.h"
#include "UI/Templates/Default/Commands/ModioUIDisplayFilteredModCollectionsCommand.h"
#include "UI/Templates/Default/Commands/ModioUIToggleModCollectionFollowedCommand.h"
#include "UI/Templates/Default/Commands/ModioUIViewModCollectionContributorsCommand.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModCollectionDetailsDialog)

TScriptInterface<IModioUIImageDisplayWidget> UModioModCollectionDetailsDialog::
GetModCollectionLogoWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIDataSourceWidget> UModioModCollectionDetailsDialog::
GetModCollectionPropertiesWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIDataSourceWidget> UModioModCollectionDetailsDialog::
GetModCollectionStatusWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIDataSourceWidget> UModioModCollectionDetailsDialog::
GetTagDisplayWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIClickableWidget> UModioModCollectionDetailsDialog::
GetExpandDescriptionButtonWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIClickableWidget> UModioModCollectionDetailsDialog::
GetFollowButtonWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIClickableWidget> UModioModCollectionDetailsDialog::
GetRateUpButtonWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIClickableWidget> UModioModCollectionDetailsDialog::
GetRateDownButtonWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUICommandMenu> UModioModCollectionDetailsDialog::
GetMoreOptionsMenuWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIClickableWidget> UModioModCollectionDetailsDialog::
GetCloseDialogButtonWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIHasTextWidget> UModioModCollectionDetailsDialog::
GetModCollectionNameTextWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIDataSourceWidget> UModioModCollectionDetailsDialog::
GetModCollectionCuratorDisplayWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIHasTextWidget> UModioModCollectionDetailsDialog::
GetModCollectionSummaryShortTextWidget_Implementation() const
{
	return nullptr;
}

void UModioModCollectionDetailsDialog::PopulateExtraOptionsCommands_Implementation(TArray<UObject*>& Commands,
	UObject* Context)
{
	if (Context)
	{
		if (Context->GetClass()->ImplementsInterface(UModioUIDataSourceWidget::StaticClass()))
		{
			if (UObject* CommandContextDataSource = Execute_GetDataSource(Context))
			{
				if (CommandContextDataSource &&
				    CommandContextDataSource->GetClass()->ImplementsInterface(
					    UModioModCollectionInfoUIDetails::StaticClass()))
				{
					FModioModCollectionInfo ModCollectionInfo =
						IModioModCollectionInfoUIDetails::Execute_GetFullModCollectionInfo(CommandContextDataSource);

					/* Awaiting SDK changes
					*  Currently we don't have a way to accuratly get followed state in syncronous manner
					// follow/unfollow
					UModioUIToggleModCollectionFollowedCommand* ToggleFollowCommand =
						NewObject<UModioUIToggleModCollectionFollowedCommand>();
					// The command needs to register for follow state event changes
					// As currently we do not have a syncronous method for checking if a collection is followed or not
					ToggleFollowCommand->RegisterFollowStateChangedReciever();
					IModioUICommandObject::Execute_SetDataSource(ToggleFollowCommand, CommandContextDataSource);
					Commands.Add(ToggleFollowCommand);
					*/

					// View Contributors
					UModioUIViewModCollectionContributorsCommand* ViewContributorsCommand =
						NewObject<UModioUIViewModCollectionContributorsCommand>();
					IModioUICommandObject::Execute_SetDataSource(ViewContributorsCommand, CommandContextDataSource);
					Commands.Add(ViewContributorsCommand);

					// More from this curator
					UModioUIDisplayFilteredModCollectionsCommand* DisplayMoreCommand =
						NewObject<UModioUIDisplayFilteredModCollectionsCommand>();
					IModioUICommandObject::Execute_SetDataSource(DisplayMoreCommand, CommandContextDataSource);
					FModioFilterParams CommandParams;
					CommandParams.MatchingAuthor(ModCollectionInfo.ProfileSubmittedBy.UserId);
					if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
					{
						if (UISubsystem->IsUGCFeatureEnabled(EModioUIFeatureFlags::Monetization))
						{
							CommandParams.RevenueType(EModioRevenueFilterType::FreeAndPaid);
						}
					}
					DisplayMoreCommand->SetFilterParams(CommandParams);
					DisplayMoreCommand->SetDisplayStringKey("MoreFromCurator");
					Commands.Add(DisplayMoreCommand);

					// Report Collection
					// Awaiting feature implementation in NativeSDK
				}
			}
		}
	}
}

TScriptInterface<IModioUIModListViewInterface> UModioModCollectionDetailsDialog::
GetCollectionModsListViewWidget_Implementation() const
{
	return nullptr;
}

void UModioModCollectionDetailsDialog::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);

	if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModCollectionInfoUIDetails::StaticClass()))
	{
		BoundModCollectionInfo = IModioModCollectionInfoUIDetails::Execute_GetFullModCollectionInfo(DataSource);
		bHasFullDescription = (BoundModCollectionInfo.ProfileDescriptionPlaintext.Len() > 0);
	}
	else
	{
		BoundModCollectionInfo = {};
		bHasFullDescription = false;
	}
	if (UWidget* GalleryWidget =
		ModioUI::GetInterfaceWidgetAsDataSourceWidget(GetModCollectionLogoWidget()))
	{
		Execute_SetDataSource(GalleryWidget, InDataSource);
	}

	if (ModioUI::GetInterfaceWidgetAsDataSourceWidget(GetMoreOptionsMenuWidget()))
	{
		if (UWidget* MenuWidget = ModioUI::GetInterfaceWidgetChecked(GetMoreOptionsMenuWidget()))
		{
			Execute_SetDataSource(MenuWidget, InDataSource);
			IModioUICommandMenu::Execute_RebuildCommandList(MenuWidget);
		}
	}
}

void UModioModCollectionDetailsDialog::NativePreConstruct()
{
	Super::NativePreConstruct();
	IModioUIMediaDownloadCompletedReceiver::Register<UModioModCollectionDetailsDialog>(
		EModioUIMediaDownloadEventType::ModCollectionLogo);
	IModioUIModCollectionInfoReceiver::Register<UModioModCollectionDetailsDialog>(
		EModioUIModCollectionInfoEventType::GetModCollectionMods);

	if (UWidget* MenuWidget = ModioUI::GetInterfaceWidgetChecked(GetMoreOptionsMenuWidget()))
	{
		FModioCommandListBuilder ModCollectionDetailsCommandBuilderDelegate;
		ModCollectionDetailsCommandBuilderDelegate.BindUFunction(
			this, GET_FUNCTION_NAME_CHECKED_TwoParams(UModioModCollectionDetailsDialog, PopulateExtraOptionsCommands,
			                                          TArray<UObject*>&, UObject*));
		IModioUICommandMenu::Execute_AddCommandListBuilder(MenuWidget, ModCollectionDetailsCommandBuilderDelegate);
	}
}

void UModioModCollectionDetailsDialog::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
}

void UModioModCollectionDetailsDialog::NativeOnModCollectionLogoDownloadCompleted(FModioModCollectionID ModCollectionID,
	FModioErrorCode ErrorCode,
	TOptional<FModioImageWrapper> Image,
	EModioLogoSize LogoSize)
{
	IModioUIMediaDownloadCompletedReceiver::NativeOnModCollectionLogoDownloadCompleted(ModCollectionID, ErrorCode,
		Image, LogoSize);
	if (!ErrorCode && Image.IsSet() &&
	    GetModCollectionLogoWidget())
	{
		IModioUIImageDisplayWidget::Execute_BeginLoadImageFromFile(
			ModioUI::GetInterfaceWidgetChecked(GetModCollectionLogoWidget()), Image.GetValue());
	}
}

void UModioModCollectionDetailsDialog::NativeOnModCollectionModsRequestCompleted(FModioModCollectionID CollectionID,
	FModioErrorCode ErrorCode,
	TOptional<FModioModInfoList> Mods)
{
	IModioUIModCollectionInfoReceiver::NativeOnModCollectionModsRequestCompleted(CollectionID, ErrorCode, Mods);
	if (!ErrorCode && Mods.IsSet() &&
	    GetCollectionModsListViewWidget())
	{
		IModioUIModListViewInterface::Execute_SetModsFromModInfoList(
			ModioUI::GetInterfaceWidgetChecked(GetCollectionModsListViewWidget()), Mods.GetValue(), false);
	}
}