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
#include "UObject/Interface.h"

#include "IModioUIInteractionFeedback.generated.h"

class APlayerController;

/**
 * @brief Interface for objects that play sound and force feedback on in response to a UI interaction
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Feedback",
		   Meta = (DocGroup = "UI",
				   Comment = "@brief Interface for objects that play feedback in response to a UI interaction"))
class MODIOUICORE_API UModioUIInteractionFeedback : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIInteractionFeedback : public IInterface
{
	GENERATED_BODY()

protected:
	virtual void NativePlayUISoundFeedback(USoundBase* UIFeedbackSound, UObject* WorldContextObject) {}

	virtual void NativePlayUIForceFeedback(UForceFeedbackEffect* UIFeedbackForceEffect,
										   APlayerController* PlayerController)
	{}

	void PlayUISoundFeedback_Implementation(USoundBase* UIFeedbackSound, UObject* WorldContextObject)
	{
		NativePlayUISoundFeedback(UIFeedbackSound, WorldContextObject);
	}

	void PlayUIForceFeedback_Implementation(UForceFeedbackEffect* UIFeedbackForceEffect,
											APlayerController* PlayerController)
	{
		NativePlayUIForceFeedback(UIFeedbackForceEffect, PlayerController);
	}

public:
	/**
	 * @brief Plays the sound feedback associated with the given interaction key
	 * @param UIFeedbackSound The feedback sound to play
	 * @param WorldContextObject Object used to resolve the world for sound playback (e.g. the calling widget)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Feedback")
	void PlayUISoundFeedback(USoundBase* UIFeedbackSound, UObject* WorldContextObject);

	/**
	 * @brief Plays the force feedback associated with the given interaction key
	 * @param UIFeedbackForceEffect The force feedback effect to play
	 * @param PlayerController The player to play force feedback on
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Feedback")
	void PlayUIForceFeedback(UForceFeedbackEffect* UIFeedbackForceEffect, APlayerController* PlayerController);
};
