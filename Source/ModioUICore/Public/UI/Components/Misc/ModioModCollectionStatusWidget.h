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
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/EventHandlers/IModioUISubscriptionsChangedReceiver.h"
#include "UI/EventHandlers/IModioUICollectionFollowStateChangedReceiver.h"

#include "ModioModCollectionStatusWidget.generated.h"

UENUM(BlueprintType)
enum class EModioModCollectionState : uint8
{
	// Not followed, subscriptions irrelevant
	NotFollowed,
	// Followed, but not all mods are subscribed
	IncompleteSubscriptions,
	// Followed, but none are subscribed
	Followed,
	// Followed and Subscribed to all mods
	Subscribed
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFollowStateChanged, EModioModCollectionState, NewState);

/**
 * @default_impl_for Mod Collection Status Widget
 * @brief This is a default implementation of the Mod Collection Status component which is intended to represent a mod
 * collection's follow state for the currently authenticated user.
 * @component_display_name Mod Collection Status Widget
 */
UCLASS()
class MODIOUICORE_API UModioModCollectionStatusWidget
	: public UModioUIComponentBase,
	  public IModioUISubscriptionsChangedReceiver,
	  public IModioUICollectionFollowStateChangedReceiver
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Full information for the mod collection this widget represents, based on its data source
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|CollectionState|Data Binding",
		meta = (BlueprintProtected))
	FModioModCollectionInfo GetModCollectionInfo();

	/**
	 * @brief Broadcasts when the mod collection's follow state is updated
	 */
	UPROPERTY(BlueprintAssignable, Category = "mod.io|UI|CollectionState|Events", meta = (BlueprintProtected))
	FOnFollowStateChanged OnCollectionFollowStateChangedEvent;

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|CollectionState|Data Binding", meta = (BlueprintProtected))
	EModioModCollectionState CurrentCollectionFollowState = EModioModCollectionState::NotFollowed;

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|CollectionState|Data Binding")
	void SetModCollectionFollowState(EModioModCollectionState NewState);

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativeSetDataSource(UObject* InDataSource) override;
	//~ End IModioUIDataSourceWidget Interface

	//~ Begin IModioUISubscriptionsChangedReceiver Interface
	virtual void NativeOnSubscriptionsChanged(FModioModID ModID, bool bNewSubscriptionState) override;
	//~ End IModioUISubscriptionsChangedReceiver Interface

	//~ Begin IModioUICollectionFollowStateChangedReceiver Interface
	virtual void
		NativeOnCollectionFollowStateChanged(FModioModCollectionID ModCollectionID, bool bNewFollowState) override;
	//~ End IModioUICollectionFollowStateChangedReceiver Interface
};