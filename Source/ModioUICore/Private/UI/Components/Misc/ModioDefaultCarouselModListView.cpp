/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioDefaultCarouselModListView.h"
#include "Components/SizeBox.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUISelectableWidget.h"
#include "Widgets/Views/IItemsSource.h"
#include "Widgets/Views/SListView.h"

template<typename ItemType>
class SModioListView : public SListView<ItemType>
{
public:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
						  FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
						  bool bParentEnabled) const override
	{
		// Store the frame when the widget is painted so we can detect when it's offscreen and reset it if needed.
		LastPaintedFrame = GFrameCounter;
		return SListView<ItemType>::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId,
											InWidgetStyle, bParentEnabled);
	}

	mutable uint64 LastPaintedFrame = 0;
};

template<typename ItemType>
class FCarouselLoopingItemSource : public UE::Slate::ItemsSource::IItemsSource<ItemType>
{
public:
	explicit FCarouselLoopingItemSource(UModioDefaultCarouselModListView* InParentWidget) : ParentWidget(InParentWidget)
	{}

	virtual const TArrayView<const ItemType> GetItems() const override
	{
		if (!ParentWidget.IsValid())
		{
			return TArrayView<const ItemType>();
		}

		int32 TilesToShow = ParentWidget->GetListItems().IsEmpty()
									  ? 0
									  : FMath::Max(ParentWidget->GetMinimumTiles(), ParentWidget->GetListItems().Num());

		const bool bShouldShowViewMoreButton = TilesToShow > 0 && ParentWidget->ShouldHaveViewMoreButton();

		const bool bNeedsRefreshCount = ParentWidget->ItemWrappers.Num() != TilesToShow;
		if (bNeedsRefreshCount)
		{
			ParentWidget->ItemWrappers.Empty();

			TArray<UObject*> ItemsToUse = ParentWidget->GetListItems();

			if (bShouldShowViewMoreButton)
			{
				ItemsToUse.Add(NewObject<UViewMoreWidgetItem>(ParentWidget.Get(), UViewMoreWidgetItem::StaticClass(),
															  NAME_None, RF_NoFlags, nullptr, false, nullptr));
			}

			bool bAddedViewMoreButton = false;
			for (int32 Index = 0; Index < TilesToShow; Index++)
			{
				UObject* ItemForWrapper = ItemsToUse[Index % ItemsToUse.Num()];
				if (bShouldShowViewMoreButton && ItemForWrapper->IsA<UViewMoreWidgetItem>())
				{
					if (!bAddedViewMoreButton) 
					{
						ParentWidget->ItemWrappers.Add(ItemForWrapper);
						bAddedViewMoreButton = true;
					}
					else
					{
						ParentWidget->ItemWrappers.Add(NewObject<UViewMoreWidgetItem>(ParentWidget.Get(), UViewMoreWidgetItem::StaticClass(),
															  NAME_None, RF_NoFlags, nullptr, false, nullptr));
					}
					continue;
				}

				UItemSourceItemWrapper* Wrapper =
					NewObject<UItemSourceItemWrapper>(ParentWidget.Get(), UItemSourceItemWrapper::StaticClass(),
													  NAME_None, RF_NoFlags, nullptr, false, nullptr);
				Wrapper->UnderlyingItem = ItemForWrapper;
				ParentWidget->ItemWrappers.Add(Wrapper);
			}
		}

		const bool bNeedsRefreshOrder = bNeedsRefreshCount || LastOffset != ParentWidget->GetListOffset();
		if (bNeedsRefreshOrder)
		{
			ParentWidget->OrderedItemWrappers.SetNum(TilesToShow);
			LastOffset = ParentWidget->GetListOffset();
			for (int32 Index = 0; Index < TilesToShow; Index++)
			{
				int32 SourceIndex = (LastOffset + Index) % TilesToShow;
				if (SourceIndex < 0)
				{
					const int32 PositiveSourceIndex = -SourceIndex;
					const int32 Remainder = PositiveSourceIndex % TilesToShow;
					SourceIndex = TilesToShow - Remainder;
				}
				UObject* Wrapper = ParentWidget->ItemWrappers[SourceIndex];
				ParentWidget->OrderedItemWrappers[Index] = Wrapper;
			}
		}

		if (ParentWidget->OrderedItemWrappers.IsEmpty())
		{
			return TArrayView<const ItemType>();
		}

		return TArrayView<const ItemType>(
			reinterpret_cast<const ItemType*>(ParentWidget->OrderedItemWrappers.GetData()),
			ParentWidget->OrderedItemWrappers.Num());
	}

