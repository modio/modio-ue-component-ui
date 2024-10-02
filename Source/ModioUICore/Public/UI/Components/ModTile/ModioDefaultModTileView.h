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

#include "Components/TileView.h"
#include "CoreMinimal.h"
#include "Core/ModioStackedBool.h"
#include "UI/Components/ComponentHelpers.h"
#include "UI/Interfaces/IModioUIModListViewInterface.h"
#include "UI/Interfaces/IModioUIObjectListWidget.h"
#include "UI/Interfaces/IModioUIObjectSelector.h"

#include "ModioDefaultModTileView.generated.h"

/**
 * @brief A default implementation of IModioUIModListViewInterface which displays mods as tiles in a grid. This widget
 * inherits from UTileView and forwards calls from IModioUIModListViewInterface and IModioUIObjectListWidget to the
 * existing TileView implementation.
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioDefaultModTileView : public UTileView,
                                                 public IModioUIModListViewInterface,
                                                 public IModioUIObjectListWidget,
                                                 public IModioUIObjectSelector
{
	GENERATED_BODY()

protected:
	//~ Begin UWidget Interface
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface

	//~ Begin IModioUIModListViewInterface Interface
	virtual void NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting) override;
	virtual void NativeSetModSelectionByID(FModioModID ModID) override;
	//~ End IModioUIModListViewInterface Interface

	FModioObjectListOnObjectWidgetCreatedMulticast OnWidgetCreated;

	//~ Begin IModioUIObjectListWidget Interface
	virtual void NativeSetObjects(const TArray<UObject*>& InObjects) override;
	virtual TArray<UObject*> NativeGetObjects() override;
	virtual UObject* NativeGetObjectAt(int32 Index) const override;
	virtual void NativeAddObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) override;
	virtual void NativeRemoveObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) override;
	//~ End IModioUIObjectListWidget Interface

	//~ Begin IModioUIObjectSelector Interface
	virtual void SetValues_Implementation(const TArray<UObject*>& InValues) override;
	virtual int32 GetNumEntries_Implementation() override;
	virtual UObject* GetSingleSelectedValue_Implementation() override;
	virtual int32 GetSingleSelectionIndex_Implementation() override;
	virtual void ClearSelectedValues_Implementation() override;
	virtual void SetSingleSelectionByIndex_Implementation(int32 Index, bool bEmitSelectionEvent) override;
	virtual void SetSingleSelectionByValue_Implementation(UObject* Value, bool bEmitSelectionEvent) override;
	virtual void AddSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler) override;
	virtual void RemoveSelectionChangedHandler_Implementation(const FModioOnObjectSelectionChanged& Handler) override;
	virtual void SetSelectedStateForIndex_Implementation(int32 Index, bool bNewSelectionState,
														 bool bEmitSelectionEvent) override;
	virtual void SetSelectedStateForValue_Implementation(UObject* Value, bool bNewSelectionState,
														 bool bEmitSelectionEvent) override;
	virtual TArray<UObject*> GetSelectedValues_Implementation() override;
	virtual void SetMultiSelectionAllowed_Implementation(bool bMultiSelectionAllowed) override;
	virtual bool GetMultiSelectionAllowed_Implementation() override;
	virtual void SetListEntryWidgetClass_Implementation(TSubclassOf<UWidget> InNewEntryClass) override;
	virtual int32 GetIndexForValue_Implementation(UObject* Value) const override;
	virtual UUserWidget* GetWidgetForValue_Implementation(UObject* Value) const override;
	//~ End IModioUIObjectSelector Interface

	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass,
											   const TSharedRef<STableViewBase>& OwnerTable) override;

	void NotifySelectionChanged(UObject* SelectedItem);

	/**
	 * @brief Passes the bound value for which selection state has changed as `SelectedValue`
	 * @default_component_event FModioOnObjectSelectionChanged
	 */
	UPROPERTY()
	FModioOnObjectSelectionChangedMulticast OnSelectedValueChanged;

	FModioStackedBool EmitSelectionEvents {true};

	TWeakObjectPtr<UObject> PreviouslySelectedWidget;

	MODIO_WIDGET_PALETTE_OVERRIDE();
};
