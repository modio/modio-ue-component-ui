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

#include "Components/ListView.h"
#include "Core/ModioStackedBool.h"
#include "CoreMinimal.h"
#include "UI/Components/ComponentHelpers.h"
#include "UI/Interfaces/IModioFocusableWidget.h"
#include "UI/Interfaces/IModioUIObjectListWidget.h"
#include "UI/Interfaces/IModioUIObjectSelector.h"
#include "UI/Interfaces/IModioScrollableWidget.h"

#include "ModioDefaultObjectSelector.generated.h"

/**
 * @default_impl_for Object Selector
 * @brief The Default Object Selector is a default implementation of the Object Selector component. This implementation
 * derives from `UListView` and defers to it for displaying a set of objects and allowing the user to select one or more
 * of them based on the widget's configuration. It requires the widgets that represent each entry in the list to
 * implement `IModioUIDataSourceWidget` and `IModioUIClickableWidget` in addition to the `IUserObjectListEntry` interface
 * requirement imposed by the underlying List View.
 * @component_display_name Object Selector
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioDefaultObjectSelector : public UListView,
													public IModioUIObjectSelector,
													public IModioUIObjectListWidget,
													public IModioFocusableWidget,
													public IModioScrollableWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Category = ListEntries))
	TSubclassOf<UWidget> MultipleSelectionListItemClass;

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|DefaultObjectSelector")
	UUserWidget* GetEntryWidgetFromItem(const UObject* Item) const;

protected:
	/**
	 * @brief Passes the bound value for which selection state has changed as `SelectedValue`
	 * @default_component_event FModioOnObjectSelectionChanged
	 */
	UPROPERTY()
	FModioOnObjectSelectionChangedMulticast OnSelectedValueChanged;

	FModioStackedBool EmitSelectionEvents {true};

	MODIO_WIDGET_PALETTE_OVERRIDE();

	//~ Begin UWidget Interface
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface

	TWeakObjectPtr<UObject> PreviouslySelectedWidget;

	virtual void NativeOnEntryWidgetClicked(UObject* Widget);

	UFUNCTION()
	void OnEntryWidgetClicked(UObject* Widget);

	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass,
													   const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual void OnSelectionChangedInternal(UObject* FirstSelectedItem) override;
	virtual void OnItemClickedInternal(UObject* Item) override;

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

	//~ Begin IModioScrollableWidget Interface
	virtual void ScrollToTop_Implementation() override;
	virtual void ScrollToBottom_Implementation() override;
	virtual void SetScrollOffset_Implementation(float Offset) override;
	virtual float GetScrollOffset_Implementation() const override;
	//~ End IModioScrollableWidget Interface

	void NotifySelectionChanged(UObject* SelectedItem);

	//~ Begin IModioUIObjectListWidget Interface
	
	/**
	 * @brief Passes the newly created widget as `ObjectWidget`, and the UObject bound to that widget as `Object`
	 * @default_component_event FModioObjectListOnObjectWidgetCreated
	 */
	UPROPERTY()
	FModioObjectListOnObjectWidgetCreatedMulticast OnWidgetCreated;

	virtual void NativeSetObjects(const TArray<UObject*>& InObjects) override;
	virtual TArray<UObject*> NativeGetObjects() override;
	virtual UObject* NativeGetObjectAt(int32 Index) const override;
	virtual void NativeAddObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) override;
	virtual void NativeRemoveObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) override;
	//~ End IModioUIObjectListWidget Interface


	//~ Begin IModioFocusableWidget Interface
	virtual UWidget* NativeGetWidgetToFocus(EUINavigation NavigationType) const override;
	//~ End IModioFocusableWidget Interface
};
