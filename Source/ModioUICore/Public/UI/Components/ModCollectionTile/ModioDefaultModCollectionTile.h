/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUIHoverableWidget.h"
#include "UI/Interfaces/IModioUISelectableWidget.h"
#include "ModioDefaultModCollectionTile.generated.h"

/**
 * @default_impl_for Mod Collection Tile
 * @brief This is a default implementation of the Mod Collection Tile component which is intended to represent a mod
 * collection in Mod List and Mod Collection Tile View components. 
 * @component_display_name Mod Collection Tile
 */
UCLASS()
class MODIOUICORE_API UModioDefaultModCollectionTile
	: public UModioUIComponentBase,
	  public IModioUIClickableWidget,
	  public IModioUISelectableWidget,
	  public IModioUIHoverableWidget,
	  public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	bool bEmitClickEvents = true;

	/**
	 * @brief Passes `this` as ClickContext
	 * @default_component_event FModioClickableOnClicked
	 */
	UPROPERTY()
	FModioClickableOnClickedMulticast OnClicked;

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Events|Mod Collection Tile", meta = (BlueprintProtected))
	void NotifyClicked();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "mod.io|UI|Mod Collection Tile",
		meta = (BlueprintProtected))
	bool bSelected = false;
	bool bSelectable = true;
	bool bToggleable = false;

	/**
	 * @brief Passes `this` as SelectionContext, `bNewSelectedState` is new selection state
	 * @default_component_event FModioSelectableOnSelectionChanged
	 */
	UPROPERTY()
	FModioSelectableOnSelectionChangedMulticast OnSelected;
	// Selection state doesnt need to be internally triggered, so no Notify* function

	/**
	 * @brief Handles both getting the selected state from an associated list if there is one, and falls back on the local state variable otherwise
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Collection Tile", meta = (BlueprintProtected))
	bool GetSelectedStateEx();

	bool bEmitHoverEvents = true;
	/**
	 * @brief Passes `this` as HoverContext, `bHoveredState` is new hover state
	 * @default_component_event FModioHoverableOnHoverStateChanged
	 */
	UPROPERTY()
	FModioHoverableOnHoverStateChangedMulticast OnHoverStateChanged;

	/**
	 * @brief Notifies the component that the hover state has changed
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Events|Mod Collection Tile", meta = (BlueprintProtected))
	void NotifyHoverStateChanged(bool bNewHoveredState);

	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

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

	virtual void AddClickedHandler_Implementation(const FModioClickableOnClicked& Handler) override;

	virtual void RemoveClickedHandler_Implementation(const FModioClickableOnClicked& Handler) override;

	virtual void EnableClick_Implementation() override;

	virtual void DisableClick_Implementation() override;

	virtual void AddHoverStateChangedHandler_Implementation(const FModioHoverableOnHoverStateChanged& Handler) override;

	virtual void RemoveHoverStateChangedHandler_Implementation(
		const FModioHoverableOnHoverStateChanged& Handler) override;

	virtual void EnableHoverEvents_Implementation() override;

	virtual void DisableHoverEvents_Implementation() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Collection Tile",
		meta = (BlueprintProtected))
	void PopulateExtraOptionsCommands(TArray<UObject*>& Commands, UObject* Context);

	#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Data Binding|Mod Collection Tile",
		meta = (BlueprintProtected))
	TObjectPtr<UObject> EditorDataSource;
	#endif

	virtual void NativePreConstruct() override;

	virtual void NativeSetDataSource(UObject* InDataSource) override;
	virtual bool IsListItemSelectable() const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Mod Collection Tile|Widget Getters",
		meta = (BlueprintProtected))
	TScriptInterface<IModioUICommandMenu> GetMoreOptionsMenuWidget() const;
};