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
#if WITH_EDITOR

	#include "CoreMinimal.h"
	#include "Factories/Factory.h"
	#include "AssetTypeActions_Base.h"
	#include "Types/ModioErrorCode.h"
	#include "Misc/Optional.h"
	#include "Types/ModioModProgressInfo.h"
	#include "Containers/Array.h"
	#include "Types/SlateEnums.h"
	
	#include "Misc/EngineVersionComparison.h"
#if UE_VERSION_NEWER_THAN(5, 2, 0)
	#include "Widgets/Views/SListView.h"
	#include "Widgets/Views/ITableRow.h"
#endif
	
	#include "ModioModProgressInfoUIFactory.generated.h"

class FModioModProgressInfoUIAssetActions : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override
	{
		return FText::FromString("Preview ModioModProgressInfo");
	}
	virtual FColor GetTypeColor() const override
	{
		return FColor(0, 70, 255);
	}
	virtual UClass* GetSupportedClass() const override;

	virtual uint32 GetCategories() override;
};

/**
 *
 */
UCLASS()
class UModioModProgressInfoUIFactory : public UFactory
{
	GENERATED_BODY()
protected:
	
public:
	UModioModProgressInfoUIFactory(const FObjectInitializer& ObjectInitializer);

	bool CanCreateNew() const override;

	UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context,
							  FFeedbackContext* Warn) override;

	FText GetDisplayName() const override;

	bool DoesSupportClass(UClass* Class) override;

	bool ConfigureProperties() override;
};

#endif
