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

#include "Components/ContentWidget.h"
#include "CoreMinimal.h"
#include "UI/Components/ComponentHelpers.h"

#include "ModioFeatureVisibilityWidget.generated.h"

enum class EModioUIFeatureFlags : uint8;
/**
 * @default_impl_for Feature Visibility Widget
 * @brief A widget that manages the visibility of its children based on the enabled state of a mod.io feature
 * @component_display_name Feature Visibility Widget
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioFeatureVisibilityWidget : public UContentWidget
{
	GENERATED_BODY()
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	UModioFeatureVisibilityWidget(const FObjectInitializer& ObjectInitializer);
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Feature Visibility Widget",
			  meta = (BlueprintProtected))
	EModioUIFeatureFlags FeatureToCheck;

	MODIO_WIDGET_PALETTE_OVERRIDE();
};
