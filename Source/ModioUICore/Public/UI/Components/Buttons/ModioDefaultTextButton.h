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

#include "CoreMinimal.h"
#include "UI/Components/Buttons/ModioButtonWidget.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUIHasTooltipWidget.h"
#include "UI/Interfaces/IModioUIHoverableWidget.h"
#include "UI/Interfaces/IModioUISelectableWidget.h"

#include "ModioDefaultTextButton.generated.h"

/**
 * @default_impl_for Button
 * @brief The Text Button is a default implementation of the Button component which implements IModioUIHasTextWidget
 * to allow an associated FText to be set at runtime. The default blueprint implementation visualizes this FText on a
 * static text widget which functions as the button's label.
 * @component_display_name Text Button
 * @required_interface IModioUIHasTextWidget|Allows the widget to be sent an FText to display at runtime
 */
UCLASS()
class MODIOUICORE_API UModioDefaultTextButton : public UModioButtonWidget, public IModioUIHasTextWidget
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Gets a reference to the widget responsible for displaying the button's label. The default implementation
	 * will forward IModioUIHasTextWidget messages to this widget if it is valid
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Text Button|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetLabelWidget() const;

	/**
	 * @brief Text to display on this button's label. For runtime changes, use IModioUIHasTextWidget::SetText
	 * @default_component_property
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Text Button|Localization",
			  meta = (BlueprintProtected))
	FText LabelText = FText::FromString("Button");

	//~ Begin IModioUIHasTextWidget Interface
	virtual void NativeSetText(const FText& DisplayText) override;
	virtual FText NativeGetText() override;
	virtual void NativeSetTextFont(FSlateFontInfo FontInfo) override;
	virtual FSlateFontInfo NativeGetTextFont() override;
	//~ End IModioUIHasTextWidget Interface

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface
};
