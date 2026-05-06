// Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>

#pragma once

#include "Delegates/Delegate.h"
#include "UObject/Interface.h"
#include "Types/ModioUser.h"
#include "Types/ModioUserList.h"

#include "IUserFollowingListProvider.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUserFollowingListChangedHandler, FModioUserList, FollowingList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUserFollowingListChangedMulticastDelegate, FModioUserList, FollowingList);

/**
 * Interface for querying or setting the following list for the current user
 */
UINTERFACE(BlueprintType)
class MODIOUICORE_API UUserFollowingListProvider : public UInterface
{
	GENERATED_BODY()
};

class MODIOUICORE_API IUserFollowingListProvider : public IInterface
{
	GENERATED_BODY()

protected:
	virtual void NativeQueryUserFollowingList()
	{
		return;
	}

	virtual void NativeRequestUserFollowingListChange(FModioUserList NewFollowingList)
	{}

	virtual void NativeRequestAddFollowedUser(FModioUserID NewFollowedUser)
	{}

	virtual void NativeRequestRemoveFollowedUser(FModioUserID UnfollowedUser)
	{}

	void QueryUserFollowingList_Implementation()
	{
		return;
	}

	void RequestUserFollowingListChange_Implementation(FModioUserList NewFollowingList)
	{
		NativeRequestUserFollowingListChange(NewFollowingList);
	}

	void RequestAddFollowedUser_Implementation(FModioUserID NewFollowedUser)
	{
		NativeRequestAddFollowedUser(NewFollowedUser);
	}

	void RequestRemoveFollowedUser_Implementation(FModioUserID NewUnfollowedUser)
	{
		NativeRequestRemoveFollowedUser(NewUnfollowedUser);
	}

public:
	/**
	 * Queries the current list of followed users, the result of which is accessed by being subscribed to 
	 * EModioUIUserFollowingEventType::ListUserFollowing on ModioUISubsystem
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UGC|User Following List Provider")
	void QueryUserFollowingList();

	/**
	 * Requests that the full list of followed users be changed
	 * @param NewFollowingList The new list of users to replace the current list of followers with.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UGC|User Following List Provider")
	void RequestUserFollowingListChange(FModioUserList NewFollowingList);

	/**
	 * Requests the adding of a single user to the current followed users list
	 * @param NewFollowedUser the user to add to the list of currently followed users
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UGC|User Following List Provider")
	void RequestAddFollowedUser(FModioUserID NewFollowedUser);

	/**
	 * Requests the removal of a single user to the current followed users list
	 * @param NewUnfollowedUser the user to remove from the list of currently followed users
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent,Category = "UGC|User Following List Provider")
	void RequestRemoveFollowedUser(FModioUserID NewUnfollowedUser);
};
