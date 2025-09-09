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

#include "Components/CheckBox.h"
#include "CoreMinimal.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUIHasTooltipWidget.h"
#include "UI/Interfaces/IModioUISelectableWidget.h"
#include "Widgets/Input/SCheckBox.h"

#include "ModioDefaultCheckBox.generated.h"

class MODIOUICORE_API SModioDefaultCheckBox : public SCheckBox
{
	SModioDefaultCheckBox(SModioDefaultCheckBox&&) = delete;
	SModioDefaultCheckBox& operator = (SModioDefaultCheckBox&&) = delete;

	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		OnHoverChanged.Broadcast(true);
		return SCheckBox::OnMouseEnter(MyGeometry, MouseEvent);
	}

	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override
	{
		OnHoverChanged.Broadcast(false);
		return SCheckBox::OnMouseLeave(MouseEvent);
	}

	virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override
	{
		OnHoverChanged.Broadcast(true);
		return SCheckBox::OnFocusReceived(MyGeometry, InFocusEvent);
	}

	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override
	{
		OnHoverChanged.Broadcast(false);
		return SCheckBox::OnFocusLost(InFocusEvent);
	}

public:
	SModioDefaultCheckBox() : SCheckBox() {}
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHoverChanged, bool);
	FOnHoverChanged OnHoverChanged;
};

class UModioDefaultTextBlock;

/**
 * @brief Default implementation of a check box that can be used as a component in mod.io UI
 */
UCLASS()
class MODIOUICORE_API UModioDefaultCheckBox : public UCheckBox,
											  public IModioUISelectableWidget,
											  public IModioUIHasTextWidget,
											  public IModioUIHasTooltipWidget
{
	GENERATED_BODY()

public:
	UModioDefaultCheckBox();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "mod.io|UI|Check Box")
	FMargin ContentPadding;

	/**
	 * @brief Sets the label of the Check Box
	 * @param InLabelText The label to set
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Check Box")
	void SetLabel(const FText& InLabelText);

	/**
	 * Gets the label of the Check Box
	 * @return The label
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Check Box")
	FText GetLabel() const;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnModioDefaultCheckBoxStateChanged, bool);
	/**
	 * Delegate called when the check box state changes
	 */
	FOnModioDefaultCheckBoxStateChanged OnModioDefaultCheckBoxStateChanged;

	/** Bound handlers for the check box state changed event (for IModioUISelectableWidget binding/unbinding) */
	TMap<FModioSelectableOnSelectionChanged, FDelegateHandle> ToggleableBoundHandlers;

protected:
	/**
	 * The class to use for the label widget
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MustImplement = "ModioUIHasTextWidget"),
			  Category = "mod.io|UI|Check Box", meta = (BlueprintProtected))
	TSubclassOf<UWidget> LabelClass;

	/**
	 * The label widget (created internally from LabelClass)
	 */
	UPROPERTY(Transient)
	TObjectPtr<UWidget> LabelWidget;

	/**
	 * The label text
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "mod.io|UI|Check Box",
			  meta = (BlueprintProtected))
	FText LabelText;

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	//~ Begin IModioUISelectableWidget Interface
	virtual void AddSelectedStateChangedHandler_Implementation(
		const FModioSelectableOnSelectionChanged& Handler) override;
	virtual void RemoveSelectedStateChangedHandler_Implementation(
		const FModioSelectableOnSelectionChanged& Handler) override;
	virtual void SetSelectable_Implementation(bool bNewSelectableState) override;
	virtual void GetSelectable_Implementation(bool& bIsSelectable) override;
	virtual void SetSelectedState_Implementation(bool bNewSelectedState) override;
	virtual void ToggleSelectedState_Implementation() override;
	virtual void GetSelectedState_Implementation(bool& bCurrentSelectedState) override;
	virtual void SetToggleable_Implementation(bool bNewToggleableState) override;
	virtual void GetToggleable_Implementation(bool& bIsToggleable) override;
	//~ End IModioUISelectableWidget Interface

	//~ Begin IModioUIHasTextWidget Interface
	virtual void NativeSetText(const FText& DisplayText) override;
	virtual FText NativeGetText() override;
	virtual void NativeSetTextFont(FSlateFontInfo FontInfo) override;
	virtual FSlateFontInfo NativeGetTextFont() override;
	//~ End IModioUIHasTextWidget Interface

	//~ Begin IModioUIHasTooltipWidget Interface
	virtual void SetTooltipEnabledState_Implementation(bool bNewEnabledState) override;
	virtual void ConfigureTooltip_Implementation(const FText& TitleText, const FText& InfoText,
												 const FText& TagText) override;
	//~ End IModioUIHasTooltipWidget Interface

public:
	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface
protected:
	/**
	 * Get the label text block text
	 * @return The label text block text
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Check Box", meta = (BlueprintProtected))
	FText GetLabelTextBlockText() const;

	/**
	 * Get the content widget of the check box
	 * @return The content widget
	 */
	virtual TSharedRef<SWidget> GetContentWidget();

	/**
	 * Get the content padding of the check box
	 * @return The content padding
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "mod.io|UI|Check Box", meta = (BlueprintProtected))
	FMargin GetCheckboxContentPadding() const;

	/**
	 * Handle the check box state changed event
	 * @param bIsChecked The new state of the check box
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "mod.io|UI|Check Box", meta = (BlueprintProtected))
	void HandleOnCheckStateChanged(bool bIsChecked);

	/**
	 * Handle the check box hovered event
	 * @param bHovered Whether the check box is hovered
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "mod.io|UI|Check Box", meta = (BlueprintProtected))
	void SetHovered(bool bHovered);
};