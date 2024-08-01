/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioDefaultScrollBox.h"

#include "Misc/EngineVersionComparison.h"

void UModioDefaultScrollBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateNavigationData(GetScrollOffset());
}

TSharedRef<SWidget> UModioDefaultScrollBox::RebuildWidget()
{
	UpdateNavigationData(GetScrollOffset());
	OnUserScrolled.RemoveDynamic(this, &UModioDefaultScrollBox::OnUserScrolledHandle);
	OnUserScrolled.AddDynamic(this, &UModioDefaultScrollBox::OnUserScrolledHandle);

	// TODO: This might be a bit hacky, but it's the only way I found to make sure that the scroll box GetScrollOffsetOfEnd() is correct (it is set up in SScrollBox sometime during the tick, not the first though). Revise this if you find a better way.
	ON_SCOPE_EXIT
	{
		if (NavigationDataTickHandle.IsValid())
		{
			FTSTicker::GetCoreTicker().RemoveTicker(NavigationDataTickHandle);
			NavigationDataTickHandle.Reset();
		}

		NavigationDataTickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateWeakLambda(this, [this](float Time) {
			UpdateNavigationData(GetScrollOffset());

			// Run only once
			return false;
		}), 0.25f); // This amount of time is somewhat arbitrary, but it seems to work well enough 
	};

	return Super::RebuildWidget();
}

void UModioDefaultScrollBox::OnUserScrolledHandle_Implementation(float CurrentOffset)
{
	UpdateNavigationData(CurrentOffset);
}

void UModioDefaultScrollBox::UpdateNavigationData_Implementation(int32 PendingScrollOffset)
{
	if (!IsValid(Navigation))
	{
		return;
	}

	if (IsDesignTime() || !IsValid(UserDefinedNavigation))
	{
		UserDefinedNavigation = NewObject<UWidgetNavigation>(this);
		UserDefinedNavigation->Up = Navigation->Up;
		UserDefinedNavigation->Down = Navigation->Down;
		UserDefinedNavigation->Left = Navigation->Left;
		UserDefinedNavigation->Right = Navigation->Right;
	}

	FCustomWidgetNavigationDelegate NavigationDelegate;
	NavigationDelegate.BindUFunction(this, "HandleCustomBoundaryNavigation");


	#if UE_VERSION_OLDER_THAN(5, 2, 0)
	if (Orientation == EOrientation::Orient_Vertical)
		#else
	if (GetOrientation() == EOrientation::Orient_Vertical)
		#endif
	{
		SetNavigationRuleCustomBoundary(EUINavigation::Down, NavigationDelegate);
		SetNavigationRuleCustomBoundary(EUINavigation::Up, NavigationDelegate);
	}
	#if UE_VERSION_OLDER_THAN(5, 2, 0)
	else if (Orientation == EOrientation::Orient_Horizontal)
		#else
	else if (GetOrientation() == EOrientation::Orient_Horizontal)
		#endif
	{
		SetNavigationRuleCustomBoundary(EUINavigation::Right, NavigationDelegate);
		SetNavigationRuleCustomBoundary(EUINavigation::Left, NavigationDelegate);
	}
		
	if (!bScrollByNavigationInput || IsDesignTime())
	{
		Navigation = UserDefinedNavigation;
		BuildNavigation();
	}
	else if (PendingScrollOffset != INDEX_NONE && (PendingScrollOffset <= 0 || PendingScrollOffset >= GetScrollOffsetOfEnd() || GetScrollOffsetOfEnd() == 0))
	{
		if (PendingScrollOffset <= 0)
		{
			Navigation->Up = UserDefinedNavigation->Up;
			Navigation->Left = UserDefinedNavigation->Left;
		}
		if (GetScrollOffsetOfEnd() == 0 || (GetScrollOffsetOfEnd() > 0 && PendingScrollOffset >= GetScrollOffsetOfEnd()))
		{
			Navigation->Right = UserDefinedNavigation->Right;
			Navigation->Down = UserDefinedNavigation->Down;
		}
		BuildNavigation();
	}
}

UWidget* UModioDefaultScrollBox::HandleCustomBoundaryNavigation_Implementation(EUINavigation InNavigation)
{
	const float NewOffset = [this, InNavigation]() {
		if (InNavigation == EUINavigation::Down || InNavigation == EUINavigation::Right)
		{
			return GetScrollOffset() + NavigationScrollOffsetStep;
		}
		if (InNavigation == EUINavigation::Up || InNavigation == EUINavigation::Left)
		{
			return GetScrollOffset() - NavigationScrollOffsetStep;
		}
		return static_cast<float>(INDEX_NONE);
	}();
	SetScrollOffset(NewOffset);
	UpdateNavigationData(NewOffset);
	return nullptr;
}