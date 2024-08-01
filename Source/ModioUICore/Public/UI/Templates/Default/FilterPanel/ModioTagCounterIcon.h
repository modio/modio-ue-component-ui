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
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUIModTagSelector.h"

#include "ModioTagCounterIcon.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioTagCounterIcon : public UModioUIComponentBase,
											 public IModioUIHasTextWidget,
											 public IModioUIModTagSelector
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Counter Icon|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetCounterLabelWidget();

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Counter Icon", meta = (BlueprintProtected))
	int32 SynchronizeCounter();

	virtual void NativeSetDataSource(UObject* InDataSource) override;
};