	virtual bool IsSame(const void* RawPointer) const override
	{
		return RawPointer == static_cast<const void*>(this);
	}

private:
	mutable TWeakObjectPtr<UModioDefaultCarouselModListView> ParentWidget;

	mutable int32 LastOffset = 0;
};

template<typename ItemType>
class FCarouselBackPaddedItemSource : public UE::Slate::ItemsSource::IItemsSource<ItemType>
{
public:
	explicit FCarouselBackPaddedItemSource(UModioDefaultCarouselModListView* InParentWidget)
		: ParentWidget(InParentWidget)
	{}

	virtual const TArrayView<const ItemType> GetItems() const override
	{
		if (!ParentWidget.IsValid())
		{
			return TArrayView<const ItemType>();
		}

		const int32 ListItemsCount = ParentWidget->GetListItems().Num();
		int32 TilesToShow = ListItemsCount + 1; // Add 1 tile for the back padding

		bool bShouldShowViewMoreButton = ParentWidget->ShouldHaveViewMoreButton();
		if (bShouldShowViewMoreButton)
		{
			TilesToShow++; // Add 1 tile for the view more button
		}

		bool bNeedsRefreshCount = ParentWidget->OrderedItemWrappers.Num() != TilesToShow;
		if (bNeedsRefreshCount)
		{
			ParentWidget->OrderedItemWrappers.Empty();
			for (int32 Index = 0; Index < ListItemsCount; Index++)
			{
				UItemSourceItemWrapper* Wrapper =
					NewObject<UItemSourceItemWrapper>(ParentWidget.Get(), UItemSourceItemWrapper::StaticClass(),
													  NAME_None, RF_NoFlags, nullptr, false, nullptr);
				Wrapper->UnderlyingItem = ParentWidget->GetListItems()[Index];
				ParentWidget->OrderedItemWrappers.Add(Wrapper);
			}

			if (bShouldShowViewMoreButton)
			{
				ParentWidget->ViewMoreItem =
					NewObject<UViewMoreWidgetItem>(ParentWidget.Get(), UViewMoreWidgetItem::StaticClass(), NAME_None,
												   RF_NoFlags, nullptr, false, nullptr);
				ParentWidget->OrderedItemWrappers.Add(ParentWidget->ViewMoreItem);
			}

			ParentWidget->BackPaddingItem = NewObject<UListViewBackPaddingWidgetItem>(
				ParentWidget.Get(), UListViewBackPaddingWidgetItem::StaticClass(), NAME_None, RF_NoFlags, nullptr,
				false, nullptr);
			ParentWidget->OrderedItemWrappers.Add(ParentWidget->BackPaddingItem);
		}

		return TArrayView<const ItemType>(
			reinterpret_cast<const ItemType*>(ParentWidget->OrderedItemWrappers.GetData()),
			ParentWidget->OrderedItemWrappers.Num());
	};

	virtual bool IsSame(const void* RawPointer) const override
	{
		return RawPointer == static_cast<const void*>(this);
	}

private:
	mutable TWeakObjectPtr<UModioDefaultCarouselModListView> ParentWidget;
};

TSharedRef<SWidget> UListViewBackPaddingWidget::RebuildWidget()
{
	WidgetTree =
		NewObject<UWidgetTree>(this, UWidgetTree::StaticClass(), NAME_None, RF_NoFlags, nullptr, false, nullptr);

	WidgetTree->RootWidget = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("PaddingBox"));
	SetWidth(1000);

	return Super::RebuildWidget();
}

void UListViewBackPaddingWidget::SetWidth(float InWidth)
{
	Cast<USizeBox>(WidgetTree->RootWidget)->SetWidthOverride(InWidth);
}

FModioModInfo UItemSourceItemWrapper::GetFullModInfo_Implementation()
{
	return IModioModInfoUIDetails::Execute_GetFullModInfo(UnderlyingItem);
}

