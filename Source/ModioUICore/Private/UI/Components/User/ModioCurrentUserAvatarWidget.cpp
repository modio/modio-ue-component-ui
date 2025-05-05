/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/User/ModioCurrentUserAvatarWidget.h"
#include "UI/Interfaces/IModioUIImageDisplayWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioCurrentUserAvatarWidget)

TScriptInterface<IModioUIImageDisplayWidget> UModioCurrentUserAvatarWidget::GetAvatarImageWidget_Implementation() const
{
	return nullptr;
}

void UModioCurrentUserAvatarWidget::OnImageLoadCompleted(UObject* Context, bool bLoadSuccess)
{
	IModioUIHasLoadStateWidget::Execute_SetLoadState(this, bLoadSuccess ? EModioUILoadState::Loaded
																		: EModioUILoadState::Errored);
}

void UModioCurrentUserAvatarWidget::TransitionToLoadState(EModioUILoadState NewState)
{
	if (UWidget* ImageWidget = ModioUI::GetInterfaceWidgetChecked(GetAvatarImageWidget()))
	{
		switch (NewState)
		{
			case EModioUILoadState::NotLoaded:
				IModioUIImageDisplayWidget::Execute_SetBrushDirectly(ImageWidget, UnloadedBrush);
				break;
			case EModioUILoadState::Errored:
				IModioUIImageDisplayWidget::Execute_SetBrushDirectly(ImageWidget, ErroredBrush);
				break;
			case EModioUILoadState::Loading:
				IModioUIImageDisplayWidget::Execute_SetBrushDirectly(ImageWidget, LoadingBrush);
				break;
			case EModioUILoadState::Loaded:
				// Loaded state transition is triggered when the avatar image widget has finished loading its own
				// internal texture and is displaying it, so do nothing
				break;
		}
	}
}

void UModioCurrentUserAvatarWidget::NativeOnUserAvatarDownloadCompleted(FModioErrorCode ErrorCode,
																		TOptional<FModioImageWrapper> Image)
{
	IModioUIUserAvatarDownloadCompletedReceiver::NativeOnUserAvatarDownloadCompleted(ErrorCode, Image);
	if (ErrorCode)
	{
		IModioUIHasLoadStateWidget::Execute_SetLoadState(this, EModioUILoadState::Errored);
	}
	else
	{
		if (UWidget* ImageWidget = ModioUI::GetInterfaceWidgetChecked(GetAvatarImageWidget()))
		{
			IModioUIHasLoadStateWidget::Execute_SetLoadState(this, EModioUILoadState::Loading);

			IModioUIImageDisplayWidget::Execute_BeginLoadImageFromFile(ImageWidget,
																	   Image.GetValue());
		}
	}
}

void UModioCurrentUserAvatarWidget::NativeUserChanged(TOptional<FModioUser> NewUser)
{
	IModioUIUserChangedReceiver::NativeUserChanged(NewUser);
	if (!NewUser)
	{
		IModioUIHasLoadStateWidget::Execute_SetLoadState(this, EModioUILoadState::NotLoaded);
	}
	else
	{
		if (UModioUISubsystem* UISubsystem = GEngine->GetEngineSubsystem<UModioUISubsystem>())
		{
			UISubsystem->RequestUserAvatar();
		}
	}
}

void UModioCurrentUserAvatarWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	IModioUIUserChangedReceiver::Register<UModioCurrentUserAvatarWidget>();
	IModioUIUserAvatarDownloadCompletedReceiver::Register<UModioCurrentUserAvatarWidget>();
	if (UWidget* ImageWidget = ModioUI::GetInterfaceWidgetChecked(GetAvatarImageWidget()))
	{
		FModioImageDisplayOnLoadStateChanged OnImageLoaded;
		OnImageLoaded.BindUFunction(this, FName("OnImageLoadCompleted"));
		IModioUIImageDisplayWidget::Execute_AddImageLoadEventHandler(ImageWidget, OnImageLoaded);
	}
}

void UModioCurrentUserAvatarWidget::NativeSetLoadState(EModioUILoadState NewState)
{
	if (NewState != CurrentLoadState)
	{
		CurrentLoadState = NewState;
		TransitionToLoadState(NewState);
		OnLoadStateChanged.Broadcast(this, NewState);
	}
}

EModioUILoadState UModioCurrentUserAvatarWidget::NativeGetLoadState() const
{
	return CurrentLoadState;
}

void UModioCurrentUserAvatarWidget::NativeAddLoadStateChangedHandler(const FModioOnLoadStateWidgetStateChanged& Handler)
{
	OnLoadStateChanged.AddUnique(Handler);
}

void UModioCurrentUserAvatarWidget::NativeRemoveLoadStateChangedHandler(
	const FModioOnLoadStateWidgetStateChanged& Handler)
{
	OnLoadStateChanged.Remove(Handler);
}
