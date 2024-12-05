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
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUIImageDisplayWidget.h"
#include "UI/Interfaces/IModioUISelectableWidget.h"

#include "ModioCommandEntry.generated.h"

/**
 * @brief A UI component that represents a single command entry in a list of commands
 */
UCLASS()
class MODIOUICORE_API UModioCommandEntry : public UModioUIComponentBase,
										   public IUserObjectListEntry,
										   public IModioUIClickableWidget,
										   public IModioUISelectableWidget
{
	GENERATED_BODY()

protected:
	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativeSetDataSource(UObject* InDataSource) override;
	//~ End IModioUIDataSourceWidget Interface

	/**
	 * @brief The label widget for the command entry (e.g. the text of the command)
	 * @return The label widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Entry|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetLabelWidget() const;

	/**
	 * @brief The clickable widget for the command entry (e.g. the button that should be clicked to execute the command)
	 * @return The clickable widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Entry|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetClickableWidget() const;

	/**
	 * @brief The icon widget for the command entry (e.g. the icon that represents the command)
	 * @return The icon widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Entry|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIImageDisplayWidget> GetIconWidget() const;

	FModioClickableOnClickedMulticast OnInnerButtonClicked;

	/**
	 * @brief Handles the inner button being clicked
	 * @param ClickContext The context in which the button was clicked
	 */
	UFUNCTION(meta = (BlueprintProtected))
	void HandleInnerButtonClicked(UObject* ClickContext);

	//~ Begin IModioUIClickableWidget Interface
	virtual void AddClickedHandler_Implementation(const FModioClickableOnClicked& Handler) override;
	virtual void RemoveClickedHandler_Implementation(const FModioClickableOnClicked& Handler) override;
	virtual void EnableClick_Implementation() override;
	virtual void DisableClick_Implementation() override;
	//~ End IModioUIClickableWidget Interface

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface
};
