/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/ModTile/ModioDefaultModTile.h"

#include "Engine/GameInstance.h"
#include "Input/CommonUIActionRouterBase.h"
#include "ModioSubsystem.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Interfaces/IModioUICommandMenu.h"
#include "UI/Templates/Default/Commands/ModioUIDisplayFilteredModsCommand.h"
#include "UI/Templates/Default/Commands/ModioUIManageModSubscriptionCommand.h"
#include "UI/Templates/Default/Commands/ModioUIPrioritizeModCommand.h"
#include "UI/Templates/Default/Commands/ModioUIReportModCommand.h"
#include "UI/Templates/Default/Commands/ModioUIToggleModEnabledCommand.h"
#include "UI/Templates/Default/Commands/ModioUIViewModCommand.h"

void UModioDefaultModTile::NotifyClicked()
{
	if (bEmitClickEvents)
	{
		OnClicked.Broadcast(this);
	}
}

void UModioDefaultModTile::NotifyHoverStateChanged(bool bNewHoveredState)
{
	if (bEmitHoverEvents)
	{
		OnHoverStateChanged.Broadcast(this, bNewHoveredState);
	}
}

bool UModioDefaultModTile::GetSelectedStateEx()
{
	if (!IUserObjectListEntry::GetListItem())
	{
		return false;
	}
	if (IUserListEntry::GetOwningListView() != nullptr)
	{
		return IUserListEntry::IsListItemSelected();
	}
	else
	{
		return bSelected;
	}
}

void UModioDefaultModTile::NativeOnItemSelectionChanged(bool bIsSelected)
{
	if (bSelectable)
	{
		// Calls up into BP
		IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

		bSelected = bIsSelected;
		// Emits our external selection event
		OnSelected.Broadcast(this, bIsSelected);

		// During the selection change, some Common UI-specific actions may need to be updated
		// This doesn't lead to performance issues, as it's only called when the selection state changes
		if (const UGameInstance* GameInstance = GetGameInstance())
		{
			for (const ULocalPlayer* LocalPlayer : GameInstance->GetLocalPlayers())
			{
				if (const UCommonUIActionRouterBase* ActionRouter =
						ULocalPlayer::GetSubsystem<UCommonUIActionRouterBase>(LocalPlayer))
				{
					ActionRouter->OnBoundActionsUpdated().Broadcast();
				}
			}
		}
	}
}

void UModioDefaultModTile::PopulateExtraOptionsCommands_Implementation(TArray<UObject*>& Commands, UObject* Context)
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
					// View Mod
					// Always shown
					UModioUIViewModCommand* ViewCommand = NewObject<UModioUIViewModCommand>();
					IModioUICommandObject::Execute_SetDataSource(ViewCommand, CommandContextDataSource);
					Commands.Add(ViewCommand);

					FModioModInfo ModInfo = IModioModInfoUIDetails::Execute_GetFullModInfo(CommandContextDataSource);
					if (UModioSubsystem* ModioSubsystem = GEngine->GetEngineSubsystem<UModioSubsystem>())
					{
						// Enable/Disable Mod
						// Only included if enable/disable feature is configured and user is subscribed
						if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
						{
							if (UISubsystem->IsUGCFeatureEnabled(EModioUIFeatureFlags::ModEnableDisable))
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
						// Prioritize mod
						// Only available if mod is subscribed and pending install or update
						TMap<FModioModID, FModioModCollectionEntry> UserSubscriptions =
							ModioSubsystem->QueryUserSubscriptions();
						if (UserSubscriptions.Contains(ModInfo.ModId))
						{
							if (UserSubscriptions[ModInfo.ModId].GetModState() == EModioModState::InstallationPending ||
								UserSubscriptions[ModInfo.ModId].GetModState() == EModioModState::UpdatePending)
							{
								UModioUIPrioritizeModCommand* PrioritizeCommand =
									NewObject<UModioUIPrioritizeModCommand>();
								IModioUICommandObject::Execute_SetDataSource(PrioritizeCommand,
																			 CommandContextDataSource);
								Commands.Add(PrioritizeCommand);
							}
						}
					}
					// Subscribe/Unsubscribe
					// Available for purchased mods or if monetization is disabled
					{
						UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
						if (Subsystem &&
							(ModInfo.Price == 0 || Subsystem->QueryUserPurchasedMods().Contains(ModInfo.ModId)))
						{
							UModioUIManageModSubscriptionCommand* ManageSubCommand =
								NewObject<UModioUIManageModSubscriptionCommand>();
							IModioUICommandObject::Execute_SetDataSource(ManageSubCommand, CommandContextDataSource);
							Commands.Add(ManageSubCommand);
						}
					}
					// More from this creator
					// Always shown
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
					// Always shown
					UModioUIReportModCommand* ReportCommand = NewObject<UModioUIReportModCommand>();
					IModioUICommandObject::Execute_SetDataSource(ReportCommand, CommandContextDataSource);
					Commands.Add(ReportCommand);
				}
			}
		}
	}
}

