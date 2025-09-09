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

#include "CoreMinimal.h"
#include "Slate/SObjectTableRow.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UObject/ObjectPtr.h"
#include "Misc/EngineVersionComparison.h"

/**
 * Table row that automatically binds data to widgets implementing IModioUIDataSourceWidget.
 * Detects if entry widget implements the data source interface and sets the data source during initialization.
 */
template <typename ItemType>
class SModioDataSourceAwareTableRow : public SObjectTableRow<ItemType>
{
public:
	SModioDataSourceAwareTableRow() : SObjectTableRow<ItemType>() {}

protected:

	SModioDataSourceAwareTableRow(SModioDataSourceAwareTableRow<ItemType>&&) = delete;
	SModioDataSourceAwareTableRow& operator = (SModioDataSourceAwareTableRow<ItemType>&&) = delete;

	//~ Begin SObjectTableRow Interface
	virtual void InitializeObjectRow() override
	{
#if UE_VERSION_OLDER_THAN(5, 4, 0)
		TSharedRef<ITypedTableView<ItemType>> OwnerTable = this->OwnerTablePtr.Pin().ToSharedRef();
		if (const ItemType* MyItemPtr = this->GetItemForThis(OwnerTable))
#else
		TSharedPtr<ITypedTableView<ItemType>> OwnerTable = this->OwnerTablePtr.Pin();
		const TObjectPtrWrapTypeOf<ItemType>* MyItemPtr = OwnerTable ? this->GetItemForThis(OwnerTable.ToSharedRef()) : nullptr;
		if (MyItemPtr)
#endif
		{
			UUserWidget& ListEntryWidget = *this->WidgetObject;
			ItemType ListItemObject = *MyItemPtr;

			if (ListEntryWidget.Implements<UModioUIDataSourceWidget>())
			{
				IModioUIDataSourceWidget::Execute_SetDataSource(&ListEntryWidget, ListItemObject);
			}
		}

		SObjectTableRow<ItemType>::InitializeObjectRow();
	}
	//~ End SObjectTableRow Interface
};