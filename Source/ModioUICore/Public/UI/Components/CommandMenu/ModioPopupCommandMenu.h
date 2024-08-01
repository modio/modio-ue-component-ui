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

#include "ModioPopupCommandMenu.generated.h"

/**
 * @default_impl_for Popup Command Menu
 * @brief A widget that displays a popup command menu
 * @component_display_name Popup Command Menu
 */
UCLASS()
class MODIOUICORE_API UModioPopupCommandMenu : public UModioUIComponentBase, public IModioUICommandMenu
{
	GENERATED_BODY()
protected:
	//~ Begin IModioUICommandMenu Interface
	virtual void AddCommandListBuilder_Implementation(const FModioCommandListBuilder& Builder) override;
	virtual void RemoveCommandListBuilder_Implementation(const FModioCommandListBuilder& Builder) override;
	virtual void RebuildCommandList_Implementation() override;
	//~ End IModioUICommandMenu Interface

	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativeSetDataSource(UObject* InDataSource) override;
	//~ End IModioUIDataSourceWidget Interface

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

	/// @brief Implement this function to construct the actual command menu widget that will be displayed as the menu
	/// when the popup button is clicked
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Popup Command Menu",
			  meta = (BlueprintProtected))
	void ConstructCommandMenuWidget();

	/**
	 * @brief Provides a reference to the internal widget that displays the command menu
	 * @return The widget that displays the command menu
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Popup Command Menu|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUICommandMenu> GetCommandMenuWidget() const;
};
