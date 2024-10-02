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

#include "ModioModTagSelectorEntry.generated.h"

/**
 * @default_impl_for Mod Tag Selector Entry
 * @brief A widget that represents a tag in a tag selector
 * @component_display_name Mod Tag Selector Entry
 */
UCLASS()
class MODIOUICORE_API UModioModTagSelectorEntry : public UModioUIComponentBase,
												  public IUserObjectListEntry,
												  public IModioUISelectableWidget,
												  public IModioUIClickableWidget,
												  public IModioUIHasTextWidget
{
	GENERATED_BODY()
protected:
	bool bAllowSelection = true;
	bool bAllowToggleable = true;
	bool bAllowClick = true;

	bool bCurrentSelectionState = false;

	/**
	 * @brief Notify listeners that the selection state of this entry has changed
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Events|Tag Selector", meta = (BlueprintProtected))
	void NotifySelectionChanged();

	/**
	 * @brief Notify listeners that this entry has been clicked
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Events|Tag Selector", meta = (BlueprintProtected))
	void NotifyClicked();

	/**
	 * @brief Passes `this` as SelectionContext, `bNewSelectedState` is new selection state
	 * @default_component_event FModioSelectableOnSelectionChanged
	 */
	UPROPERTY()
	FModioSelectableOnSelectionChangedMulticast OnSelectionStateChanged;
	
	/**
	 * @brief Passes `this` as ClickContext
	 * @default_component_event FModioClickableOnClicked
	 */
	UPROPERTY()
	FModioClickableOnClickedMulticast OnClicked;

	//~ Begin UUserWidget Interface
	virtual void NativeConstruct() override;
	//~ End UUserWidget Interface

	//~ Begin IUserObjectListEntry Interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//~ End IUserObjectListEntry Interface

	/**
	 * @brief Get the widget that displays the tag text for this entry
	 * @return The text widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetTagLabelWidget() const;

	//~ Begin IModioUIHasTextWidget Interface
	virtual void SetWidgetText_Implementation(const FText& DisplayText) override;
	virtual FText GetWidgetText_Implementation() override;
	//~ End IModioUIHasTextWidget Interface

	/**
	 * @brief Get the widget that can be clicked to select this entry
	 * @return The clickable widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIClickableWidget> GetClickableWidget() const;

	/**
	 * @brief Handle the click event for this entry
	 * @param ClickedObject The object that was clicked
	 */
	UFUNCTION(meta = (BlueprintProtected))
	void OnTagEntryClicked(UObject* ClickedObject);

	//~ Begin IModioUIClickableWidget Interface
	virtual void AddClickedHandler_Implementation(const FModioClickableOnClicked& Handler) override;
	virtual void RemoveClickedHandler_Implementation(const FModioClickableOnClicked& Handler) override;
	virtual void EnableClick_Implementation() override;
	virtual void DisableClick_Implementation() override;
	//~ End IModioUIClickableWidget Interface

	/**
	 * @brief Get the widget that represents the selection state of this entry
	 * @return The selection widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUISelectableWidget> GetSelectionWidget() const;

	//~ Begin IModioUISelectableWidget Interface
	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	virtual bool IsListItemSelectable() const override;
	virtual void AddSelectedStateChangedHandler_Implementation(const FModioSelectableOnSelectionChanged& Handler) override;
	virtual void RemoveSelectedStateChangedHandler_Implementation(const FModioSelectableOnSelectionChanged& Handler) override;
	virtual void SetSelectable_Implementation(bool bNewSelectableState) override;
	virtual void GetSelectable_Implementation(bool& bIsSelectable) override;
	virtual void SetSelectedState_Implementation(bool bNewSelectedState) override;
	virtual void ToggleSelectedState_Implementation() override;
	virtual void GetSelectedState_Implementation(bool& bCurrentSelectedState) override;
	virtual void SetToggleable_Implementation(bool bNewToggleableState) override;
	virtual void GetToggleable_Implementation(bool& bIsToggleable) override;
	//~ End IModioUISelectableWidget Interface

	//~ Begin IUserListEntry Interface
	virtual void NativeOnEntryReleased() override;
	//~ End IUserListEntry Interface
};