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

#include "Components/WrapBox.h"
#include "CoreMinimal.h"
#include "UI/Components/ComponentHelpers.h"
#include "UI/Interfaces/IModioUIObjectListWidget.h"

#include "ModioObjectWrapBox.generated.h"

/**
 * @default_impl_for Object List
 * @brief The Object Wrap Box is a default implementation of the Object List component. This implementation derives from
 * `UWrapBox` and defers to it for visualizing a set of objects. It does this by instantiating a specific
 * `IModioUIDataSourceWidget`-implementing widget for each object in the set, using that interface to bind the widget to
 * the object in question, and then adding the widget as a child to the wrap box implementation, which handles the
 * positioning and layout of those widgets.
 * @component_display_name Object Wrap Box
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioObjectWrapBox : public UWrapBox, public IModioUIObjectListWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	TArray<UObject*> BoundObjects;

	/**
	 * @brief The class of the widget to instantiate for each object in the list. Must implement ModioUIDataSourceWidget
	 */
	UPROPERTY(EditAnywhere, Category = "mod.io|UI|Data Binding|Object Wrap Box",
			  meta = (MustImplement = "ModioUIDataSourceWidget", BlueprintProtected))
	TSubclassOf<UWidget> WidgetClass;

	/**
	 * @brief Passes the newly created widget as `ObjectWidget`, and the UObject bound to that widget as `Object`
	 * @default_component_event FModioObjectListOnObjectWidgetCreated
	 */
	UPROPERTY()
	FModioObjectListOnObjectWidgetCreatedMulticast OnWidgetCreated;

	//~ Begin UWidget Interface
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface

	//~ Begin IModioUIObjectListWidget Interface
	virtual void NativeSetObjects(const TArray<UObject*>& InObjects) override;
	virtual TArray<UObject*> NativeGetObjects() override;
	virtual UObject* NativeGetObjectAt(int32 Index) const override;
	virtual void NativeAddObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) override;
	virtual void NativeRemoveObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) override;
	//~ End IModioUIObjectListWidget Interface

	MODIO_WIDGET_PALETTE_OVERRIDE();
};
