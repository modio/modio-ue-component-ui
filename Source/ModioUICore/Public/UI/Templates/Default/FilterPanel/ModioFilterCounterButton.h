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
#include "UI/Components/Buttons/ModioDefaultIconButton.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUIModTagSelector.h"

#include "ModioFilterCounterButton.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioFilterCounterButton : public UModioDefaultIconButton, public IModioUIModTagSelector
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Filter Button|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIDataSourceWidget> GetFilterCounterWidget();

	virtual void NativeSetDataSource(UObject* InDataSource) override;
};