FModioModID UItemSourceItemWrapper::GetModID_Implementation()
{
	return IModioModInfoUIDetails::Execute_GetModID(UnderlyingItem);
}

float UModioDefaultCarouselModListView::CalculateBackPaddingWidth() const
{
	const int32 BackPaddingIndex = OrderedItemWrappers.IndexOfByKey(BackPaddingItem);
	if (BackPaddingIndex <= 0)
	{
		return 1000.0f;
	}

	UUserWidget* WidgetBeforePadding = GetEntryWidgetFromItem(OrderedItemWrappers[BackPaddingIndex - 1]);
	if (!WidgetBeforePadding)
	{
		return 1000.0f;
	}

	const FGeometry ThisGeometry = GetCachedGeometry();
	const float ThisSize = ThisGeometry.GetLocalSize().X;
	const float TileSize = WidgetBeforePadding->GetCachedGeometry().GetLocalSize().X;

	return ThisSize - TileSize;
}

float UModioDefaultCarouselModListView::GetItemWidgetSize() const
{
	for (UUserWidget* Widget : GetDisplayedEntryWidgets())
	{
		if (Widget && Widget->IsVisible() && !Widget->IsA<UListViewBackPaddingWidget>())
		{
			return Widget->GetCachedGeometry().GetAbsoluteSize().X;
		}
	}

	return 0.0f;
}

void UModioDefaultCarouselModListView::Tick(float DeltaTime)
{
	// Clear inertial scrolling every tick since it causes issues with our manual scrolling
	ClearInertialScrolling();

	if (UListViewBackPaddingWidget* BackPaddingWidget =
			GetEntryWidgetFromItem<UListViewBackPaddingWidget>(BackPaddingItem))
	{
		BackPaddingWidget->SetWidth(CalculateBackPaddingWidth());
	}

	TickFocus();

	TickNavigationScrolling(DeltaTime);

	if (bResetScrollWhenOffscreen)
	{
		CheckOffscreen();
	}
}

bool UModioDefaultCarouselModListView::IsTickable() const
{
	// Check if we're being destroyed first
	if (HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed))
	{
		return false;
	}

	// Check if the outer is valid and not being destroyed
	UObject* Outer = GetOuter();
	if (!Outer || Outer->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed))
	{
		return false;
	}

	// Now safe to call GetWorld()
	UWorld* World = GetWorld();
	return World != nullptr && !World->HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed);
}

void UModioDefaultCarouselModListView::CheckOffscreen()
{
	SWidget* CachedWidget = GetCachedWidget().Get();
	if (!CachedWidget)
	{
		return;
	}
	const uint64 CurrentFrame = GFrameCounter;
	if (CurrentFrame <
		static_cast<SModioListView<UObject*>*>(CachedWidget)->LastPaintedFrame + OffscreenResetFrameCount)
	{
		return;
	}

	ScrollDirectly(bLoopTiles ? DefaultLoopingListItemOffset : 0.0f);
	ListItemOffset = DefaultLoopingListItemOffset;
}

void UModioDefaultCarouselModListView::TickFocus()
{
	APlayerController* PlayerController = GetOwningPlayer();
	const bool bHasAnyUserFocus = HasUserFocus(PlayerController) || HasUserFocusedDescendants(PlayerController);
	if (bHasAnyUserFocus != bHadAnyUserFocus)
	{
		bHadAnyUserFocus = bHasAnyUserFocus;
		OnUserFocusChanged(bHasAnyUserFocus);
	}
}

TSubclassOf<UUserWidget> UModioDefaultCarouselModListView::GetDesiredEntryClassForItem(UObject* Item) const
{
	if (Item->IsA<UListViewBackPaddingWidgetItem>())
	{
		return UListViewBackPaddingWidget::StaticClass();
	}
	else if (Item->IsA<UViewMoreWidgetItem>())
	{
		return ViewMoreButtonClass;
	}

	return ITypedUMGListView<UObject*>::GetDesiredEntryClassForItem(Item);
}

bool UModioDefaultCarouselModListView::ShouldHaveViewMoreButton() const
{
	return !GetListItems().IsEmpty() && ViewMoreButtonClass;
}

