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
 *
 */
UCLASS(meta = (ModioWidget))
class MODIOUICORE_API UModioObjectWrapBox : public UWrapBox, public IModioUIObjectListWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeSetObjects(const TArray<UObject*>& InObjects) override;

	virtual TArray<UObject*> NativeGetObjects() override;

	UPROPERTY(Transient)
	TArray<UObject*> BoundObjects;

	UPROPERTY(EditAnywhere, Category = "mod.io|UI|Data Binding|Object Wrap Box",
			  meta = (MustImplement = "ModioUIDataSourceWidget", BlueprintProtected))
	TSubclassOf<UWidget> WidgetClass;

	UPROPERTY()
	FModioObjectListOnObjectWidgetCreatedMulticast OnWidgetCreated;

	//~ Begin UWidget Interface
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface

	virtual void NativeAddObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) override;

	virtual void NativeRemoveObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) override;

	MODIO_WIDGET_PALETTE_OVERRIDE();
};
