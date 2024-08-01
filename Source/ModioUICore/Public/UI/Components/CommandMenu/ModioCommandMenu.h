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
 * @brief A menu that displays a list of commands that can be executed by the user
 */
UCLASS()
class MODIOUICORE_API UModioCommandMenu : public UModioUIComponentBase, public IModioUICommandMenu
{
	GENERATED_BODY()
protected:
	/**
	 * @brief The selector widget for the command menu (e.g. the widget that allows the user to select a command)
	 * @return The selector widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Menu|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIObjectSelector> GetSelectorWidget() const;

	/**
	 * @brief The list of command objects that the menu should display
	 */
	UPROPERTY(Transient)
	TArray<UObject*> CommandObjects;

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