void UModioDefaultCarouselModListView::OnUserFocusChanged(bool bNewFocus)
{
	if (!bNewFocus)
	{
		// If we have a selected item then unselect it
		UObject* SelectedItem = GetSelectedItem();
		if (SelectedItem)
		{
			IModioUIObjectSelector::Execute_SetSelectedStateForValue(this, SelectedItem, false, false);
		}
		return;
	}

	// if the selected item is hovered by the mouse, dont change the selection.
	UWidget* SelectedWidget = GetEntryWidgetFromItem(GetSelectedItem());
	if (SelectedWidget && SelectedWidget->IsHovered())
	{
		return;
	}

	UObject* PreferredFocusItem = GetPreferredFocusedItem();
	if (!PreferredFocusItem || HasScrollVelocity())
	{
		bHadAnyUserFocus = false; // Cant focus
		return;
	}
	UWidget* FocusWidget = GetEntryWidgetFromItem(PreferredFocusItem);
	if (FocusWidget)
	{
		UWidget* WidgetToFocus = IModioFocusableWidget::Execute_GetWidgetToFocus(FocusWidget, EUINavigation::Next);
		if (WidgetToFocus)
		{
			FocusWidget = WidgetToFocus;
		}
		FocusWidget->SetUserFocus(GetOwningPlayer());
	}
	IModioUIObjectSelector::Execute_SetSelectedStateForValue(this, PreferredFocusItem, true, false);
	NavigateTo(PreferredFocusItem);
}

UObject* UModioDefaultCarouselModListView::GetPreferredFocusedItem() const
{
	FGeometry ThisGeometry = GetCachedGeometry();
	const float ThisSize = ThisGeometry.GetAbsoluteSize().X;
	const float ThisStart = ThisGeometry.GetAbsolutePosition().X;
	const float TileSize = GetItemWidgetSize() + GetHorizontalEntrySpacing();
	const float TileSizePercent = TileSize / ThisSize;
	const float DesiredPosition = (DefaultItemScrollOffset * TileSizePercent) + ThisStart;

	UObject* ClosestItem = nullptr;
	float LastDistance = TNumericLimits<float>::Max();
	for (UObject* Item : OrderedItemWrappers)
	{
		UUserWidget* Widget = GetEntryWidgetFromItem(Item);
		if (!Widget)
		{
			continue;
		}

		FGeometry TileGeometry = Widget->GetCachedGeometry();
		const float TileStart = TileGeometry.GetAbsolutePosition().X;
		const float Distance = FMath::Abs(TileStart - DesiredPosition);
		if (Distance < LastDistance)
		{
			LastDistance = Distance;
			ClosestItem = Item;
		}
	}

	if (ClosestItem)
	{
		return ClosestItem;
	}

	const int32 AmountOfItems = OrderedItemWrappers.Num();
	if (AmountOfItems == 0)
	{
		return nullptr;
	}
	int32 Index = (DefaultLoopingListItemOffset) % AmountOfItems;
	return OrderedItemWrappers[Index];
}

