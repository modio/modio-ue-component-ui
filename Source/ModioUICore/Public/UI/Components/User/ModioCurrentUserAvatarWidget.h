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
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/Components/User/ModioUserDisplay.h"
#include "UI/EventHandlers/IModioUIUserAvatarDownloadCompletedReceiver.h"
#include "UI/EventHandlers/IModioUIUserChangedReceiver.h"
#include "UI/Interfaces/IModioUIHasLoadStateWidget.h"

#include "ModioCurrentUserAvatarWidget.generated.h"

/**
 * @default_impl_for Current User Avatar
 * @brief Widget that displays the current user's avatar
 * @component_display_name Current User Avatar
 */
UCLASS()
class MODIOUICORE_API UModioCurrentUserAvatarWidget : public UModioUserDisplay,
													  public IModioUIUserAvatarDownloadCompletedReceiver,
													  public IModioUIUserChangedReceiver,
													  public IModioUIHasLoadStateWidget
{
	GENERATED_BODY()

protected:
	/**
	 * @brief The brush to use when the avatar is not loaded
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Style|UserAvatar", meta = (BlueprintProtected))
	FSlateBrush UnloadedBrush;

	/**
	 * @brief The brush to use when the avatar is errored (failed to load)
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Style|UserAvatar", meta = (BlueprintProtected))
	FSlateBrush ErroredBrush;

	/**
	 * @brief The brush to use when the avatar is loading
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Style|UserAvatar", meta = (BlueprintProtected))
	FSlateBrush LoadingBrush;

	/**
	 * @brief Get the image widget that displays the avatar
	 * @return The image widget
	 * @default_component_widget_getter
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|UserAvatar|Widget Getters",
			  meta = (BlueprintProtected))
	TScriptInterface<IModioUIImageDisplayWidget> GetAvatarImageWidget() const;

	FModioOnLoadStateWidgetStateChangedMulticast OnLoadStateChanged;
	EModioUILoadState CurrentLoadState = EModioUILoadState::NotLoaded;

	void OnImageLoadCompleted(UObject* Context, bool bLoadSuccess);
	virtual void TransitionToLoadState(EModioUILoadState NewState);

	//~ Begin IModioUIUserAvatarDownloadCompletedReceiver Interface
	virtual void NativeOnUserAvatarDownloadCompleted(FModioErrorCode ErrorCode,
													 TOptional<FModioImageWrapper> Image) override;
	//~ End IModioUIUserAvatarDownloadCompletedReceiver Interface

	//~ Begin IModioUIUserChangedReceiver Interface
	virtual void NativeUserChanged(TOptional<FModioUser> NewUser) override;
	//~ End IModioUIUserChangedReceiver Interface

	//~ Begin UUserWidget Interface
	virtual void NativePreConstruct() override;
	//~ End UUserWidget Interface

	//~ Begin IModioUIHasLoadStateWidget Interface
	virtual void NativeSetLoadState(EModioUILoadState NewState) override;
	virtual EModioUILoadState NativeGetLoadState() const override;
	virtual void NativeAddLoadStateChangedHandler(const FModioOnLoadStateWidgetStateChanged& Handler) override;
	virtual void NativeRemoveLoadStateChangedHandler(const FModioOnLoadStateWidgetStateChanged& Handler) override;
	//~ End IModioUIHasLoadStateWidget Interface
};
