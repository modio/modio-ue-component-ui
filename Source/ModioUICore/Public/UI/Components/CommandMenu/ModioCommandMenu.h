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
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/Interfaces/IModioUICommandMenu.h"
#include "UI/Interfaces/IModioUIObjectSelector.h"

#include "ModioCommandMenu.generated.h"

/**
 * @default_impl_for Command Menu
 * @brief The Command Menu is a default implementation of the Command Menu component which uses an internal
 * sub-component implementing IModioUIObjectSelector to display a set of commands to users and allow them to select one.
 * When a command is selected, the Command Menu will query the selected command object to check if the command can be
 * executed and invoke it if so.
 * @component_display_name Command Menu
 */
UCLASS()
class MODIOUICORE_API UModioCommandMenu : public UModioUIComponentBase, public IModioUICommandMenu
{
	GENERATED_BODY()
protected:
	/**
	 * @brief Provides a reference to the internal widget that actually displays the available commands and and tracks
	 * the user's selection. The default implementation will forward IModioUIObjectSelector messages to this widget if
	 * it is valid, and assumes that this widget will be responsible for data-binding the command objects to each entry
	 * widget, and that those entry widgets will be responsible for inspecting their bound object for any information they want to visualize.
	 * @return The selector widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Menu|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIObjectSelector> GetSelectorWidget() const;

	/**
	 * @brief The list of command objects that the menu should display
	 */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> CommandObjects;

	FModioCommandListBuilderMulticast OnBuildCommandList;

	//~ Begin IModioUICommandMenu Interface
	virtual void AddCommandListBuilder_Implementation(const FModioCommandListBuilder& Builder) override;
	virtual void RemoveCommandListBuilder_Implementation(const FModioCommandListBuilder& Builder) override;
	virtual void RebuildCommandList_Implementation() override;
	//~ End IModioUICommandMenu Interface

	/**
	 * @brief Called when a command is selected by the user
	 * @param SelectedCommandObject The command object that was selected
	 */
	UFUNCTION(meta = (BlueprintProtected))
	void OnCommandSelected(UObject* SelectedCommandObject);

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface
};
