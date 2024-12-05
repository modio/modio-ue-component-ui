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

#include "Components/SlateWrapperTypes.h"
#include "Delegates/DelegateCombinations.h"
#include "UObject/Interface.h"

#include "IModioUIDialog.generated.h"

/**
 * @brief Enum for specific dialog types, can be extended if there are additional dialog types added
 */
UENUM(BlueprintType)
enum class EModioUIDialogType : uint8
{
	ModDetails,
	ReportMod,
	EnableDisableManagement,
	PurchaseMod,
	StoreUi
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FModioDialogShownEvent, bool, bOpen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioDialogShownEventMulticast, bool, bOpen);

DECLARE_DYNAMIC_DELEGATE(FModioDialogEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FModioDialogEventMulticast);

DECLARE_DYNAMIC_DELEGATE_OneParam(FModioDialogButtonClickedEvent, int32, ButtonIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioDialogButtonClickedEventMulticast, int32, ButtonIndex);

/**
 * @brief Interface indicating that the implementing widget behaves as a modal dialog that can emit Confirmed/Canceled/Shown events
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Dialog", Meta = (DocGroup = "UI", Comment = "@brief Interface indicating that the implementing widget behaves as a modal dialog that can emit Confirmed/Canceled/Shown events"))
class MODIOUICORE_API UModioUIDialog : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIDialog : public IInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Registers a callback to receive notifications when the dialog is confirmed
	 * @param Handler Delegate to invoke on a confirmation event
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Dialog")
	void AddDialogConfirmedHandler(const FModioDialogEvent& Handler);

	/**
	 * @brief Registers a callback to receive notifications when the dialog is canceled
	 * @param Handler Delegate to invoke on a cancellation event
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Dialog")
	void AddDialogCanceledHandler(const FModioDialogEvent& Handler);

	/**
	 * @brief Registers a callback to receive notifications when the dialog is shown
	 * @param Handler Delegate to invoke after the dialog is shown
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Dialog")
	void AddDialogShownHandler(const FModioDialogShownEvent& Handler);

	/**
	 * @brief Unregisters a callback so it no longer receives notifications when the dialog is confirmed
	 * @param Handler Delegate that will no longer receive confirmation events
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Dialog")
	void RemoveDialogConfirmedHandler(const FModioDialogEvent& Handler);

	/**
	 * @brief Unregisters a callback so it no longer receives notifications when the dialog is cancelled
	 * @param Handler Delegate that will no longer receive cancellation events
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Dialog")
	void RemoveDialogCanceledHandler(const FModioDialogEvent& Handler);

	/**
	 * @brief Unregisters a callback so it no longer receives notifications when the dialog is Shown
	 * @param Handler Delegate that will no longer receive events when the dialog is shown
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Dialog")
	void RemoveDialogShownHandler(const FModioDialogShownEvent& Handler);

	/**
	 * @brief Registers a callback to receive notifications when a dialog button is clicked
	 * @param Handler Delegate to invoke when a button is clicked
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Dialog")
	void AddDialogButtonClickedHandler(const FModioDialogButtonClickedEvent& Handler);

	/**
	 * @brief Unregisters a callback so it no longer receives notifications when a dialog button is clicked
	 * @param Handler Delegate that will no longer receive invocations
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Dialog")
	void RemoveDialogButtonClickedHandler(const FModioDialogButtonClickedEvent& Handler);

	/**
	 * @brief Requests that the modal dialog be displayed
	 * @param bFocusCancelButton Should the cancel button on the dialog automatically receive focus
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Dialog")
	void ShowModal(bool bFocusCancelButton);

	/**
	 * @brief Closes the modal dialog, removing it from display
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Dialog")
	void CloseModal();
};
