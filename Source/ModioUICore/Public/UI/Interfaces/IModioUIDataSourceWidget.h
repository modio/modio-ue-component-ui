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

#include "UObject/Interface.h"

#include "IModioUIDataSourceWidget.generated.h"

/**
 * @brief Interface indicating that the implementing widget can bind to a UObject-derived data source
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding", Meta = (DocGroup = "UI", Comment = "@brief Interface indicating that the implementing widget can bind to a UObject-derived data source"))
class MODIOUICORE_API UModioUIDataSourceWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIDataSourceWidget : public IInterface
{
	GENERATED_BODY()

protected:
	/** Flag to indicate if the data source has been set by the user */
	bool bRoutedSetDataSource = false;

	virtual void NativeSetDataSource(UObject* InDataSource) {}
	void SetDataSource_Implementation(UObject* InDataSource)
	{
		NativeSetDataSource(InDataSource);
	}
	virtual UObject* NativeGetDataSource() { return nullptr; }
	UObject* GetDataSource_Implementation()
	{
		return NativeGetDataSource();
	}

public:
	/**
	 * @brief Sets the associated data source/data binding object for the implementing widget
	 * @param InDataSource the object to use as a data source
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding")
	void SetDataSource(UObject* InDataSource);

	/**
	 * @brief Gets the associated data source/data binding object for the implementing widget
	 * @return the object used as a data source
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding")
	UObject* GetDataSource();
};