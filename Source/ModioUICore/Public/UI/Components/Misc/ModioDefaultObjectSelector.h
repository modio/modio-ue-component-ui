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
#include "UI/Interfaces/IModioUIObjectListWidget.h"
#include "UI/Interfaces/IModioUIObjectSelector.h"

#include "ModioDefaultObjectSelector.generated.h"

/**
 *
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioDefaultObjectSelector : public UListView,
													public IModioUIObjectSelector,
													public IModioUIObjectListWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Category = ListEntries))
	TSubclassOf<UWidget> MultipleSelectionListItemClass;

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|DefaultObjectSelector")
	UUserWidget* GetEntryWidgetFromItem(const UObject* Item) const;

protected:
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
	//~ End IModioUIObjectSelector Interface

	void NotifySelectionChanged(UObject* SelectedItem);

	//~ Begin IModioUIObjectListWidget Interface
	FModioObjectListOnObjectWidgetCreatedMulticast OnWidgetCreated;
	virtual void NativeSetObjects(const TArray<UObject*>& InObjects) override;
	virtual TArray<UObject*> NativeGetObjects() override;
	virtual void NativeAddObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) override;
	virtual void NativeRemoveObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) override;
	//~ End IModioUIObjectListWidget Interface
};