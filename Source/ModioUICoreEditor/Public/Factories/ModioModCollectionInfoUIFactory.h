/*
 *  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once
#if WITH_EDITOR

	#include "AssetTypeActions_Base.h"
	#include "Containers/Array.h"
	#include "CoreMinimal.h"
	#include "Factories/Factory.h"
	#include "Misc/Optional.h"
	#include "Types/ModioErrorCode.h"
	#include "Types/ModioModCollection.h"
	#include "Types/SlateEnums.h"

	#include "Misc/EngineVersionComparison.h"
	#if UE_VERSION_NEWER_THAN(5, 2, 0)
		#include "Widgets/Views/ITableRow.h"
		#include "Widgets/Views/SListView.h"
	#endif

	#include "ModioModCollectionInfoUIFactory.generated.h"

class FModioModCollectionInfoUIAssetActions : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override
	{
		return FText::FromString("Preview ModioModCollectionInfoUI");
	}
	virtual FColor GetTypeColor() const override
	{
		return FColor(149, 70, 255);
	}
	virtual UClass* GetSupportedClass() const override;

	virtual uint32 GetCategories() override;
};

/**
 *
 */
UCLASS()
class UModioModCollectionInfoUIFactory : public UFactory
{
	GENERATED_BODY()
protected:
	volatile bool bOperationInProgress = true;
	bool bRequiresSDKShutdown = false;
	TArray<TSharedPtr<FModioModCollectionInfo>> ModCollectionInfos;
	TSharedPtr<SWindow> ImportWindow;
	TSharedPtr<SListView<TSharedPtr<FModioModCollectionInfo>>> AvailableModCollectionInfoList;
	TSharedPtr<FModioModCollectionInfo> SelectedItem;
	bool bConfirmClicked = false;

public:
	UModioModCollectionInfoUIFactory(const FObjectInitializer& ObjectInitializer);

	bool CanCreateNew() const override;

	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context,
							  FFeedbackContext* Warn) override;

	FText GetDisplayName() const override;

	bool DoesSupportClass(UClass* Class) override;

	bool ConfigureProperties() override;

	void OnModCollectionListRetrieved(FModioErrorCode ec, TOptional<FModioModCollectionInfoList> ModCollectionList);
	FReply OnConfirmClicked();
	EVisibility IsListVisible() const;
	bool IsModCollectionInfoSelected() const;
	void OnSelectionChanged(TSharedPtr<FModioModCollectionInfo> Item, ESelectInfo::Type);
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FModioModCollectionInfo> Item,
											   const TSharedRef<class STableViewBase>& OwnerTable);
};

#endif
