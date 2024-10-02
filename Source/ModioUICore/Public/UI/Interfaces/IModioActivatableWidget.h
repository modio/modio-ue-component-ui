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
#include "UObject/Interface.h"
#include "IModioActivatableWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FModioOnActivationChanged, UObject*, ActivationContext, bool, bIsActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioOnActivationChangedMulticast, UObject*, ActivationContext, bool, bIsActivated);

/**
 * @brief Interface indicating the implementing widget can be activated or deactivated
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Activatable",
	Meta = (DocGroup = "UI", Comment =
		"@brief Interface indicating the implementing widget can be activated or deactivated"))
class UModioActivatableWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioActivatableWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeActivate() {}
	void Activate_Implementation()
	{
		NativeActivate();
	}

	virtual void NativeDeactivate() {}
	void Deactivate_Implementation()
	{
		NativeDeactivate();
	}

	virtual void NativeAddActivationChangedHandler(const FModioOnActivationChanged& Handler) {}
	void AddActivationChangedHandler_Implementation(const FModioOnActivationChanged& Handler)
	{
		NativeAddActivationChangedHandler(Handler);
	}

	virtual void NativeRemoveActivationChangedHandler(const FModioOnActivationChanged& Handler) {}
	void RemoveActivationChangedHandler_Implementation(const FModioOnActivationChanged& Handler)
	{
		NativeRemoveActivationChangedHandler(Handler);
	}

public:
	/**
	 * @brief Activates the implementing widget
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Activatable")
	void Activate();

	/**
	 * @brief Deactivates the implementing widget
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Activatable")
	void Deactivate();

	/**
	 * @brief Registers a delegate to receive callbacks when the activation state changes
	 * @param Handler Delegate to invoke on activation state change
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Focusable")
	void AddActivationChangedHandler(const FModioOnActivationChanged& Handler);

	/**
	 * @brief Unregisters a delegate from receiving callbacks when the activation state changes
	 * @param Handler Delegate to remove from activation state change
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Focusable")
	void RemoveActivationChangedHandler(const FModioOnActivationChanged& Handler);
};