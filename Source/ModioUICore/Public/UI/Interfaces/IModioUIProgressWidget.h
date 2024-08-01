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
#include "Templates/SubclassOf.h"
#include "UObject/Interface.h"
#include "Widgets/Notifications/SProgressBar.h"

#include "IModioUIProgressWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FModioProgressWidgetValueChanged, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioProgressWidgetValueChangedMulticast, float, NewValue);

/**
 * @brief Interface for widgets that display progress based on a scalar value 0..1
*/
UINTERFACE(BlueprintType, Category = "mod.io|UI|Progress", Meta = (DocGroup = "UI", Comment = "@brief Interface for widgets that display progress based on a scalar value 0..1"))
class MODIOUICORE_API UModioUIProgressWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIProgressWidget : public IInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief Registers a delegate to receive callbacks when the progress value of the implementing widget changes
	 * @param Handler Delegate to receive notifications on value changed events
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|ProgressWidget")
	void AddValueChangedHandler(const FModioProgressWidgetValueChanged& Handler);

	/**
	 * @brief Unsubscribes a delegate so it no longer receives callbacks when the implementing widget's progress value changes
	 * @param Handler Delegate to remove from the notification list
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|ProgressWidget")
	void RemoveValueChangedHandler(const FModioProgressWidgetValueChanged& Handler);

	/**
	 * @brief Overrides the current color being used to indicate progress on the widget
	 * @param Override The new color to use
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ProgressWidget")
	void SetColorOverride(FLinearColor Override);

	/**
	 * @brief Resets the color being used to indicate progress to the default implementation-defined value
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ProgressWidget")
	void ClearColorOverride();

	/**
	 * @brief Retrieves the current progress value of the implementing widget
	 * @return Progress Value in the range 0..1
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ProgressWidget")
	float GetProgress();

	/**
	 * @brief Directly sets the current progress value of the implementing widget
	 * @param NewProgressValue The new progress value
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ProgressWidget")
	void SetProgress(float NewProgressValue);

	/**
	 * @brief Sets whether the progress widget should be a marquee/throbber
	 * @param bNewIsMarquee True if the progress widget should show indeterminate progress
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|ProgressWidget")
	void SetMarquee(bool bNewIsMarquee);
};
