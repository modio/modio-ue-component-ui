/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/EventHandlers/IModioUITokenPackReceiver.h"
#include "UI/EventHandlers/IModioUIDialogDisplayEventReceiver.h"
#include "UI/EventHandlers/IModioUIWalletBalanceUpdatedEventReceiver.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUITokenPackListInterface.h"

#include "ModioTokenPackBrowser.generated.h"

/**
 * 
 */
UCLASS()
class MODIOUICORE_API UModioTokenPackBrowser : public UModioUIComponentBase,
												public IModioUITokenPackReceiver,
												public IModioUIDialogDisplayEventReceiver,
												public IModioUIWalletBalanceUpdatedEventReceiver
{
	GENERATED_BODY()

protected:

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

	//~ Begin IModioUITokenPackReceiver Interface
	virtual void NativeOnListAllTokenPacksRequestCompleted(FModioErrorCode ErrorCode, TOptional<FModioTokenPackList> List) override;
	//~ End IModioUItokenPackReceiver Interface

	//~ Begin IModioUIDialogDisplayEventReceiver Interface
	virtual void NativeOnDialogDisplayEvent(EModioUIDialogType DialogRequested, UObject* InDataSource) override;
	//~ End IModioUIDialogDisplayEventReceiver Interface

	//~ Begin IModioUIWalletBalanceUpdatedEventReceiver Interface
	virtual void NativeOnWalletBalanceUpdated(uint64 NewBalance) override;
	//~ End IModioUIWalletBalanceUpdatedEventReceiver Interface

	// Grid of TokenPackTiles.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Token Pack Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUITokenPackListInterface> GetTokenTileViewWidget() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Token Pack Browser|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetTitleTextWidget() const;
	
};
