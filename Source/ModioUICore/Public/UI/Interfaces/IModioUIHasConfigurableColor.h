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

#include "Core/LinearColorRef.h"
#include "CoreMinimal.h"
#include "Styling/SlateColor.h"
#include "UObject/Interface.h"

#include "IModioUIHasConfigurableColor.generated.h"

UINTERFACE(BlueprintType)
class UModioUIHasConfigurableColor : public UInterface
{
	GENERATED_BODY()
};

/**
 * @brief Interface indicating that the implementing widget can have its associated colors configured at runtime.
 */
class MODIOUICORE_API IModioUIHasConfigurableColor
{
	GENERATED_BODY()

	virtual void ConfigureWidgetColorDirectly_Implementation(FName ColorName, FSlateColor ColorValue) {};
	virtual void ConfigureWidgetColor_Implementation(FName ColorName, FLinearColorRef ColorValue) {};

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "mod.io|UI|Configurable Color",
			  meta = (ColorName = "Default"))
	void ConfigureWidgetColorDirectly(FName ColorName, FSlateColor ColorValue);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "mod.io|UI|Configurable Color",
			  meta = (ColorName = "Default"))
	void ConfigureWidgetColor(FName ColorName, FLinearColorRef ColorValue);
};
