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

#include "Components/ProgressBar.h"
#include "CoreMinimal.h"
#include "UI/Components/ComponentHelpers.h"
#include "UI/Interfaces/IModioUIHasTooltipWidget.h"
#include "UI/Interfaces/IModioUIProgressWidget.h"

#include "ModioDefaultProgressBar.generated.h"

/**
 * @default_impl_for Progress Bar
 * @brief The Progress Bar is a default implementation of the Progress component which utilizes UProgressBar for the underlying functionality.
 * @component_display_name Progress Bar
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioDefaultProgressBar : public UProgressBar,
												 public IModioUIProgressWidget,
												 public IModioUIHasTooltipWidget
{
	GENERATED_BODY()

protected:
	TOptional<FLinearColor> DefaultFillColor;

	FModioProgressWidgetValueChangedMulticast OnValueChanged;

	//~ Begin IModioUIProgressWidget Interface
	virtual void AddValueChangedHandler_Implementation(const FModioProgressWidgetValueChanged& Handler) override;
	virtual void RemoveValueChangedHandler_Implementation(const FModioProgressWidgetValueChanged& Handler) override;
	virtual void SetColorOverride_Implementation(FLinearColor Override) override;
	virtual void ClearColorOverride_Implementation() override;
	virtual float GetProgress_Implementation() override;
	virtual void SetProgress_Implementation(float NewProgressValue) override;
	virtual void SetMarquee_Implementation(bool bNewIsMarquee) override;
	//~ End IModioUIProgressWidget Interface

	//~ Begin IModioUIHasTooltipWidget Interface
	virtual void SetTooltipEnabledState_Implementation(bool bNewEnabledState) override;
	virtual void ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText,
												 const FText& TagText) override;
	//~ End IModioUIHasTooltipWidget Interface

	MODIO_WIDGET_PALETTE_OVERRIDE();
};
