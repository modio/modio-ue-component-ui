/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Templates/Default/Dialogs/ModioModDetailsDialog.h"

#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Templates/Default/Commands/ModioUIDisplayFilteredModsCommand.h"
#include "UI/Templates/Default/Commands/ModioUIReportModCommand.h"
#include "UI/Templates/Default/Commands/ModioUIToggleModEnabledCommand.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioModDetailsDialog)

bool UModioModDetailsDialog::IsUserSubscribed()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		TMap<FModioModID, FModioModCollectionEntry> Subscriptions = Subsystem->QueryUserSubscriptions();
		if (Subscriptions.Find(BoundModInfo.ModId))
		{
			return true;
		}
	}
	return false;
}

bool UModioModDetailsDialog::IsModPurchased()
{
	if (UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
	{
		TMap<FModioModID, FModioModInfo> Purchases = Subsystem->QueryUserPurchasedMods();
		if (Purchases.Find(BoundModInfo.ModId))
		{
			return true;
		}
	}
	return false;
}

void UModioModDetailsDialog::PopulateExtraOptionsCommands_Implementation(TArray<UObject*>& Commands, UObject* Context)
{
	if (Context)
	{
		if (Context->GetClass()->ImplementsInterface(UModioUIDataSourceWidget::StaticClass()))
		{
			if (UObject* CommandContextDataSource = IModioUIDataSourceWidget::Execute_GetDataSource(Context))
			{
				if (CommandContextDataSource &&
					CommandContextDataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
				{
					FModioModInfo ModInfo = IModioModInfoUIDetails::Execute_GetFullModInfo(CommandContextDataSource);

					// Enable/Disable Mod
					// Only included if enable/disable feature is enabled in project settings
					if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
					{
						if (UISubsystem->IsUGCFeatureEnabled(EModioUIFeatureFlags::ModEnableDisable))
						{
							if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
							{
								if (ModioSubsystem->QueryUserInstallations(true).Contains(ModInfo.ModId))
								{
									UModioUIToggleModEnabledCommand* ToggleCommand =
										NewObject<UModioUIToggleModEnabledCommand>();
									IModioUICommandObject::Execute_SetDataSource(ToggleCommand,
																				 CommandContextDataSource);
									Commands.Add(ToggleCommand);
								}
							}
						}
					}
					// More from this creator
					UModioUIDisplayFilteredModsCommand* DisplayMoreCommand =
						NewObject<UModioUIDisplayFilteredModsCommand>();
					IModioUICommandObject::Execute_SetDataSource(DisplayMoreCommand, CommandContextDataSource);
					FModioFilterParams CommandParams;
					CommandParams.MatchingAuthor(ModInfo.ProfileSubmittedBy.UserId);
					if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
					{
						if (UISubsystem->IsUGCFeatureEnabled(EModioUIFeatureFlags::Monetization))
						{
							CommandParams.RevenueType(EModioRevenueFilterType::FreeAndPaid);
						}
					}
					DisplayMoreCommand->SetFilterParams(CommandParams);
					DisplayMoreCommand->SetDisplayStringKey("MoreFromCreator");
					Commands.Add(DisplayMoreCommand);
					// Report Mod
					UModioUIReportModCommand* ReportCommand = NewObject<UModioUIReportModCommand>();
					IModioUICommandObject::Execute_SetDataSource(ReportCommand, CommandContextDataSource);
					Commands.Add(ReportCommand);
				}
			}
		}
	}
}

void UModioModDetailsDialog::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);

	if (DataSource && DataSource->GetClass()->ImplementsInterface(UModioModInfoUIDetails::StaticClass()))
	{
		BoundModInfo = IModioModInfoUIDetails::Execute_GetFullModInfo(DataSource);
		bHasFullDescription = (BoundModInfo.ProfileDescriptionPlaintext.Len() > 0);
	}
	else
	{
		BoundModInfo = {};
		bHasFullDescription = false;
	}
	if (UWidget* GalleryWidget = ModioUI::GetInterfaceWidgetAsDataSourceWidget(GetModImageGalleryWidget()))
	{
		IModioUIDataSourceWidget::Execute_SetDataSource(GalleryWidget, InDataSource);
	}

	if (ModioUI::GetInterfaceWidgetAsDataSourceWidget(GetMoreOptionsMenuWidget()))
	{
		if (UWidget* MenuWidget = ModioUI::GetInterfaceWidgetChecked(GetMoreOptionsMenuWidget()))
		{
			IModioUIDataSourceWidget::Execute_SetDataSource(MenuWidget, InDataSource);
			IModioUICommandMenu::Execute_RebuildCommandList(MenuWidget);
		}
	}
}

void UModioModDetailsDialog::NativePreConstruct()
{
	Super::NativePreConstruct();

	IModioUISubscriptionsChangedReceiver::Register<UModioModDetailsDialog>();
	IModioUIMediaDownloadCompletedReceiver::Register<UModioModDetailsDialog>(EModioUIMediaDownloadEventType::ModLogo);
	IModioUIWalletBalanceUpdatedEventReceiver::Register<UModioModDetailsDialog>();

	if (UWidget* MenuWidget = ModioUI::GetInterfaceWidgetChecked(GetMoreOptionsMenuWidget()))
	{
		FModioCommandListBuilder ModDetailsCommandBuilderDelegate;
		ModDetailsCommandBuilderDelegate.BindUFunction(
			this, GET_FUNCTION_NAME_CHECKED_TwoParams(UModioModDetailsDialog, PopulateExtraOptionsCommands,
													  TArray<UObject*>&, UObject*));
		IModioUICommandMenu::Execute_AddCommandListBuilder(MenuWidget, ModDetailsCommandBuilderDelegate);
	}

#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		IModioUIDataSourceWidget::Execute_SetDataSource(this, EditorDataSource);
	}
#endif
}

void UModioModDetailsDialog::NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState)
{
	// Route the event to BP
	IModioUISubscriptionsChangedReceiver::NativeOnSubscriptionsChanged(ModID, bNewSubscriptionState);
}

TScriptInterface<IModioUIHasTextWidget> UModioModDetailsDialog::GetModNameTextWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIDataSourceWidget> UModioModDetailsDialog::GetModAuthorDisplayWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIHasTextWidget> UModioModDetailsDialog::GetModSummaryShortTextWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIDataSourceWidget> UModioModDetailsDialog::GetModPropertiesWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModDetailsDialog::GetExpandDescriptionButtonWidget_Implementation()
	const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModDetailsDialog::GetSubscribeButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModDetailsDialog::GetPurchaseButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModDetailsDialog::GetRateUpButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModDetailsDialog::GetRateDownButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUICommandMenu> UModioModDetailsDialog::GetMoreOptionsMenuWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIClickableWidget> UModioModDetailsDialog::GetModDependenciesButtonWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIDataSourceWidget> UModioModDetailsDialog::GetTagDisplayWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIMediaDownloadCompletedReceiver> UModioModDetailsDialog::
	GetModImageGalleryWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModDetailsDialog::GetCloseDialogButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIClickableWidget> UModioModDetailsDialog::GetWalletButtonWidget_Implementation() const
{
	return nullptr;
}
TScriptInterface<IModioUIDataSourceWidget> UModioModDetailsDialog::GetModStatusWidget_Implementation() const
{
	return nullptr;
}
