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

#include "ModioUISubsystem.h"
#include "UObject/Interface.h"

#include "IModioUIAuthenticationChangedReceiver.generated.h"

/**
 * @brief Interface for classes that wish to be informed when the authentication state of the current user changes
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Events|Authentication", Meta = (DocGroup = "Event Handler Interfaces"))
class MODIOUICORE_API UModioUIAuthenticationChangedReceiver : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIAuthenticationChangedReceiver : public IInterface
{
	GENERATED_BODY()

	bool bRoutedAuthenticationChanged = false;

	void UserAuthenticationChangedHandler(TOptional<FModioUser> User);

protected:
	template<typename ImplementingClass>
	void Register()
	{
		UModioUISubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>();
		if (Subsystem)
		{
			Subsystem->OnUserChanged.AddUObject(
				Cast<ImplementingClass>(this),
				&IModioUIAuthenticationChangedReceiver::UserAuthenticationChangedHandler);

			// Update with the "current" state of user authentication
			UserAuthenticationChangedHandler(GEngine->GetEngineSubsystem<UModioSubsystem>()->QueryUserProfile());
		}
	}

	virtual void NativeOnAuthenticationChanged(TOptional<FModioUser> User);

	UFUNCTION(BlueprintImplementableEvent, Category = "mod.io|UI|Events|Authentication", meta = (BlueprintProtected))
	void OnAuthenticationChanged(FModioOptionalUser User);
};
