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

#include "Blueprint/IUserObjectListEntry.h"
#include "CoreMinimal.h"
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUISelectableWidget.h"

#include "ModioPresetFilterEntryWidget.generated.h"

/**
 * @default_impl_for Preset Filter Entry
 * @brief A widget that represents a filter entry in a filter preset
 * @component_display_name Preset Filter Entry
 */
UCLASS()
class MODIOUICORE_API UModioPresetFilterEntryWidget : public UModioUIComponentBase,
													  public IUserObjectListEntry,
													  public IModioUIHasTextWidget,
													  public IModioUIClickableWidget,
													  public IModioUISelectableWidget
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Provides a reference to the internal widget that displays the label for this filter entry
	 * @return The widget that displays the label
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetLabelWidget() const;

	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativeSetDataSource(UObject* InDataSource) override;
	//~ End IModioUIDataSourceWidget Interface

	//~ Begin IModioUIHasTextWidget Interface
	virtual void NativeSetText(const FText& DisplayText) override;
	virtual FText NativeGetText() override;
	virtual void NativeSetTextFont(FSlateFontInfo FontInfo) override;
	virtual FSlateFontInfo NativeGetTextFont() override;
	//~ End IModioUIHasTextWidget Interface
};
