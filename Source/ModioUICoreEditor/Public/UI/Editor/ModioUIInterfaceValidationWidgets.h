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

#include "Templates/SharedPointer.h"
#include "UI/Editor/SModioToggleButton.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUIHoverableWidget.h"
#include "UI/Interfaces/IModioUIImageDisplayWidget.h"
#include "UI/Interfaces/IModioUIObjectSelector.h"
#include "UI/Interfaces/IModioUIProgressWidget.h"
#include "UI/Interfaces/IModioUISelectableWidget.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"
#include "ComponentTestWidgets/SModioUIClickableTestWidget.h"
#include "ComponentTestWidgets/SModioUICommandMenuTestWidget.h"
#include "ComponentTestWidgets/SModioUIHasTextTestWidget.h"
#include "ComponentTestWidgets/SModioUIHasTooltipTestWidget.h"
#include "ComponentTestWidgets/SModioUIHoverableTestWidget.h"
#include "ComponentTestWidgets/SModioUIImageDisplayTestWidget.h"
#include "ComponentTestWidgets/SModioUIObjectSelectorTestWidget.h"
#include "ComponentTestWidgets/SModioUIProgressBarTestWidget.h"
#include "ComponentTestWidgets/SModioUISelectableTestWidget.h"
#include "ComponentTestWidgets/SModioUIStringInputTestWidget.h"
#include "ComponentTestWidgets/SModioUITextValidatorTestWidget.h"
#include "UI/Interfaces/IModioUICommandMenu.h"
#include "UI/Interfaces/IModioUIDialog.h"
#include "UI/Interfaces/IModioUIHasTooltipWidget.h"
#include "UI/Interfaces/IModioUIModListViewInterface.h"
#include "UI/Interfaces/IModioUIObjectListWidget.h"
#include "UI/Interfaces/IModioUITextValidator.h"
#include "UObject/StrongObjectPtr.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"

inline TSharedRef<SWidget> MakeInterfaceTestWidgetForClass(UClass* InterfaceClass,
														   TObjectPtr<UObject> ImplementingWidget)
{
	if (InterfaceClass == UModioUIClickableWidget::StaticClass())
	{
		return SNew(SModioUIClickableTestWidget).TargetObject(ImplementingWidget);
	}
	if (InterfaceClass == UModioUISelectableWidget::StaticClass())
	{
		return SNew(SModioUISelectableTestWidget).TargetObject(ImplementingWidget);
	}
	if (InterfaceClass == UModioUIHasTextWidget::StaticClass())
	{
		return SNew(SModioUIHasTextTestWidget).TargetObject(ImplementingWidget);
	}
	if (InterfaceClass == UModioUIStringInputWidget::StaticClass())
	{
		return SNew(SModioUIStringInputTestWidget).TargetObject(ImplementingWidget);
	}
	if (InterfaceClass == UModioUIHoverableWidget::StaticClass())
	{
		return SNew(SModioUIHoverableTestWidget).TargetObject(ImplementingWidget);
	}
	if (InterfaceClass == UModioUIObjectSelector::StaticClass())
	{
		return SNew(SModioUIObjectSelectorTestWidget).TargetObject(ImplementingWidget);
	}
	if (InterfaceClass == UModioUIImageDisplayWidget::StaticClass())
	{
		return SNew(SModioUIImageDisplayTestWidget).TargetObject(ImplementingWidget);
	}
	if (InterfaceClass == UModioUIProgressWidget::StaticClass())
	{
		return SNew(SModioUIProgressBarTestWidget).TargetObject(ImplementingWidget);
	}
	if (InterfaceClass == UModioUIHasTooltipWidget::StaticClass())
	{
		return SNew(SModioUIHasTooltipTestWidget).TargetObject(ImplementingWidget);
	}
	if (InterfaceClass == UModioUITextValidator::StaticClass())
	{
		return SNew(SModioUITextValidatorTestWidget).TargetObject(ImplementingWidget);
	}
	if (InterfaceClass == UModioUICommandMenu::StaticClass())
	{
		return SNew(SModioUICommandMenuTestWidget).TargetObject(ImplementingWidget);
	}
	else
	{
		return SNullWidget::NullWidget;
	}
}