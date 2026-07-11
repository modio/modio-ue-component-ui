/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Editor/ComponentTestWidgets/SModioUIImageDisplayTestWidget.h"

#include "UI/Editor/SModioToggleButton.h"
#include "Widgets/Input/SNumericEntryBox.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SModioUIImageDisplayTestWidget)

void SModioUIImageDisplayTestWidget::Construct(const FArguments& InArgs)
	{
		TargetObject = InArgs._TargetObject.Get();
		DelegateBridge = TStrongObjectPtr<UModioUIImageDisplayBridge>(NewObject<UModioUIImageDisplayBridge>());
		DelegateBridge->OnValuechanged.BindSP(this, &SModioUIImageDisplayTestWidget::HandleImageLoaded);
		ImageLoadedListener.BindUFunction(DelegateBridge.Get(), FName("HandleImageLoaded"));

		IModioUIImageDisplayWidget::Execute_AddImageLoadEventHandler(TargetObject.Get(), ImageLoadedListener);

		// clang-format off
		ChildSlot
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SModioToggleButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.bInitialState(true)
				.TrueText(FText::FromString("Disable Image Loaded Handler"))
				.FalseText(FText::FromString("Enable Image Loaded Handler"))
				.ClickedWhenTrue_Lambda([this](){IModioUIImageDisplayWidget::Execute_RemoveImageLoadEventHandler(TargetObject.Get(), ImageLoadedListener);})
				.ClickedWhenFalse_Lambda([this](){IModioUIImageDisplayWidget::Execute_AddImageLoadEventHandler(TargetObject.Get(), ImageLoadedListener);})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SModioToggleButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.TrueText(FText::FromString("Don't Match Texture Size"))
				.FalseText(FText::FromString("Match Texture Size"))
				.ClickedWhenTrue_Lambda([this](){IModioUIImageDisplayWidget::Execute_SetBrushMatchTextureSize(TargetObject.Get(), false);})
				.ClickedWhenFalse_Lambda([this](){IModioUIImageDisplayWidget::Execute_SetBrushMatchTextureSize(TargetObject.Get(), true);})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Set Desired Size"))
				.OnClicked_Lambda([this](){IModioUIImageDisplayWidget::Execute_SetDesiredBrushSize(TargetObject.Get(), DesiredBrushSize);return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(SNumericEntryBox<float>)
				/*.Value_Lambda([this](){return DesiredBrushSize;})
				.OnValueChanged_Lambda([this](float NewValue){DesiredBrushSize = NewValue;})
				.MinValue(0)
				.MaxValue(1)
				.MinSliderValue(0)
				.MaxSliderValue(1)*/
			]
			
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(STextBlock)
				/*.Text_Lambda([this]() {
					if (TargetObject.IsValid())
					{
						return FText::FromString("Value: " + FString::SanitizeFloat(IModioUIImageDisplayWidget::Execute_GetProgress(TargetObject.Get())));
					}
					else
					{
						return FText::FromString("No Target Object");
					}
				})*/
				
			]+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Reset Value Changed"))
				.OnClicked_Lambda([this](){ImageLoadStatus.Reset(); return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(STextBlock)
				.Text_Lambda([this]() {
					if (ImageLoadStatus.IsSet())
					{
						return FText::FromString( ImageLoadStatus.GetValue()? "Image Loaded OK" : "Image load failure");
					}
					else
					{
						return FText::FromString("Image Not Loaded");
					}
				})
			]
		];
		// clang-format on
	};