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
 * @default_impl_for Command Menu
 * @brief The Popup Command Menu is a default implementation of the Command Menu component which is intended as an
 * abstract base class for a widget that manages an internal sub-component that can be shown and dismissed based on user
 * input. This base class provides an overridable function to construct the internal menu sub-component that will be
 * displayed on request, and handles forwarding IModioUICommandMenu messages to that widget. Derived classes should
 * provide an implementation of ConstructCommandMenuWidget, and their own custom behaviour for showing and dismissing
 * that widget instance.
 * @component_display_name Popup Command Menu
 */
UCLASS(abstract)
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

	/// @brief Implement this function to construct the actual command menu widget that will be displayed and dismissed
	/// on user input. Implementing classes are responsible for holding onto this internal sub-component so it is not
	/// garbage collected.
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