void UModioDefaultCarouselModListView::TickNavigationScrolling(float DeltaTime)
{
	// If we grab the list with the mouse or our target somehow isnt in the list anymore, stop scrolling
	if (IsRightClickScrolling() || !OrderedItemWrappers.Contains(TargetNavigation))
	{
		TargetNavigation = nullptr;
	}

	if (!TargetNavigation)
	{
		if (ScrollVelocity != 0.0f)
		{
			const float ScrollDelta = ScrollVelocity * DeltaTime * ScrollingAnimationInterpolationSpeed;

			if (FMath::Abs(ScrollDelta) > 0.01f)
			{
				float NewScroll = Cast<UListView>(this)->GetScrollOffset() - ScrollDelta;
				bool bClearedScrollVelocity = false;
				if (!bLoopTiles)
				{
					float MaxScroll = FMath::Max(GetListItems().Num() - 1.0f, 0.0f);
					if (ShouldHaveViewMoreButton())
					{
						// Consider the view more button
						MaxScroll += 1.0f;
					}
					if (NewScroll > MaxScroll)
					{
						NewScroll = MaxScroll;
						ScrollVelocity = 0;
						bClearedScrollVelocity = true;
					}
					else if (NewScroll < 0)
					{
						NewScroll = 0;
						ScrollVelocity = 0;
						bClearedScrollVelocity = true;
					}
				}

				if (!bClearedScrollVelocity)
				{
					ScrollVelocity -= ScrollDelta;
				}
				bNavigationScroll = true;
				Cast<UListView>(this)->SetScrollOffset(NewScroll);
				bNavigationScroll = false;
				LastDirectScroll = NewScroll;
				OnScroll.Broadcast(this);
			}
			else
			{
				ScrollVelocity = 0.0f;
			}
		}
		return;
	}

	// Cancel any scroll velocity if we have a target
	ScrollVelocity = 0.0f;

	FGeometry ThisGeometry = GetCachedGeometry();
	float ThisStart = ThisGeometry.GetAbsolutePosition().X;
	float ThisSize = ThisGeometry.GetAbsoluteSize().X;
	float ThisEnd = ThisStart + ThisSize;

	float TileStart = 0.0f;
	float TileSize = 0.0f;

	UUserWidget* SelectedWidget = GetEntryWidgetFromItem(TargetNavigation);
	if (SelectedWidget)
	{
		FGeometry TileGeometry = SelectedWidget->GetCachedGeometry();
		TileStart = TileGeometry.GetAbsolutePosition().X;
		TileSize = TileGeometry.GetAbsoluteSize().X + GetHorizontalEntrySpacing();

		APlayerController* PlayerController = GetOwningPlayer();
		const bool bHasAnyUserFocus = HasUserFocus(PlayerController) || HasUserFocusedDescendants(PlayerController);
		if (bHasAnyUserFocus && !SelectedWidget->HasUserFocus(PlayerController) &&
			!SelectedWidget->HasUserFocusedDescendants(PlayerController))
		{
			// Transfer the focus to the widget we are navigating to, as it may have not existed when we started
			// navigation and it was created and came in to view.
			SelectedWidget->SetUserFocus(PlayerController);
		}
	}
	else
	{
		TileSize = GetItemWidgetSize() + GetHorizontalEntrySpacing();
		if (bNavigatingForward)
		{
			// Start one tile to the right of the list view
			TileStart = ThisEnd + TileSize;
		}
		else
		{
			// Start one tile to the left of the list view
			TileStart = -TileSize;
		}
	}

	if (TileSize <= 1.0f || ThisSize <= 1.0f)
	{
		// If we dont have a valid size then we cant scroll
		return;
	}

	const float CurrentPosition = UKismetMathLibrary::MapRangeUnclamped(TileStart, ThisStart, ThisEnd, 0.0f, 1.0f);
	const float TileSizePercent = TileSize / ThisSize;

	const float DesiredPosition = DefaultItemScrollOffset * TileSizePercent;
	const float Delta = DesiredPosition - CurrentPosition;
	const float ScrollDelta = Delta / TileSizePercent * DeltaTime * ScrollingAnimationInterpolationSpeed;

	if (FMath::Abs(ScrollDelta) <= 0.01f && LastScrollDelta == ScrollDelta)
	{
		// Due to some tiny differences in how the target position is calculated here and when the actual widget is
		// rendered, if we are very close to the target position and have a duplicate delta then end scrolling to
		// prevent jitter around the target position.
		TargetNavigation = nullptr;
	}

	if (FMath::Abs(ScrollDelta) > 0.00001f)
	{
		const float CurrentScroll = Cast<UListView>(this)->GetScrollOffset();
		const float NewScroll = CurrentScroll - ScrollDelta;
		bNavigationScroll = true;
		Cast<UListView>(this)->SetScrollOffset(NewScroll);
		bNavigationScroll = false;
		LastDirectScroll = NewScroll;
		LastScrollDelta = ScrollDelta;
		OnScroll.Broadcast(this);
	}
	else
	{
		TargetNavigation = nullptr;
	}
}

int32 UModioDefaultCarouselModListView::GetMinimumTiles() const
{
	if (!bLoopTiles)
	{
		return GetListItems().Num();
	}

	FGeometry ThisGeometry = GetCachedGeometry();
	const float ThisSize = ThisGeometry.GetAbsoluteSize().X;
	const float TileSize = GetItemWidgetSize();

	if (ThisSize <= 1.0f || TileSize <= 1.0f)
	{
		// Assume the widget isnt set up properly yet
		return MinimumLoopingTiles;
	}

	const float TilesThatFit = ThisSize / (TileSize + GetHorizontalEntrySpacing());

	// Round up, then add the default offset on either side, plus one extra to ensure we have enough when scrolling.
	const int32 CalculatedMinimumTiles = FMath::CeilToInt(TilesThatFit) + DefaultLoopingListItemOffset * 2 + 1;

	const int32 Result = FMath::Max(MinimumLoopingTiles, CalculatedMinimumTiles);
	return Result;
}

