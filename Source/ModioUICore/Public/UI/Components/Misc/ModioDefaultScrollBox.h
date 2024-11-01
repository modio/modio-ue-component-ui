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

#include "Components/ScrollBox.h"
#include "Containers/Ticker.h"
#include "CoreMinimal.h"
#include "UI/Components/ComponentHelpers.h"

#include "ModioDefaultScrollBox.generated.h"

class UWidgetNavigation;

/**
 * @default_impl_for Scroll Box
 * @brief A widget that allows scrolling of its content. Apart from the default scroll box functionality, this scroll box also supports scrolling by controller/keyboard navigation
 * @component_display_name Scroll Box
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioDefaultScrollBox : public UScrollBox
{
	GENERATED_BODY()

protected:
	/** Whether to scroll using the controller/keyboard navigation input. If true, the scroll box will scroll content
	 * when there's no visible/focusable widget in the navigated direction but there's room to scroll */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "mod.io|UI|Scroll Box", meta = (BlueprintProtected))
	bool bScrollByNavigationInput = false;

	/** The amount to scroll by the controller/keyboard input when there's no visible/focusable widget in the navigated
	 * direction but there's room to scroll. Only relevant if bScrollByNavigationInput is true */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,
			  meta = (EditCondition = "bScrollByNavigationInput", BlueprintProtected),
			  Category = "mod.io|UI|Scroll Box")
	int32 NavigationScrollOffsetStep = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (UIMin = 0.0, UIMax = 1.0, BlueprintProtected),
			  Category = "mod.io|UI|Scroll Box")
	float NavigationScrollOffsetSpeed = 0.1f;

public:
	//~ Begin UScrollBox Interface
	virtual void SynchronizeProperties() override;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UScrollBox Interface

protected:
	/**
	 * Called when the user scrolls the scroll box
	 * @param CurrentOffset The current scroll offset
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Scroll Box", meta = (BlueprintProtected))
	void OnUserScrolledHandle(float CurrentOffset);

	/**
	 * Updates navigation data based on the pending scroll offset
	 * @param PendingScrollOffset The pending scroll offset (may differ from the actual current scroll offset due to
	 * ongoing scrolling animation, if any)
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Scroll Box", meta = (BlueprintProtected))
	void UpdateNavigationData(int32 PendingScrollOffset);

	/**
	 * Handles custom navigation when the scroll box reaches its boundary during navigation
	 * @param InNavigation The navigation being handled
	 * @return The widget to navigate to
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "mod.io|UI|Scroll Box", meta = (BlueprintProtected))
	UWidget* HandleCustomBoundaryNavigation(EUINavigation InNavigation);

	/** User-defined navigation data to apply when the scroll box can no longer scroll. Relevant only if
	 * bScrollByNavigationInput is true */
	UPROPERTY(Instanced, BlueprintReadOnly, Category = "mod.io|UI|Scroll Box", meta = (BlueprintProtected))
	TObjectPtr<UWidgetNavigation> UserDefinedNavigation;

protected:
	/** Handle to the navigation data tick */
	FTSTicker::FDelegateHandle NavigationDataTickHandle;

	MODIO_WIDGET_PALETTE_OVERRIDE();
};