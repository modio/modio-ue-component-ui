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

#include "UObject/Interface.h"

#include "IModioUIHasLoadStateWidget.generated.h"

UENUM(BlueprintType, Category = "mod.io|UI|Loading State Widget")
enum class EModioUILoadState : uint8
{
	NotLoaded,
	Loading,
	Loaded,
	Errored
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FModioOnLoadStateWidgetStateChanged, UObject*, Object, EModioUILoadState, NewState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioOnLoadStateWidgetStateChangedMulticast, UObject*, Object,
											 EModioUILoadState, NewState);

/**
 * @brief Interface indicating that the implementing widget has a loading state that is either internally or externally
 * managed, and will emit events on loading state transitions
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Loading State Widget", Meta = (DocGroup = "UI", Comment = "@brief Interface indicating that the implementing widget has a loading state that is either internally or externally managed, and will emit events on loading state transitions"))
class MODIOUICORE_API UModioUIHasLoadStateWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIHasLoadStateWidget : public IInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeSetLoadState(EModioUILoadState NewState) {}
	void SetLoadState_Implementation(EModioUILoadState NewState)
	{
		NativeSetLoadState(NewState);
	}
	virtual EModioUILoadState NativeGetLoadState() const
	{
		return EModioUILoadState::NotLoaded;
	}
	virtual EModioUILoadState GetLoadState_Implementation() const
	{
		return NativeGetLoadState();
	}
	virtual void NativeAddLoadStateChangedHandler(const FModioOnLoadStateWidgetStateChanged& Handler) {}
	virtual void AddLoadStateChangedHandler_Implementation(const FModioOnLoadStateWidgetStateChanged& Handler)
	{
		NativeAddLoadStateChangedHandler(Handler);
	}
	virtual void NativeRemoveLoadStateChangedHandler(const FModioOnLoadStateWidgetStateChanged& Handler) {};
	virtual void RemoveLoadStateChangedHandler_Implementation(const FModioOnLoadStateWidgetStateChanged& Handler)
	{
		NativeRemoveLoadStateChangedHandler(Handler);
	}

public:
	/**
	 * @brief Requests the implementing widget change its load state to the specified state
	 * @param NewState the new state to change to
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Loading State Widget")
	void SetLoadState(EModioUILoadState NewState);

	/**
	 * @brief Retrieves the implementing widget's current load state
	 * @return The current load state for the widget
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Loading State Widget")
	EModioUILoadState GetLoadState() const;

	/**
	 * @brief Registers a delegate to receive callbacks when the load state of the implementing widget changes, either from internal transitions or an external request
	 * @param Handler Delegate to invoke on a state change event
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Loading State Widget")
	void AddLoadStateChangedHandler(const FModioOnLoadStateWidgetStateChanged& Handler);

	/**
	 * @brief Unregisters a delegate so it will no longer receive callbacks when the load state of the implementing widget changes
	 * @param Handler The delegate to unregister
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Loading State Widget")
	void RemoveLoadStateChangedHandler(const FModioOnLoadStateWidgetStateChanged& Handler);
};