TSharedRef<STableViewBase> UModioDefaultCarouselModListView::RebuildListWidget()
{
	TSharedRef<STableViewBase> ListWidget = ConstructListView<SModioListView>();

	TSharedRef<SListView<UObject*>> ListView = StaticCastSharedRef<SListView<UObject*>>(ListWidget);

	if (bLoopTiles)
	{
		ListView->SetItemsSource(MakeUnique<FCarouselLoopingItemSource<UObject*>>(this));
	}
	else
	{
		ListView->SetItemsSource(MakeUnique<FCarouselBackPaddedItemSource<UObject*>>(this));
	}

	return ListWidget;
}

void UModioDefaultCarouselModListView::NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting)
{
	Super::NativeSetListItems(InListItems, bAddToExisting);

	if (bScrollAdjustment || !bLoopTiles)
	{
		return;
	}

	ScrollDirectly(DefaultLoopingListItemOffset);
	ListItemOffset = -DefaultLoopingListItemOffset;
}

void UModioDefaultCarouselModListView::ScrollDirectly(float NewScroll)
{
	bScrollAdjustment = true;

	Cast<UListViewBase>(this)->SetScrollOffset(NewScroll);
	LastDirectScroll = NewScroll;

	bScrollAdjustment = false;
	OnScroll.Broadcast(this);
}

bool UModioDefaultCarouselModListView::IsRightClickScrolling() const
{
	if (!MyListView)
	{
		return false;
	}

	return MyListView->IsRightClickScrolling();
}

void UModioDefaultCarouselModListView::ClearInertialScrolling()
{
	if (!MyListView)
	{
		return;
	}

	MyListView->EndInertialScrolling();
}

void UModioDefaultCarouselModListView::NavigateTo(UObject* Item)
{
	TargetNavigation = Item;
}

void UModioDefaultCarouselModListView::OnListViewScrolledInternal(float ItemOffset, float DistanceRemaining)
{
	if (bScrollAdjustment)
	{
		return;
	}

	if (!bNavigationScroll && !IsRightClickScrolling())
	{
		// If we didnt scroll intentionally then undo the scroll action
		ScrollDirectly(LastDirectScroll);
		return;
	}

	ON_SCOPE_EXIT
	{
		Super::OnListViewScrolledInternal(ItemOffset, DistanceRemaining);
	};
	if (!bLoopTiles)
	{
		LastDirectScroll = ItemOffset;
		return;
	}

	if (ItemOffset >= DefaultLoopingListItemOffset + 1.0f)
	{
		int32 FloorOffset = FMath::FloorToInt(ItemOffset);

		ScrollDirectly(ItemOffset - FloorOffset + DefaultLoopingListItemOffset);

		ListItemOffset += FloorOffset - DefaultLoopingListItemOffset;
	}
	else if (ItemOffset <= DefaultLoopingListItemOffset - 1.0f)
	{
		int32 FloorOffset = FMath::FloorToInt(ItemOffset);

		ScrollDirectly(ItemOffset - FloorOffset + DefaultLoopingListItemOffset - 1.0f);

		ListItemOffset -= DefaultLoopingListItemOffset - FloorOffset - 1;
	}
	else
	{
		LastDirectScroll = ItemOffset;
	}
}

void UModioDefaultCarouselModListView::NativeSetText(const FText& DisplayText)
{
	PresetName = DisplayText;
}

FText UModioDefaultCarouselModListView::NativeGetText()
{
	return PresetName;
}

