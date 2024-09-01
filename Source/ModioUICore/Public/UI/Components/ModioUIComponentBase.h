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
											  public IModioFocusableWidget
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

	//~ Begin IModioFocusableWidget Interface
	virtual UWidget* NativeGetWidgetToFocus(EUINavigation NavigationType) const override;
	//~ End IModioFocusableWidget Interface

	//~ Begin UCommonActivatableWidget Interface
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~ End UCommonActivatableWidget Interface
};
