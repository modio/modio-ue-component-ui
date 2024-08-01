/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIAuthenticationChangedReceiver.h"

void IModioUIAuthenticationChangedReceiver::NativeOnAuthenticationChanged(TOptional<FModioUser> User)
{
	bRoutedAuthenticationChanged = true;
	Execute_OnAuthenticationChanged(Cast<UObject>(this), FModioOptionalUser {User});
}

void IModioUIAuthenticationChangedReceiver::UserAuthenticationChangedHandler(TOptional<FModioUser> User)
{
	bRoutedAuthenticationChanged = false;
	NativeOnAuthenticationChanged(User);
}