UUserWidget& UModioDefaultCarouselModListView::OnGenerateEntryWidgetInternal(
	UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	UUserWidget& Result = Super::OnGenerateEntryWidgetInternal(Item, DesiredEntryClass, OwnerTable);

	if (UItemSourceItemWrapper* ItemWrapperDataSource = Cast<UItemSourceItemWrapper>(Item))
	{
		ItemWrapperDataSource->NavHandler = NewObject<UItemWidgetNavigationHandler>(&Result);
		ItemWrapperDataSource->NavHandler->HandleWidget(Result, this);
	}
	else if (UViewMoreWidgetItem* ViewMoreWidgetItem = Cast<UViewMoreWidgetItem>(Item))
	{
		ViewMoreWidgetItem->NavHandler = NewObject<UItemWidgetNavigationHandler>(&Result);
		ViewMoreWidgetItem->NavHandler->HandleWidget(Result, this);
		ViewMoreWidgetItem->ViewMorePresetFilterName = PresetName;
		IModioUIDataSourceWidget::Execute_SetDataSource(&Result, Item);
	}
	else if (UListViewBackPaddingWidget* PaddingWidget = Cast<UListViewBackPaddingWidget>(&Result))
	{
		PaddingWidget->SetWidth(CalculateBackPaddingWidth());
	}

	return Result;
}

UWidget* UModioDefaultCarouselModListView::HandleItemNavigation(UWidget* Widget, EUINavigation InNavigation)
{
	if (!ensure(InNavigation == EUINavigation::Left || InNavigation == EUINavigation::Right))
	{
		// Only left and right navigation is supported for carousel navigation right now
		return Widget;
	}

	if (!Widget || !Widget->Implements<UModioUIDataSourceWidget>())
	{
		return Widget;
	}

	UObject* DataSource = IModioUIDataSourceWidget::Execute_GetDataSource(Widget);
	if (!DataSource)
	{
		return Widget;
	}
	IModioUIObjectSelector::Execute_SetSelectedStateForValue(this, DataSource, false, false);

	UWidget* DirectionalWidget = NavigateInDirection(InNavigation);
	return DirectionalWidget ? DirectionalWidget : Widget;
}

UWidget* UModioDefaultCarouselModListView::NavigateInDirection(EUINavigation InNavigation)
{
	if (!ensure(InNavigation == EUINavigation::Left || InNavigation == EUINavigation::Right))
	{
		// Only left and right navigation is supported for carousel navigation right now
		return nullptr;
	}

	// unselect currently selected item if one is selected
	if (UObject* SelectedItem = GetSelectedItem())
	{
		IModioUIObjectSelector::Execute_SetSelectedStateForValue(this, SelectedItem, false, false);
	}

	int32 Index = OrderedItemWrappers.IndexOfByKey(GetPreferredFocusedItem());

	Index += (InNavigation == EUINavigation::Left) ? -1 : 1;
	if (bLoopTiles)
	{
		if (Index < 0)
		{
			const int32 PositiveIndex = -Index;
			const int32 Remainder = PositiveIndex % OrderedItemWrappers.Num();
			Index = OrderedItemWrappers.Num() - Remainder;
		}
		else if (Index >= OrderedItemWrappers.Num())
		{
			Index %= OrderedItemWrappers.Num();
		}
	}
	else
	{
		int32 NavigableTileCount = GetListItems().Num();
		if (ShouldHaveViewMoreButton())
		{
			// Allow navigation to the view more button
			NavigableTileCount++;
		}
		Index = FMath::Clamp(Index, 0, NavigableTileCount - 1);
	}

	if (UObject* TargetItem = OrderedItemWrappers[Index])
	{
		IModioUIObjectSelector::Execute_SetSelectedStateForValue(this, TargetItem, true, false);
		NavigateTo(TargetItem);
		bNavigatingForward = InNavigation == EUINavigation::Right;
		UWidget* FocusWidget = GetEntryWidgetFromItem(TargetItem);
		return FocusWidget;
	}

	return nullptr;
}

float UModioDefaultCarouselModListView::GetItemCountOnScreen() const
{
	FGeometry ThisGeometry = GetCachedGeometry();
	const float ThisSize = ThisGeometry.GetAbsoluteSize().X;
	const float TileSize = GetItemWidgetSize();

	if (ThisSize <= 1.0f || TileSize <= 1.0f)
	{
		// Assume the widget isnt set up properly yet
		return 1;
	}

	return ThisSize / (TileSize + GetHorizontalEntrySpacing());
}

