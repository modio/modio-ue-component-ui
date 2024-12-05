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

#include "IModioUIObjectListWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FModioObjectListOnObjectWidgetCreated, UWidget*,ObjectWidget, UObject*,Object);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioObjectListOnObjectWidgetCreatedMulticast, UWidget*, ObjectWidget, UObject*, Object);

/**
 * @brief Interface indicating that the implementing widget can bind to a list of UObject-derived items
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Data Binding", Meta = (DocGroup = "UI", Comment = "@brief Interface indicating that the implementing widget can bind to a list of UObject-derived items"))
class MODIOUICORE_API UModioUIObjectListWidget : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IModioUIObjectListWidget : public IInterface
{
	GENERATED_BODY()

protected:
	bool bRoutedSetObjects = false;

	virtual void NativeSetObjects(const TArray<UObject*>& InObjects) {}
	void SetObjects_Implementation(const TArray<UObject*>& InObjects)
	{
		NativeSetObjects(InObjects);
	}
	virtual TArray<UObject*> NativeGetObjects() {return {};}
	TArray<UObject*> GetObjects_Implementation()
	{
		return NativeGetObjects();
	}
	virtual UObject* NativeGetObjectAt(int32 Index) const {return nullptr;}
	UObject* GetObjectAt_Implementation(int32 Index) const
	{
		return NativeGetObjectAt(Index);
	}
	virtual void NativeAddObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) {}
	void AddObjectWidgetCreatedHandler_Implementation(const FModioObjectListOnObjectWidgetCreated& Handler)
	{
		NativeAddObjectWidgetCreatedHandler(Handler);
	}
	virtual void NativeRemoveObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler) {}
	void RemoveObjectWidgetCreatedHandler_Implementation(const FModioObjectListOnObjectWidgetCreated& Handler)
	{
		NativeRemoveObjectWidgetCreatedHandler(Handler);
	}

	virtual UWidget* NativeGetFirstEntryWidget() const
	{
		return nullptr;
	}
	UWidget* GetFirstEntryWidget_Implementation() const
	{
		return NativeGetFirstEntryWidget();
	}

public:
	/**
	 * @brief Sets the associated data source/data binding objects for the implementing widget
	 * @param InObjects the objects to use as a data source
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding")
	void SetObjects(const TArray<UObject*>& InObjects);

	/**
	 * @brief Retrieves the associated data source objects for the implementing widget
	 * @return Array of UObject-derived objects used as a data source
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding")
	TArray<UObject*> GetObjects();

	/**
	 * @brief Retrieves the object at a specific index in the data source
	 * @param Index The index of the object to retrieve
	 * @return The object at the specified index
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding")
	UObject* GetObjectAt(int32 Index) const;

	/**
	 * @brief Registers a delegate to receive callbacks when a new object widget is created
	 * @param Handler Delegate to invoke when a new object widget is created
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Data Binding")
	void AddObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler);

	/**
	 * @brief Unregisters a delegate from receiving object widget creation callbacks
	 * @param Handler Delegate to remove
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Events|Data Binding")
	void RemoveObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler);

	/**
	 * @brief Gets the first widget entry in the contained list widget, or nullptr if the list is empty.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Data Binding")
	UWidget* GetFirstEntryWidget();
};
