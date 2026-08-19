/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/ModioUIDefaultFeedbackProvider.h"

#include "Core/ModioForceFeedbackRef.h"
#include "Core/ModioSoundCueRef.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioUIDefaultFeedbackProvider)

void UModioUIDefaultFeedbackProvider::NativePlayUISoundFeedback(USoundBase* UIFeedbackSound,
																UObject* WorldContextObject)
{
	if (UIFeedbackSound == nullptr || WorldContextObject == nullptr)
	{
		return;
	}
	UGameplayStatics::PlaySound2D(WorldContextObject, UIFeedbackSound, 0.5f, 1.0f);
}

void UModioUIDefaultFeedbackProvider::NativePlayUIForceFeedback(UForceFeedbackEffect* UIFeedbackForceEffect,
																APlayerController* PlayerController)
{
	if (UIFeedbackForceEffect == nullptr || PlayerController == nullptr)
	{
		return;
	}
	PlayerController->ClientPlayForceFeedback(UIFeedbackForceEffect, FForceFeedbackParameters());
}