void UModioDefaultCarouselModListView::ScrollRow(bool bBackwards)
{
	const float ItemsOnScreen = GetItemCountOnScreen();
	const int32 ItemsToScroll = FMath::Max(1, FMath::FloorToInt(ItemsOnScreen));
	const int32 CurrentIndex = OrderedItemWrappers.IndexOfByKey(GetPreferredFocusedItem());
	int32 TargetIndex;
	if (bBackwards)
	{
		TargetIndex = CurrentIndex - ItemsToScroll;
	}
	else
	{
		TargetIndex = CurrentIndex + ItemsToScroll;
	}
	if (bLoopTiles)
	{
		if (TargetIndex < 0)
		{
			const int32 PositiveIndex = -TargetIndex;
			const int32 Remainder = PositiveIndex % OrderedItemWrappers.Num();
			TargetIndex = OrderedItemWrappers.Num() - Remainder;
		}
		else if (TargetIndex >= OrderedItemWrappers.Num())
		{
			TargetIndex %= OrderedItemWrappers.Num();
		}
	}
	else
	{
		int32 NavigableTileCount = GetListItems().Num();
		if (ShouldHaveViewMoreButton())
		{
			// Allow navigation to the view more button
			NavigableTileCount++;
		}
		TargetIndex = FMath::Clamp(TargetIndex, 0, NavigableTileCount - 1);
	}

	if (UObject* TargetItem = OrderedItemWrappers[TargetIndex])
	{
		NavigateTo(TargetItem);
		bNavigatingForward = !bBackwards;
	}
}

void UModioDefaultCarouselModListView::ScrollVisibleArea_Implementation(bool bBackwards)
{
	ScrollRow(bBackwards);
}

bool UModioDefaultCarouselModListView::CanScrollInDirection_Implementation(bool bBackward) const
{
	if (bLoopTiles)
	{
		return !GetListItems().IsEmpty();
	}

	const float CurrentScroll = Cast<UListView>(this)->GetScrollOffset();
	if (bBackward)
	{
		return CurrentScroll > ScrollCheckPadding;
	}
	const bool bShowViewMore = ShouldHaveViewMoreButton();
	if (bShowViewMore && bScrollCheckViewMoreTileVisible)
	{
		UWidget* ViewMoreWidget = GetEntryWidgetFromItem(ViewMoreItem);
		if (ViewMoreWidget)
		{
			FGeometry ViewMoreGeometry = ViewMoreWidget->GetCachedGeometry();
			FGeometry ThisGeometry = GetCachedGeometry();
			const float ThisEnd = ThisGeometry.GetAbsolutePosition().X + ThisGeometry.GetAbsoluteSize().X;
			const float ViewMoreStart =
				ViewMoreGeometry.GetAbsolutePosition().X + (ScrollCheckPadding * ViewMoreGeometry.GetAbsoluteSize().X);
			if (ViewMoreStart < ThisEnd)
			{
				return false;
			}
		}
	}

	float MaxScroll = FMath::Max(GetListItems().Num() - 1.0f - ScrollCheckPadding, 0.0f);
	if (bShowViewMore)
	{
		// Consider the view more button
		MaxScroll += 1.0f;
	}

	return CurrentScroll < MaxScroll;
}

UWidget* UModioDefaultCarouselModListView::NativeGetWidgetToFocus(EUINavigation NavigationType) const
{
	return GetEntryWidgetFromItem(GetPreferredFocusedItem());
}

void UModioDefaultCarouselModListView::NativeAddFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler) {}

void UModioDefaultCarouselModListView::NativeRemoveFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler) {}

void UItemWidgetNavigationHandler::HandleWidget(UUserWidget& InWidget,
												UModioDefaultCarouselModListView* InParentListView)
{
	FCustomWidgetNavigationDelegate OnNavigateDelegate;
	OnNavigateDelegate.BindDynamic(this, &UItemWidgetNavigationHandler::OnNavigate);
	InWidget.SetNavigationRuleCustom(EUINavigation::Left, OnNavigateDelegate);
	InWidget.SetNavigationRuleCustom(EUINavigation::Right, OnNavigateDelegate);

	ParentListView = InParentListView;
	HandledWidget = &InWidget;
}

UWidget* UItemWidgetNavigationHandler::OnNavigate(EUINavigation Navigation)
{
	if (!ensure(ParentListView && HandledWidget))
	{
		return nullptr;
	}

	return ParentListView->HandleItemNavigation(HandledWidget, Navigation);
}
