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

#include "Components/Border.h"
#include "CoreMinimal.h"
#include "UI/Interfaces/IModioUIHasConfigurableColor.h"

#include "ModioDefaultBorder.generated.h"

/**
 * @brief Default implementation of a border that can be used as a component in mod.io UI
 */
UCLASS()
class MODIOUICORE_API UModioDefaultBorder : public UBorder, public IModioUIHasConfigurableColor
{
	GENERATED_BODY()
protected:
	virtual void ConfigureWidgetColorDirectly_Implementation(FName ColorName, FSlateColor ColorValue) override;

	virtual void ConfigureWidgetColor_Implementation(FName ColorName, FLinearColorRef ColorValue) override;
};
