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
#include "UI/Interfaces/IModioUISelectableWidget.h"

#include "ModioEnumSelectorEntry.generated.h"

/**
 * @default_impl_for Enum Selector Entry
 * @brief the Enum Selector Entry is a default implementation of the Enum Selector Entry component. This default
 * implementation uses a subwidget for a text label for the enum value, a clickable subwidget to allow users to make
 * selections, and a selectable subwidget to visually represent the selection state of the associated enum value in the
 * selector.
 * @component_display_name Enum Selector Entry
 */
UCLASS()
class MODIOUICORE_API UModioEnumSelectorEntry : public UModioUIComponentBase,
												public IUserObjectListEntry,
												public IModioUIClickableWidget,
												public IModioUISelectableWidget
{
	GENERATED_BODY()

protected:
	IMPLEMENT_MODIO_WIDGET_GETTER_VALIDATION(GetLabelWidget(), GetClickableWidget());

	/**
	 * @brief Passes `this` as ClickContext
	 * @default_component_event FModioClickableOnClicked
	 */
	UPROPERTY()
	FModioClickableOnClickedMulticast OnClicked;

	bool bEmitClickEvents = true;

	UFUNCTION(meta = (BlueprintProtected))
	void InternalClickHandler(UObject* ClickContext);

	/**
	 * @brief Notifies the component that the clickable widget has been clicked
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Events|Enum Selector Entry", meta = (BlueprintProtected))
	void NotifyClicked();

	/**
	 * @brief Passes `this` as SelectionContext, `bNewSelectedState` is new selection state
	 * @default_component_event FModioSelectableOnSelectionChanged
	 */
	UPROPERTY()
	FModioSelectableOnSelectionChangedMulticast OnSelectionChanged;

	UFUNCTION(meta = (BlueprintProtected))
	void InternalSelectionChangedHandler(UObject* SelectionContext, bool bNewSelectedState);

	/**
	 * @brief Notifies the component that the selection state has changed
	 * @param bNewSelectedState The new selection state
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Events|Enum Selector Entry", meta = (BlueprintProtected))
	void NotifySelectionChanged(bool bNewSelectedState);

	/**
	 * @brief Provides a reference to the internal widget that is used for displaying the display string for the bound enum value
	 * @return The label widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Enum Selector Entry|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetLabelWidget() const;

	/**
	 * @brief Provides a reference to the internal widget that will receive click/enter events so that the enclosing selector can change selection based on user input
	 * @return The clickable widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Enum Selector Entry|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetClickableWidget() const;

	/**
	 * @brief Provides a reference to the internal widget that will visualize the current selection state of the bound enum value
	 * @return The selectable widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Enum Selector Entry|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUISelectableWidget> GetSelectableWidget() const;

	virtual void NativeSetDataSource(UObject* InDataSource) override;

	virtual void AddClickedHandler_Implementation(const FModioClickableOnClicked& Handler) override;

	virtual void RemoveClickedHandler_Implementation(const FModioClickableOnClicked& Handler) override;

	virtual void EnableClick_Implementation() override;

	virtual void DisableClick_Implementation() override;

	virtual void NativePreConstruct() override;

	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual void AddSelectedStateChangedHandler_Implementation(const FModioSelectableOnSelectionChanged& Handler) override;

	virtual void RemoveSelectedStateChangedHandler_Implementation(const FModioSelectableOnSelectionChanged& Handler) override;

	virtual void SetSelectable_Implementation(bool bNewSelectableState) override;

	virtual void GetSelectable_Implementation(bool& bIsSelectable) override;

	virtual void SetSelectedState_Implementation(bool bNewSelectedState) override;

	virtual void ToggleSelectedState_Implementation() override;

	virtual void GetSelectedState_Implementation(bool& bCurrentSelectedState) override;

	virtual void SetToggleable_Implementation(bool bNewToggleableState) override;

	virtual void GetToggleable_Implementation(bool& bIsToggleable) override;

	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
};
