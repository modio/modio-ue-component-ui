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

#include "Blueprint/UserWidget.h"
#include "CommonActivatableWidget.h"
#include "CoreMinimal.h"
#include "UI/Components/ComponentHelpers.h"
#include "UI/Interfaces/IModioActivatableWidget.h"
#include "UI/Interfaces/IModioFocusableWidget.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"

#include "UObject/ObjectPtr.h"

#include "ModioUIComponentBase.generated.h"

/**
 * @brief Base class for all mod.io UI components that derive from UUserWidget
 */
UCLASS(Abstract, meta = (ModioWidget))
class MODIOUICORE_API UModioUIComponentBase : public UCommonActivatableWidget,
											  public IModioUIDataSourceWidget,
											  public IModioFocusableWidget,
											  public IModioActivatableWidget
{
	GENERATED_BODY()

protected:
	IMPLEMENT_MODIO_WIDGET_VALIDATION();

	MODIO_WIDGET_PALETTE_OVERRIDE();

	TOptional<FLinearColor> OriginalColorAndOpacity;

	/**
	 * @brief Cached object to use as the source for data binding. Populated by SetDataSource
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Data Binding", Transient,
			  meta = (BlueprintProtected))
	TObjectPtr<UObject> DataSource;

	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativeSetDataSource(UObject* InDataSource) override;
	virtual UObject* NativeGetDataSource() override;
	//~ End IModioUIDataSourceWidget Interface

	//~ Begin IModioActivatableWidget Interface
	virtual void NativeActivate() override;
	virtual void NativeDeactivate() override;
	virtual void NativeAddActivationChangedHandler(const FModioOnActivationChanged& Handler) override;
	virtual void NativeRemoveActivationChangedHandler(const FModioOnActivationChanged& Handler) override;
	//~ End IModioActivatableWidget Interface

	/**
	 * @brief Passes `this` as ActivationContext, `bIsActivated` as the new activation state
	 * @default_component_event FModioOnActivationChanged
	 */
	UPROPERTY()
	FModioOnActivationChangedMulticast OnModioActivationChanged;

	//~ Begin IModioFocusableWidget Interface
	virtual UWidget* NativeGetWidgetToFocus(EUINavigation NavigationType) const override;
	virtual void NativeAddFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler) override;
	virtual void NativeRemoveFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler) override;
	//~ End IModioFocusableWidget Interface

	//~ Begin UUserWidget Interface
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	//~ End UUserWidget Interface

	/**
	 * @brief Passes `this` as FocusContext, `InFocusEvent` as the focus event, `bIsFocused` as the new focus state
	 * @default_component_event FModioOnFocusPathChanged
	 */
	UPROPERTY()
	FModioOnFocusPathChangedMulticast OnModioFocusPathChanged;

	//~ Begin UCommonActivatableWidget Interface
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface
};