void UModioDefaultModTile::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (UWidget* MenuWidget = ModioUI::GetInterfaceWidgetChecked(GetMoreOptionsMenuWidget()))
	{
		FModioCommandListBuilder ModDetailsCommandBuilderDelegate;
		ModDetailsCommandBuilderDelegate.BindUFunction(
			this, GET_FUNCTION_NAME_CHECKED_TwoParams(UModioDefaultModTile, PopulateExtraOptionsCommands,
													  TArray<UObject*>&, UObject*));
		IModioUICommandMenu::Execute_AddCommandListBuilder(MenuWidget, ModDetailsCommandBuilderDelegate);
	}

#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		NativeOnListItemObjectSet(EditorDataSource);
	}
#endif
}

void UModioDefaultModTile::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	DataSource = InDataSource;

	if (ModioUI::GetInterfaceWidgetAsDataSourceWidget(GetMoreOptionsMenuWidget()))
	{
		if (UWidget* MenuWidget = ModioUI::GetInterfaceWidgetChecked(GetMoreOptionsMenuWidget()))
		{
			IModioUIDataSourceWidget::Execute_SetDataSource(MenuWidget, InDataSource);
			IModioUICommandMenu::Execute_RebuildCommandList(MenuWidget);
		}
	}
}

bool UModioDefaultModTile::IsListItemSelectable() const
{
	return bSelectable;
}

TScriptInterface<IModioUICommandMenu> UModioDefaultModTile::GetMoreOptionsMenuWidget_Implementation() const
{
	return nullptr;
}

void UModioDefaultModTile::DisableHoverEvents_Implementation()
{
	bEmitHoverEvents = false;
}

void UModioDefaultModTile::EnableHoverEvents_Implementation()
{
	bEmitHoverEvents = true;
}

void UModioDefaultModTile::RemoveHoverStateChangedHandler_Implementation(
	const FModioHoverableOnHoverStateChanged& Handler)
{
	OnHoverStateChanged.Remove(Handler);
}

void UModioDefaultModTile::AddHoverStateChangedHandler_Implementation(const FModioHoverableOnHoverStateChanged& Handler)
{
	OnHoverStateChanged.AddUnique(Handler);
}

void UModioDefaultModTile::DisableClick_Implementation()
{
	bEmitClickEvents = false;
}

void UModioDefaultModTile::EnableClick_Implementation()
{
	bEmitClickEvents = true;
}

void UModioDefaultModTile::RemoveClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnClicked.Remove(Handler);
}

void UModioDefaultModTile::AddClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnClicked.AddUnique(Handler);
}

void UModioDefaultModTile::GetToggleable_Implementation(bool& bIsToggleable)
{
	bIsToggleable = bToggleable;
}

void UModioDefaultModTile::SetToggleable_Implementation(bool bNewToggleableState)
{
	bToggleable = bNewToggleableState;
}

void UModioDefaultModTile::GetSelectedState_Implementation(bool& bCurrentSelectedState)
{
	if (bSelectable)
	{
		bCurrentSelectedState = GetSelectedStateEx();
	}
	else
	{
		bCurrentSelectedState = false;
	}
}

void UModioDefaultModTile::ToggleSelectedState_Implementation()
{
	if (bSelectable)
	{
		bool bCurrentSelectionState;
		IModioUISelectableWidget::Execute_GetSelectedState(this, bCurrentSelectionState);
		IModioUISelectableWidget::Execute_SetSelectedState(this, !bCurrentSelectionState);
	}
}

void UModioDefaultModTile::SetSelectedState_Implementation(bool bNewSelectedState)
{
	if (bSelectable)
	{
		bool bCurrentSelectedState = GetSelectedStateEx();
		if (bCurrentSelectedState != bNewSelectedState)
		{
			bSelected = bNewSelectedState;
			NativeOnItemSelectionChanged(bNewSelectedState);
		}
	}
}

void UModioDefaultModTile::GetSelectable_Implementation(bool& bIsSelectable)
{
	bIsSelectable = bSelectable;
}

void UModioDefaultModTile::SetSelectable_Implementation(bool bNewSelectableState)
{
	bSelectable = bNewSelectableState;
}

void UModioDefaultModTile::RemoveSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	OnSelected.Remove(Handler);
}

void UModioDefaultModTile::AddSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	OnSelected.AddUnique(Handler);
}
