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
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUIObjectListWidget.h"
#include "UI/Interfaces/IModioUIObjectSelector.h"

#include "ModioModTagSelectorCategory.generated.h"

/**
 * @default_impl_for Mod Tag Selector Category
 * @brief A widget that displays a category of tags that can be selected
 * @component_display_name Mod Tag Selector Category
 */
UCLASS()
class MODIOUICORE_API UModioModTagSelectorCategory : public UModioUIComponentBase,
													 public IUserObjectListEntry,
													 public IModioUIObjectSelector,
													 public IModioUIHasTextWidget
{
	GENERATED_BODY()

protected:
	//~ Begin IUserObjectListEntry Interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//~ End IUserObjectListEntry Interface

	/**
	 * @brief Provides a reference to the internal widget that actually displays the tags for this category
	 * @return The widget that displays the tags
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Tag Selector Category|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIObjectSelector> GetCategoryTagListWidget() const;

	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativeSetDataSource(UObject* InDataSource) override;
	//~ End IModioUIDataSourceWidget Interface

	//~ Begin IModioUIObjectSelector Interface
	virtual void SetValues_Implementation(const TArray<UObject*>& InValues) override;
	virtual UObject* GetSingleSelectedValue_Implementation() override;
	virtual int32 GetNumEntries_Implementation() override;
	virtual int32 GetSingleSelectionIndex_Implementation() override;
	virtual void ClearSelectedValues_Implementation() override;
	virtual void SetSingleSelectionByIndex_Implementation(int32 Index, bool bEmitSelectionEvent) override;
	virtual void SetSingleSelectionByValue_Implementation(UObject* Value, bool bEmitSelectionEvent) override;
	virtual void AddSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler) override;
	virtual void RemoveSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler) override;
	virtual void SetSelectedStateForIndex_Implementation(int32 Index, bool bNewSelectionState, bool bEmitSelectionEvent) override;
	virtual void SetSelectedStateForValue_Implementation(UObject* Value, bool bNewSelectionState, bool bEmitSelectionEvent) override;
	virtual TArray<UObject*> GetSelectedValues_Implementation() override;
	virtual void SetMultiSelectionAllowed_Implementation(bool bMultiSelectionAllowed) override;
	virtual bool GetMultiSelectionAllowed_Implementation() override;
	virtual int32 GetIndexForValue_Implementation(UObject* Value) const override;
	//~ End IModioUIObjectSelector Interface

	/**
	 * @brief Get the label widget that displays the category name
	 * @return The label widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Tag Selector Category|Widget Getters", meta = (BlueprintProtected))
	TScriptInterface<IModioUIHasTextWidget> GetTagCategoryLabel() const;

	//~ Begin IModioUIHasTextWidget Interface
	virtual void SetWidgetText_Implementation(const FText& DisplayText) override;
	virtual FText GetWidgetText_Implementation() override;
	//~ End IModioUIHasTextWidget Interface

	//~ Begin IUserListEntry Interface
	virtual void NativeOnEntryReleased() override;
	//~ End IUserListEntry Interface
};
