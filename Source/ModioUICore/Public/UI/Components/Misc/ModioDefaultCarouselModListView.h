/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UI/Components/Misc/ModioDefaultModListView.h"
#include "UI/Interfaces/IModioModInfoUIDetails.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioFocusableWidget.h"

#include "ModioDefaultCarouselModListView.generated.h"

class UModioDefaultCarouselModListView;
class UItemWidgetNavigationHandler;
class USizeBox;
enum class EUINavigation : uint8;

UCLASS()
class MODIOUICORE_API UItemWidgetNavigationHandler : public UObject
{
	GENERATED_BODY()

public:
	void HandleWidget(UUserWidget& InWidget, UModioDefaultCarouselModListView* InParentListView);

protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> HandledWidget;

	UPROPERTY()
	TObjectPtr<UModioDefaultCarouselModListView> ParentListView;

	UFUNCTION()
	UWidget* OnNavigate(EUINavigation Navigation);
};

UCLASS()
class MODIOUICORE_API UItemSourceItemWrapper : public UObject, public IModioModInfoUIDetails
{
	GENERATED_BODY()
public:
	UItemSourceItemWrapper() = default;

	//~ Begin IModioModInfoUIDetails Interface
	virtual FModioModInfo GetFullModInfo_Implementation() override;
	virtual FModioModID GetModID_Implementation() override;
	//~ End IModioModInfoUIDetails Interface

	UPROPERTY()
	TObjectPtr<UItemWidgetNavigationHandler> NavHandler;

	UPROPERTY()
	TObjectPtr<UObject> UnderlyingItem;
};

UCLASS()
class MODIOUICORE_API UListViewBackPaddingWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	void SetWidth(float InWidth);
};

UCLASS(BlueprintType)
class MODIOUICORE_API UListViewBackPaddingWidgetItem : public UObject
{
	GENERATED_BODY()
};

UCLASS(BlueprintType)
class MODIOUICORE_API UViewMoreWidgetItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UItemWidgetNavigationHandler> NavHandler;

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|Mod Carousel")
	FText ViewMorePresetFilterName;
};

/**
 * @default_impl_for Mod Carousel List View
 * @brief A default implementation of IModioUIModListViewInterface which displays mods as a carousel. This widget
 * inherits from UModioDefaultModListView.
 * @component_display_name Mod Carousel List View
 */
UCLASS()
class MODIOUICORE_API UModioDefaultCarouselModListView : public UModioDefaultModListView,
														 public IModioUIHasTextWidget,
														 public IModioFocusableWidget,
														 public FTickableGameObject
{
	GENERATED_BODY()

protected:
	//~ Begin FTickableGameObject
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UModioDefaultCarouselModListView, STATGROUP_Tickables);
	}

	virtual bool IsTickable() const override;
	//~ End FTickableGameObject

	//~ Begin IModioUIModListViewInterface Interface
	virtual void NativeSetListItems(const TArray<UObject*>& InListItems, bool bAddToExisting) override;
	//~ End IModioUIModListViewInterface Interface

	//~ Begin IModioScrollableWidget Interface
	virtual void ScrollVisibleArea_Implementation(bool bBackwards) override;
	virtual bool CanScrollInDirection_Implementation(bool bBackward) const override;
	//~ End IModioScrollableWidget Interface

	//~ Begin IModioUIHasTextWidget Interface
	virtual void NativeSetText(const FText& DisplayText) override;
	virtual FText NativeGetText() override;
	//~ End IModioUIHasTextWidget Interface

	//~ Begin IModioFocusableWidget Interface
	virtual UWidget* NativeGetWidgetToFocus(EUINavigation NavigationType) const override;
	virtual void NativeAddFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler) override;
	virtual void NativeRemoveFocusPathChangedHandler(const FModioOnFocusPathChanged& Handler) override;
	//~ End IModioFocusableWidget Interface

	virtual void OnListViewScrolledInternal(float ItemOffset, float DistanceRemaining) override;
	virtual TSharedRef<STableViewBase> RebuildListWidget() override;

	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass,
													   const TSharedRef<STableViewBase>& OwnerTable) override;
	virtual TSubclassOf<UUserWidget> GetDesiredEntryClassForItem(UObject* Item) const override;

	void TickNavigationScrolling(float DeltaTime);
	void TickFocus();
	void CheckOffscreen();
	void OnUserFocusChanged(bool bNewFocus);

	bool IsRightClickScrolling() const;

	void NavigateTo(UObject* Item);

	void ClearInertialScrolling();

	UWidget* HandleItemNavigation(UWidget* Widget, EUINavigation InNavigation);

