/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/ModioUIComponentBase.h"

void UModioUIComponentBase::NativeSetDataSource(UObject* InDataSource)
{
	DataSource = InDataSource;
}

UObject* UModioUIComponentBase::NativeGetDataSource()
{
	return DataSource;
}

void UModioUIComponentBase::NativeActivate()
{
	ActivateWidget();
}

void UModioUIComponentBase::NativeDeactivate()
{
	DeactivateWidget();
}

void UModioUIComponentBase::NativeAddActivationChangedHandler(const FModioOnActivationChanged& Handler)
{
	OnModioActivationChanged.AddUnique(Handler);
}

void UModioUIComponentBase::NativeRemoveActivationChangedHandler(const FModioOnActivationChanged& Handler)
{
	OnModioActivationChanged.Remove(Handler);
}

UWidget* UModioUIComponentBase::NativeGetWidgetToFocus(EUINavigation NavigationType) const
{
	UE_LOG(ModioUICore, Verbose, TEXT("Widget '%s' does not have a focus target set. Please override GetWidgetToFocus in Blueprint or GetWidgetToFocusNative in C++ to set a focus target."), *GetName());
	return nullptr;
}

void UModioUIComponentBase::NativeAddFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler)
{
	OnModioFocusPathChanged.Add(Handler);
}

void UModioUIComponentBase::NativeRemoveFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler)
{
	OnModioFocusPathChanged.Remove(Handler);
}

void UModioUIComponentBase::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	OnModioFocusPathChanged.Broadcast(this, InFocusEvent, true);
}

void UModioUIComponentBase::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	OnModioFocusPathChanged.Broadcast(this, InFocusEvent, false);
}

void UModioUIComponentBase::NativeOnActivated()
{
	Super::NativeOnActivated();
	OnModioActivationChanged.Broadcast(this, true);
}

void UModioUIComponentBase::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	OnModioActivationChanged.Broadcast(this, false);
}

UWidget* UModioUIComponentBase::NativeGetDesiredFocusTarget() const
{
	// "Next" navigation seems like a good default for most widgets, when navigation direction is not defined
	if (UWidget* WidgetToFocus = Execute_GetWidgetToFocus(this, EUINavigation::Next))
	{
		return WidgetToFocus;
	}
	return Super::NativeGetDesiredFocusTarget();
}
