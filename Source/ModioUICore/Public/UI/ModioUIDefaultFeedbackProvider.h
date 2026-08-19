/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UI/Interfaces/IModioUIInteractionFeedback.h"
#include "UObject/Object.h"

#include "ModioUIDefaultFeedbackProvider.generated.h"

/**
 * @brief Default implementation of IModioUIInteractionFeedback that resolves the sound and force feedback reference
 * tables for the given key and plays them via UModioUIFeedbackLibrary
 */
UCLASS()
class MODIOUICORE_API UModioUIDefaultFeedbackProvider : public UObject, public IModioUIInteractionFeedback
{
	GENERATED_BODY()

protected:
	virtual void NativePlayUISoundFeedback(USoundBase* UIFeedbackSound, UObject* WorldContextObject) override;
	virtual void NativePlayUIForceFeedback(UForceFeedbackEffect* UIFeedbackForceEffect,
										   APlayerController* PlayerController) override;
};
