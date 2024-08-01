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
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"

#include "ModioSearchBar.generated.h"

/**
 * Incomplete and not in use.
 * Consider replacing the search bar elements used in the mod browser with this.
 */
UCLASS()
class MODIOUICORE_API UModioSearchBar : public UModioUIComponentBase,
										public IModioUIStringInputWidget,
										public IModioUIClickableWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Search Bar|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIStringInputWidget> GetSearchBoxWidget() const;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Search Bar|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetButtonWidget() const;
};
