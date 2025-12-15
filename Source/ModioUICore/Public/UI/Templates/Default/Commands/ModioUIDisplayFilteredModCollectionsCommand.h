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

#include "CoreMinimal.h"
#include "UI/Interfaces/IModioModFilterUIDetails.h"
#include "UI/Interfaces/IModioUICommandMenu.h"
#include "Brushes/SlateNoResource.h"
#include "Core/ModioFilterParamsUI.h"
#include "ModioUISubsystem.h"
#include "UObject/Object.h"
#include "ModioUIDisplayFilteredModCollectionsCommand.generated.h"

/**
 * @nodoc
 */
UCLASS()
class MODIOUICORE_API UModioUIDisplayFilteredModCollectionsCommand
	: public UObject,
	  public IModioUICommandObject,
	  public IModioModFilterUIDetails
{
	GENERATED_BODY()

protected:
	static inline const FString CommandID{TEXT("DisplayFilteredModCollections")};

	UPROPERTY(Transient, BlueprintReadOnly, Category = "mod.io|UI|Commands|Display Filtered Mod Collections",
		meta = (BlueprintProtected))
	TObjectPtr<UObject> DataSource;

	UPROPERTY(meta = (BlueprintProtected))
	FModioFilterParams UnderlyingParams;

	UPROPERTY(meta = (BlueprintProtected))
	FName DisplayStringKey;

	virtual void SetDataSource_Implementation(UObject* InDataSource) override
	{
		DataSource = InDataSource;
	}

	virtual FText GetCommandDisplayString_Implementation() override
	{
		if (!DisplayStringKey.IsNone())
		{
			bool bWasParamsResolved = false;
			return UModioUICommandLibrary::GetCommandText(CommandID, DisplayStringKey, bWasParamsResolved);
		}

		return FText::GetEmpty();
	}

	virtual FSlateBrush GetCommandBrush_Implementation() override
	{
		if (!DisplayStringKey.IsNone())
		{
			bool bWasParamsResolved = false;
			return UModioUICommandLibrary::GetCommandBrush(CommandID, DisplayStringKey, bWasParamsResolved);
		}
		return FSlateNoResource{};
	}

	virtual void Execute_Implementation(UObject* Context) override
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			UISubsystem->RequestListModCollections(UnderlyingParams, DisplayStringKey.ToString());
		}
	}

	virtual bool CanExecute_Implementation(UObject* Context) override
	{
		return true;
	}

	virtual FModioFilterParams GetFilterParams_Implementation() override
	{
		return UnderlyingParams;
	}

	virtual bool GetFilterName_Implementation(FText& Name) override
	{
		Name = GetCommandDisplayString();
		return true;
	}

public:
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Commands|Display Filtered Mod Collections")
	void SetFilterParams(const FModioFilterParams& FilterParams)
	{
		UnderlyingParams = FilterParams;
	}

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Commands|Display Filtered Mod Collections")
	void SetDisplayStringKey(const FName& InDisplayStringKey)
	{
		DisplayStringKey = InDisplayStringKey;
	}
};