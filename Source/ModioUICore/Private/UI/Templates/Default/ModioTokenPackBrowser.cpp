/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Templates/Default/ModioTokenPackBrowser.h"

#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioTokenPackBrowser)

void UModioTokenPackBrowser::NativePreConstruct()
{
	Super::NativePreConstruct();

	IModioUITokenPackReceiver::Register<UModioTokenPackBrowser>(EModioUITokenPackEventType::ListAllTokenPacks);
	IModioUIDialogDisplayEventReceiver::Register<UModioTokenPackBrowser>();
	IModioUIWalletBalanceUpdatedEventReceiver::Register<UModioTokenPackBrowser>();
}

void UModioTokenPackBrowser::NativeOnListAllTokenPacksRequestCompleted(FModioErrorCode ErrorCode, TOptional<FModioTokenPackList> List)
{
	IModioUITokenPackReceiver::NativeOnListAllTokenPacksRequestCompleted(ErrorCode, List);

	if (ErrorCode || !List.IsSet())
	{
		return;
	}
	if (ModioUI::GetInterfaceWidgetChecked(GetTokenTileViewWidget()))
	{
		IModioUITokenPackListInterface::Execute_SetTokenPacksFromTokenPackList(GetTokenTileViewWidget().GetObject(),
																	 List.GetValue(), false);
	}
}

void UModioTokenPackBrowser::NativeOnDialogDisplayEvent(EModioUIDialogType DialogRequested, UObject* InDataSource)
{
	IModioUIDialogDisplayEventReceiver::NativeOnDialogDisplayEvent(DialogRequested, InDataSource);
}

void UModioTokenPackBrowser::NativeOnWalletBalanceUpdated(uint64 NewBalance)
{
	IModioUIWalletBalanceUpdatedEventReceiver::NativeOnWalletBalanceUpdated(NewBalance);
}

TScriptInterface<IModioUITokenPackListInterface> UModioTokenPackBrowser::GetTokenTileViewWidget_Implementation() const
{
	return nullptr;
}

TScriptInterface<IModioUIHasTextWidget> UModioTokenPackBrowser::GetTitleTextWidget_Implementation() const
{
	return nullptr;
}