public:
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Carousel")
	UWidget* NavigateInDirection(EUINavigation InNavigation);

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Carousel")
	float GetItemCountOnScreen() const;

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Mod Carousel")
	void ScrollRow(bool bBackwards);

	int32 GetListOffset() const
	{
		return ListItemOffset;
	}

	int32 GetMinimumTiles() const;

protected:
	float GetItemWidgetSize() const;

	UObject* GetPreferredFocusedItem() const;

	// Scrolls and sets bScrollAdjustment to true so OnListViewScrolledInternal can early exit
	void ScrollDirectly(float NewScroll);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "mod.io|UI|Mod Carousel", meta = (BlueprintProtected))
	int32 DefaultLoopingListItemOffset = 2;

	// How much the the items are offset when scrolling into view. Measured in item widget size, so 0.1 means 10% of the
	// widget size will be offset to the right when scrolled into view.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "mod.io|UI|Mod Carousel", meta = (BlueprintProtected))
	float DefaultItemScrollOffset = 0.0f;

	// How many minimum tiles we should generate when looping tiles. Needed in order to fill the screen with enough
	// tiles to loop correctly.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "mod.io|UI|Mod Carousel", meta = (BlueprintProtected))
	float MinimumLoopingTiles = 20;

	// If we should loop tiles or not
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "mod.io|UI|Mod Carousel", meta = (BlueprintProtected))
	bool bLoopTiles = true;

	// Widget to spawn for the "View More" button
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "mod.io|UI|Mod Carousel", meta = (BlueprintProtected))
	TSubclassOf<UUserWidget> ViewMoreButtonClass;

	// How much space to ignore when checking if we can scroll in a direction. Meaasured in percent of the item widget
	// size
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "mod.io|UI|Mod Carousel", meta = (BlueprintProtected))
	float ScrollCheckPadding = 0.1f;

	// If we should check for the view more tile being visible when checking if we can scroll forward and prevent
	// scrolling if it is
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "mod.io|UI|Mod Carousel", meta = (BlueprintProtected))
	bool bScrollCheckViewMoreTileVisible = true;

	// If true, will reset the scroll offset when no tiles are visible.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "mod.io|UI|Mod Carousel", meta = (BlueprintProtected))
	bool bResetScrollWhenOffscreen = true;

	// How many frames the widget must be off screen before we reset the scroll offset if bResetScrollWhenOffscreen is
	// true
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "mod.io|UI|Mod Carousel", meta = (BlueprintProtected))
	int32 OffscreenResetFrameCount = 5;

	float CalculateBackPaddingWidth() const;

	FText PresetName = FText::GetEmpty();

	UPROPERTY()
	TObjectPtr<UObject> TargetNavigation;

	int32 ListItemOffset = 0;

	bool bScrollAdjustment = false;
	bool bNavigationScroll = false;
	bool bScrollingFromNavigation = false;
	bool bNavigatingForward = false;
	bool bHadAnyUserFocus = false;

	float LastDirectScroll = 0.0f;

	float ScrollVelocity = 0.0f;

	bool HasScrollVelocity() const
	{
		return FMath::Abs(ScrollVelocity) > 0.01f;
	}

	float LastScrollDelta = 0.0f;

public:
	// Wrapper objects ordered in the same order as GetListItems.
	UPROPERTY()
	TArray<TObjectPtr<UObject>> ItemWrappers;

	// Wrapper objects ordered in the order we want to display them in the list.
	UPROPERTY()
	TArray<TObjectPtr<UObject>> OrderedItemWrappers;

	UPROPERTY()
	TObjectPtr<UObject> ViewMoreItem;

	UPROPERTY()
	TObjectPtr<UObject> BackPaddingItem;

	bool ShouldHaveViewMoreButton() const;

protected:
	friend class UItemWidgetNavigationHandler;
};
