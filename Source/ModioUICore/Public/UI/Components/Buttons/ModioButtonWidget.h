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

#include "CommonButtonBase.h"
#include "CoreMinimal.h"
#include "UI/Components/ComponentHelpers.h"
#include "UI/Interfaces/IModioFocusableWidget.h"
#include "UI/Interfaces/IModioUIBoundActionWidget.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Interfaces/IModioUIHasTooltipWidget.h"
#include "UI/Interfaces/IModioUIHoverableWidget.h"
#include "UI/Interfaces/IModioUISelectableWidget.h"

#include "ModioButtonWidget.generated.h"

class UNamedSlot;

/**
 * @brief The button component is a basic building block in the mod.io Component UI framework. It implements interfaces
 * to support data binding as well as click, selection and hover events.
 * @component_display_name Button
 * @required_interface IModioUIClickableWidget|Allows the widget to emit events when clicked
 * @required_interface IModioUISelectableWidget|Allows the widget to be configured as selectable
 * @required_interface IModioUIHoverableWidget|Allows the widget to emit events when hovered
 * @required_interface IModioUIHasTooltipWidget|Allows the widget to have its tooltip externally configured
 * @required_interface IModioUIDataSourceWidget|Allows the widget to data-bind to a UObject*
 */
UCLASS(Abstract, meta = (ModioWidget))
class MODIOUICORE_API UModioButtonWidget : public UCommonButtonBase,
										   public IModioUIBoundActionWidget,
										   public IModioUISelectableWidget,
										   public IModioUIHoverableWidget,
										   public IModioUIHasTooltipWidget,
										   public IModioUIDataSourceWidget,
										   public IModioFocusableWidget
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Passes `this` as ClickContext
	 * @default_component_event FModioClickableOnClicked
	 */
	UPROPERTY()
	FModioClickableOnClickedMulticast OnClicked;

	/**
	 * @brief Cached object to use as the source for data binding
	 * Populated by SetDataSource
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Data Binding", Transient,
			  meta = (BlueprintProtected))
	TObjectPtr<UObject> DataSource;

	/**
	 * @brief Whether the button should focus on hover. This is useful for gamepad navigation
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Button")
	bool bFocusOnHover = true;

	/**
	 * @brief Sets the focus on the button
	 * This function performs the "deep" focus on the Common UI button, which means that it will set the focus on the button itself
	 * This is useful since UCommonButtonBase is derived from UUserWidget, which doesn't support focus when setting it directly
	 */
	void SetCommonUIButtonFocus();

	/**
	 * @brief Gets the Slate button widget
	 * The button is highly encapsulated and this function tries to scan the widget tree to find the button
	 * @return The Slate button widget if found, nullptr otherwise
	 */
	TSharedPtr<SButton> GetSlateButton() const;

	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativeSetDataSource(UObject* InDataSource) override;
	virtual UObject* NativeGetDataSource() override;
	//~ End IModioUIDataSourceWidget Interface

	//~ Begin IModioUIClickableWidget Interface
	virtual void AddClickedHandler_Implementation(const FModioClickableOnClicked& Handler) override;
	virtual void RemoveClickedHandler_Implementation(const FModioClickableOnClicked& Handler) override;
	virtual void EnableClick_Implementation() override;
	virtual void DisableClick_Implementation() override;
	//~ End IModioUIClickableWidget Interface

	FDataTableRowHandle StoredInputActionRow;
	FModioBoundActionProgressed OnModioActionProgressed;
	FModioBoundActionCompleted OnModioActionCompleted;

	//~ Begin IModioUIBoundActionWidget Interface
	virtual void SetBoundAction_Implementation(const FDataTableRowHandle& InputActionRow) override;
	virtual bool TryGetBoundAction_Implementation(FDataTableRowHandle& InputActionRow) const override;
	virtual void ClearBoundAction_Implementation() override;
	virtual void AddActionProgressHandler_Implementation(const FModioBoundActionProgressed& Handler) override;
	virtual void RemoveActionProgressHandler_Implementation(const FModioBoundActionProgressed& Handler) override;
	virtual void AddActionCompletedHandler_Implementation(const FModioBoundActionCompleted& Handler) override;
	virtual void RemoveActionCompletedHandler_Implementation(const FModioBoundActionCompleted& Handler) override;
	//~ End IModioUIBoundActionWidget Interface

	//~ Begin IModioFocusableWidget Interface
	virtual UWidget* NativeGetWidgetToFocus(EUINavigation NavigationType) const override;
	virtual void NativeAddFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler) override;
	virtual void NativeRemoveFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler) override;
	//~ End IModioFocusableWidget Interface

	/**
	 * @brief Passes `this` as FocusContext, `InFocusEvent` as the focus event, `bIsFocused` as the new focus state
	 * @default_component_event FModioOnFocusPathChanged
	 */
	UPROPERTY()
	FModioOnFocusPathChangedMulticast OnModioFocusPathChanged;

	/**
	 * @brief Passes `this` as SelectionContext, `bNewSelectedState` is new selection state
	 * @default_component_event FModioSelectableOnSelectionChanged
	 */
	UPROPERTY()
	FModioSelectableOnSelectionChangedMulticast OnSelected;

	//~ Begin IModioUISelectableWidget Interface
	virtual void AddSelectedStateChangedHandler_Implementation(
		const FModioSelectableOnSelectionChanged& Handler) override;
	virtual void RemoveSelectedStateChangedHandler_Implementation(
		const FModioSelectableOnSelectionChanged& Handler) override;
	virtual void SetSelectable_Implementation(bool bNewSelectableState) override;
	virtual void GetSelectable_Implementation(bool& bIsSelectable) override;
	virtual void SetSelectedState_Implementation(bool bNewSelectedState) override;
	virtual void ToggleSelectedState_Implementation() override;
	virtual void GetSelectedState_Implementation(bool& bCurrentSelectedState) override;
	virtual void SetToggleable_Implementation(bool bNewToggleableState) override;
	virtual void GetToggleable_Implementation(bool& bIsToggleable) override;
	//~ End IModioUISelectableWidget Interface

	/**
	 * @brief Passes `this` as HoverContext, `bHoveredState` is new hover state
	 * @default_component_event FModioHoverableOnHoverStateChanged
	 */
	UPROPERTY()
	FModioHoverableOnHoverStateChangedMulticast OnHoverStateChanged;

	bool bIsHoverable = true;

	//~ Begin IModioUIHoverableWidget Interface
	virtual void AddHoverStateChangedHandler_Implementation(const FModioHoverableOnHoverStateChanged& Handler) override;
	virtual void RemoveHoverStateChangedHandler_Implementation(
		const FModioHoverableOnHoverStateChanged& Handler) override;
	virtual void EnableHoverEvents_Implementation() override;
	virtual void DisableHoverEvents_Implementation() override;
	//~ End IModioUIHoverableWidget Interface

	//~ Begin IModioUIHasTooltipWidget Interface
	virtual void SetTooltipEnabledState_Implementation(bool bNewEnabledState) override;
	virtual void ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText,
												 const FText& TagText) override;
	//~ End IModioUIHasTooltipWidget Interface

	//~ Begin UCommonButtonBase Interface
	virtual void NativeOnClicked() override;
	virtual void NativeOnActionComplete() override;
	virtual void NativeOnActionProgress(float HeldPercent) override;
	virtual void NativeOnSelected(bool bBroadcast) override;
	virtual void NativeOnDeselected(bool bBroadcast) override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	//~ End UCommonButtonBase Interface

	//~ Begin UUserWidget Interface
	virtual void HandleFocusReceived() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	//~ End UUserWidget Interface

	MODIO_WIDGET_PALETTE_OVERRIDE();
};
