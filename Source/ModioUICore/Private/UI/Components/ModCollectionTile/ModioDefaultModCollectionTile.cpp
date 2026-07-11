/*
 * Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/ModCollectionTile/ModioDefaultModCollectionTile.h"

#include "Engine/GameInstance.h"
#include "ModioSubsystem.h"
#include "Input/CommonUIActionRouterBase.h"
#include "UI/Interfaces/IModioModCollectionInfoUIDetails.h"
#include "UI/Interfaces/IModioUICommandMenu.h"
#include "UI/Templates/Default/Commands/ModioUIViewModCollectionCommand.h"
#include "UI/Templates/Default/Commands/ModioUIDisplayFilteredModCollectionsCommand.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioDefaultModCollectionTile)

void UModioDefaultModCollectionTile::NotifyClicked()
{
	if (bEmitClickEvents)
	{
		OnClicked.Broadcast(this);
	}
}

bool UModioDefaultModCollectionTile::GetSelectedStateEx()
{
	if (!GetListItem())
	{
		return false;
	}
	if (GetOwningListView() != nullptr)
	{
		return IsListItemSelected();
	}
	return bSelected;
}

void UModioDefaultModCollectionTile::NotifyHoverStateChanged(bool bNewHoveredState)
{
	if (bEmitHoverEvents)
	{
		OnHoverStateChanged.Broadcast(this, bNewHoveredState);
	}
}

void UModioDefaultModCollectionTile::NativeOnItemSelectionChanged(bool bIsSelected)
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

void UModioDefaultModCollectionTile::AddSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	OnSelected.AddUnique(Handler);
}

void UModioDefaultModCollectionTile::RemoveSelectedStateChangedHandler_Implementation(
	const FModioSelectableOnSelectionChanged& Handler)
{
	OnSelected.Remove(Handler);
}

void UModioDefaultModCollectionTile::SetSelectable_Implementation(bool bNewSelectableState)
{
	bSelectable = bNewSelectableState;
}

void UModioDefaultModCollectionTile::GetSelectable_Implementation(bool& bIsSelectable)
{
	bIsSelectable = bSelectable;
}

void UModioDefaultModCollectionTile::SetSelectedState_Implementation(bool bNewSelectedState)
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

void UModioDefaultModCollectionTile::ToggleSelectedState_Implementation()
{
	if (bSelectable)
	{
		bool bCurrentSelectionState;
		Execute_GetSelectedState(this, bCurrentSelectionState);
		Execute_SetSelectedState(this, !bCurrentSelectionState);
	}
}

void UModioDefaultModCollectionTile::GetSelectedState_Implementation(bool& bCurrentSelectedState)
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

void UModioDefaultModCollectionTile::SetToggleable_Implementation(bool bNewToggleableState)
{
	bToggleable = bNewToggleableState;
}

void UModioDefaultModCollectionTile::GetToggleable_Implementation(bool& bIsToggleable)
{
	bIsToggleable = bToggleable;
}

void UModioDefaultModCollectionTile::AddClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnClicked.AddUnique(Handler);
}

void UModioDefaultModCollectionTile::RemoveClickedHandler_Implementation(const FModioClickableOnClicked& Handler)
{
	OnClicked.Remove(Handler);
}

void UModioDefaultModCollectionTile::EnableClick_Implementation()
{
	bEmitClickEvents = true;
}

void UModioDefaultModCollectionTile::DisableClick_Implementation()
{
	bEmitClickEvents = false;
}

void UModioDefaultModCollectionTile::AddHoverStateChangedHandler_Implementation(
	const FModioHoverableOnHoverStateChanged& Handler)
{
	OnHoverStateChanged.AddUnique(Handler);
}

void UModioDefaultModCollectionTile::RemoveHoverStateChangedHandler_Implementation(
	const FModioHoverableOnHoverStateChanged& Handler)
{
	OnHoverStateChanged.Remove(Handler);
}

void UModioDefaultModCollectionTile::EnableHoverEvents_Implementation()
{
	bEmitHoverEvents = true;
}

void UModioDefaultModCollectionTile::DisableHoverEvents_Implementation()
{
	bEmitHoverEvents = false;
}

void UModioDefaultModCollectionTile::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (UWidget* MenuWidget = ModioUI::GetInterfaceWidgetChecked(GetMoreOptionsMenuWidget()))
	{
		FModioCommandListBuilder ModCollectionDetailsCommandBuilderDelegate;
		ModCollectionDetailsCommandBuilderDelegate.BindUFunction(
			this, GET_FUNCTION_NAME_CHECKED_TwoParams(UModioDefaultModCollectionTile, PopulateExtraOptionsCommands,
			                                          TArray<UObject*>&, UObject*));
		IModioUICommandMenu::Execute_AddCommandListBuilder(MenuWidget, ModCollectionDetailsCommandBuilderDelegate);
	}

	#if WITH_EDITORONLY_DATA
	if (IsDesignTime())
	{
		NativeOnListItemObjectSet(EditorDataSource);
	}
	#endif
}

void UModioDefaultModCollectionTile::NativeSetDataSource(UObject* InDataSource)
{
	Super::NativeSetDataSource(InDataSource);
	DataSource = InDataSource;

	if (ModioUI::GetInterfaceWidgetAsDataSourceWidget(GetMoreOptionsMenuWidget()))
	{
		if (UWidget* MenuWidget = ModioUI::GetInterfaceWidgetChecked(GetMoreOptionsMenuWidget()))
		{
			Execute_SetDataSource(MenuWidget, InDataSource);
			IModioUICommandMenu::Execute_RebuildCommandList(MenuWidget);
		}
	}
}

bool UModioDefaultModCollectionTile::IsListItemSelectable() const
{
	return bSelectable;
}

TScriptInterface<IModioUICommandMenu> UModioDefaultModCollectionTile::GetMoreOptionsMenuWidget_Implementation() const
{
	return nullptr;
}

void UModioDefaultModCollectionTile::PopulateExtraOptionsCommands_Implementation(TArray<UObject*>& Commands,
	UObject* Context)
{
	if (!Context || !Context->GetClass()->ImplementsInterface(UModioUIDataSourceWidget::StaticClass()))
	{
		return;
	}

	UObject* CommandContextDataSource = Execute_GetDataSource(Context);
	if (!CommandContextDataSource || !CommandContextDataSource->GetClass()->ImplementsInterface(
		    UModioModCollectionInfoUIDetails::StaticClass()))
	{
		return;
	}

	FModioModCollectionInfo ModCollectionInfo =
		IModioModCollectionInfoUIDetails::Execute_GetFullModCollectionInfo(
			CommandContextDataSource);

	// View Collection
	UModioUIViewModCollectionCommand* ViewCommand = NewObject<UModioUIViewModCollectionCommand>();
	IModioUICommandObject::Execute_SetDataSource(ViewCommand, CommandContextDataSource);
	Commands.Add(ViewCommand);

	// Follow / Unfollow Collection
	// Awaiting backend changes

	// more from curator
	UModioUIDisplayFilteredModCollectionsCommand* MoreFromCuratorCommand =
		NewObject<UModioUIDisplayFilteredModCollectionsCommand>();
	IModioUICommandObject::Execute_SetDataSource(MoreFromCuratorCommand, CommandContextDataSource);
	MoreFromCuratorCommand->SetFilterParams(
		FModioFilterParams().MatchingAuthor(ModCollectionInfo.ProfileSubmittedBy.UserId));
	MoreFromCuratorCommand->SetDisplayStringKey("MoreFromCurator");
	Commands.Add(MoreFromCuratorCommand);

	// Report Collection
	// Awaiting implementation in native SDK
}