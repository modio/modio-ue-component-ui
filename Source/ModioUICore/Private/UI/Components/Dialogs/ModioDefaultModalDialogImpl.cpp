/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Dialogs/ModioDefaultModalDialogImpl.h"

#include "UI/Components/ComponentHelpers.h"

void UModioDefaultModalDialogImpl::AddDialogConfirmedHandler_Implementation(const FModioDialogEvent& Handler)
{
	MODIO_INTERFACE_UNIMPLEMENTED_FUNCTION_MSG(
		"Dialog helper class only implements DialogButtonClicked portion of interface");
}

void UModioDefaultModalDialogImpl::AddDialogCanceledHandler_Implementation(const FModioDialogEvent& Handler)
{
	MODIO_INTERFACE_UNIMPLEMENTED_FUNCTION_MSG(
		"Dialog helper class only implements DialogButtonClicked portion of interface");
}

void UModioDefaultModalDialogImpl::AddDialogShownHandler_Implementation(const FModioDialogShownEvent& Handler)
{
	MODIO_INTERFACE_UNIMPLEMENTED_FUNCTION_MSG(
		"Dialog helper class only implements DialogButtonClicked portion of interface");
}

void UModioDefaultModalDialogImpl::RemoveDialogConfirmedHandler_Implementation(const FModioDialogEvent& Handler)
{
	MODIO_INTERFACE_UNIMPLEMENTED_FUNCTION_MSG(
		"Dialog helper class only implements DialogButtonClicked portion of interface");
}

void UModioDefaultModalDialogImpl::RemoveDialogCanceledHandler_Implementation(const FModioDialogEvent& Handler)
{
	MODIO_INTERFACE_UNIMPLEMENTED_FUNCTION_MSG(
		"Dialog helper class only implements DialogButtonClicked portion of interface");
}

void UModioDefaultModalDialogImpl::RemoveDialogShownHandler_Implementation(const FModioDialogShownEvent& Handler)
{
	MODIO_INTERFACE_UNIMPLEMENTED_FUNCTION_MSG(
		"Dialog helper class only implements DialogButtonClicked portion of interface");
}

void UModioDefaultModalDialogImpl::ShowModal_Implementation(bool bFocusCancelButton)
{
	ActivateWidget();
}

void UModioDefaultModalDialogImpl::CloseModal_Implementation()
{
	DeactivateWidget();
}

void UModioDefaultModalDialogImpl::AddDialogButtonClickedHandler_Implementation(
	const FModioDialogButtonClickedEvent& Handler)
{
	OnDialogButtonClicked.AddUnique(Handler);
}

void UModioDefaultModalDialogImpl::RemoveDialogButtonClickedHandler_Implementation(
	const FModioDialogButtonClickedEvent& Handler)
{
	OnDialogButtonClicked.Remove(Handler);
}
