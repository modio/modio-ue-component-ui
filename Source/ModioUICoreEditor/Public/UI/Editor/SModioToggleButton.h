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
#include "Widgets/Input/SButton.h"

class MODIOUICOREEDITOR_API SModioToggleButton : public SButton
{
protected:
	bool bCurrentState = false;

public:
	SLATE_BEGIN_ARGS(SModioToggleButton)
		: _HAlign(HAlign_Fill),
		  _VAlign(VAlign_Fill),
		  _ContentPadding(FMargin(4.0, 2.0))
	{}

	/** Horizontal alignment */
	SLATE_ARGUMENT(EHorizontalAlignment, HAlign)

	/** Vertical alignment */
	SLATE_ARGUMENT(EVerticalAlignment, VAlign)

	/** Spacing between button's border and the content. */
	SLATE_ATTRIBUTE(FMargin, ContentPadding)

	SLATE_ATTRIBUTE(FText, TrueText)
	SLATE_ATTRIBUTE(FText, FalseText)

	/** Called when the button is clicked */
	SLATE_EVENT(FSimpleDelegate, ClickedWhenTrue)
	SLATE_EVENT(FSimpleDelegate, ClickedWhenFalse)

	SLATE_ARGUMENT(bool, bInitialState)

	SLATE_END_ARGS()

	void Construct(const SModioToggleButton::FArguments& InArgs)
	{
		SButton::Construct(
			SButton::FArguments()
				.HAlign(InArgs._HAlign)
				.VAlign(InArgs._VAlign)
				.ContentPadding(InArgs._ContentPadding)
				.Text_Lambda([this, TrueText = InArgs._TrueText, FalseText = InArgs._FalseText]() {
					return bCurrentState ? TrueText.Get() : FalseText.Get();
				})
				.OnClicked_Lambda([this, TrueDelegate = InArgs._ClickedWhenTrue, FalseDelegate = InArgs._ClickedWhenFalse]() {
					
					const FSimpleDelegate& WrappedDelegate = bCurrentState ? TrueDelegate : FalseDelegate;
					if (WrappedDelegate.IsBound())
					{
						WrappedDelegate.Execute();
					}
					bCurrentState = !bCurrentState;
					return FReply::Handled();
				}));
		bCurrentState = InArgs._bInitialState;
	}

	bool GetState()
	{
		return bCurrentState;
	}
};