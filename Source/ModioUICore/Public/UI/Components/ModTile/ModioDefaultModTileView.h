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
#include "UI/Components/ComponentHelpers.h"
#include "UI/Interfaces/IModioUIModListViewInterface.h"
#include "UI/Interfaces/IModioUIObjectListWidget.h"

#include "ModioDefaultModTileView.generated.h"

/**
 * @brief A default implementation of IModioUIModListViewInterface which displays mods as tiles in a grid. This widget
 * inherits from UTileView and forwards calls from IModioUIModListViewInterface and IModioUIObjectListWidget to the
 * existing TileView implementation.
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioDefaultModTileView : public UTileView,
												 public IModioUIModListViewInterface,
												 public IModioUIObjectListWidget
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
	virtual void NativeAddObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) override;
	virtual void NativeRemoveObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) override;
	//~ End IModioUIObjectListWidget Interface

	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass,
											   const TSharedRef<STableViewBase>& OwnerTable) override;

	MODIO_WIDGET_PALETTE_OVERRIDE();
};