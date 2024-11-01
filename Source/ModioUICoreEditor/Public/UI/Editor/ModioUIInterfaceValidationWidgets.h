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
#include "UObject/StrongObjectPtr.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"

#include "ModioUIInterfaceValidationWidgets.generated.h"

template<typename WidgetType>
TWeakPtr<WidgetType> MakeWeakThis(WidgetType* Widget)
{
	return StaticCastWeakPtr<WidgetType>(Widget->AsWeak());
}

UCLASS()
class UModioUIClickableBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_OneParam(FBridgedDelegate, UObject*);

	UFUNCTION()
	void HandleClick(UObject* ClickContext)
	{
		OnClicked.ExecuteIfBound(ClickContext);
	}

public:
	FBridgedDelegate OnClicked;
};

class SModioUIClickableTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIClickableTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUIClickableBridge> DelegateBridge;

	FModioClickableOnClicked ClickListener;
	bool bClicked;

	void HandleClick(UObject* ClickContext)
	{
		bClicked = true;
	}
	void ResetClick()
	{
		bClicked = false;
	}
	bool bHandlerAdded = false;
	bool bClickEnabled = true;
	FText GetHandlerButtonLabel() const
	{
		return FText::FromString(bHandlerAdded ? "Remove Click Handler" : "Add Click Handler");
	}
	FText GetClickEventButtonLabel() const
	{
		return FText::FromString(bClickEnabled ? "Disable Click Events" : "Enable Click Events");
	}

	void ToggleHandler()
	{
		if (TargetObject.IsValid())
		{
			if (bHandlerAdded)
			{
				IModioUIClickableWidget::Execute_RemoveClickedHandler(TargetObject.Get(), ClickListener);
			}
			else
			{
				IModioUIClickableWidget::Execute_AddClickedHandler(TargetObject.Get(), ClickListener);
			}
			bHandlerAdded = !bHandlerAdded;
		}
	}

	void ToggleEvents()
	{
		if (TargetObject.IsValid())
		{
			if (bClickEnabled)
			{
				IModioUIClickableWidget::Execute_DisableClick(TargetObject.Get());
			}
			else
			{
				IModioUIClickableWidget::Execute_EnableClick(TargetObject.Get());
			}
			bClickEnabled = !bClickEnabled;
		}
	}

	void Construct(const FArguments& InArgs)
	{
		TargetObject = InArgs._TargetObject.Get();
		DelegateBridge = TStrongObjectPtr<UModioUIClickableBridge>(NewObject<UModioUIClickableBridge>());
		DelegateBridge->OnClicked.BindSP(this, &SModioUIClickableTestWidget::HandleClick);
		ClickListener.BindUFunction(DelegateBridge.Get(), FName("HandleClick"));

		IModioUIClickableWidget::Execute_EnableClick(TargetObject.Get());

		// clang-format off
		ChildSlot
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
				.Text_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return GetHandlerButtonLabel();} return FText{};})
				.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ToggleHandler();} return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
				.Text_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return GetClickEventButtonLabel();} return FText{};})
				.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ToggleEvents();} return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
				.Text(FText::FromString("Reset Clicked State"))
				.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ResetClick();} return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(STextBlock)
				.Text_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){return FText::FromString(bClicked? "Clicked!": "Not Clicked");} return FText{};})
			]

		];
		// clang-format on
	};
};

UCLASS()
class UModioUISelectableBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_TwoParams(FBridgedDelegate, UObject*, bool);

	UFUNCTION()
	void HandleSelectionChanged(UObject* SelectionContext, bool bNewState)
	{
		OnSelectionChanged.ExecuteIfBound(SelectionContext, bNewState);
	}

public:
	FBridgedDelegate OnSelectionChanged;
};

class SModioUISelectableTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUISelectableTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUISelectableBridge> DelegateBridge;

	FModioSelectableOnSelectionChanged OnSelectionChanged;
	bool bSelected;

	void HandleSelectionChanged(UObject* SelectionContext, bool bNewState)
	{
		bSelected = bNewState;
	}
	void ResetSelected()
	{
		bSelected = false;
		IModioUISelectableWidget::Execute_SetSelectedState(TargetObject.Get(), false);
	}
	bool bHandlerAdded = false;
	bool bSelectionEnabled = true;
	bool bToggleableEnabled = true;
	FText GetHandlerButtonLabel() const
	{
		return FText::FromString(bHandlerAdded ? "Remove Selection Handler" : "Add Selection Handler");
	}
	FText GetSelectionEventButtonLabel() const
	{
		return FText::FromString(bSelectionEnabled ? "Disable Selectable" : "Enable Selectable");
	}
	FText GetToggleableButtonLabel() const
	{
		return FText::FromString(bToggleableEnabled ? "Disable Toggleable" : "Enable Toggleable");
	}
	void ToggleHandler()
	{
		if (TargetObject.IsValid())
		{
			if (bHandlerAdded)
			{
				IModioUISelectableWidget::Execute_RemoveSelectedStateChangedHandler(TargetObject.Get(),
																					OnSelectionChanged);
			}
			else
			{
				IModioUISelectableWidget::Execute_AddSelectedStateChangedHandler(TargetObject.Get(),
																				 OnSelectionChanged);
			}
			bHandlerAdded = !bHandlerAdded;
		}
	}

	void ToggleEvents()
	{
		if (TargetObject.IsValid())
		{
			if (bSelectionEnabled)
			{
				IModioUISelectableWidget::Execute_SetSelectable(TargetObject.Get(), false);
			}
			else
			{
				IModioUISelectableWidget::Execute_SetSelectable(TargetObject.Get(), true);
			}
			bSelectionEnabled = !bSelectionEnabled;
		}
	}
	void ToggleToggleable()
	{
		if (TargetObject.IsValid())
		{
			if (bToggleableEnabled)
			{
				IModioUISelectableWidget::Execute_SetToggleable(TargetObject.Get(), false);
			}
			else
			{
				IModioUISelectableWidget::Execute_SetToggleable(TargetObject.Get(), true);
			}
			bToggleableEnabled = !bToggleableEnabled;
		}
	}
	void Construct(const FArguments& InArgs)
	{
		TargetObject = InArgs._TargetObject.Get();
		DelegateBridge = TStrongObjectPtr<UModioUISelectableBridge>(NewObject<UModioUISelectableBridge>());
		DelegateBridge->OnSelectionChanged.BindSP(this, &SModioUISelectableTestWidget::HandleSelectionChanged);
		OnSelectionChanged.BindUFunction(DelegateBridge.Get(), FName("HandleSelectionChanged"));

		IModioUISelectableWidget::Execute_SetSelectable(TargetObject.Get(), true);
		IModioUISelectableWidget::Execute_SetToggleable(TargetObject.Get(), true);

		// clang-format off
		ChildSlot
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
				.Text_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return GetHandlerButtonLabel();} return FText{};})
				.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ToggleHandler();} return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
				.Text_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return GetSelectionEventButtonLabel();} return FText{};})
				.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ToggleEvents();} return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
				.Text_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return GetToggleableButtonLabel();} return FText{};})
				.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ToggleToggleable();} return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
				.Text(FText::FromString("Reset Selected State"))
				.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ResetSelected();} return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(STextBlock)
				.Text_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){return FText::FromString(bSelected? "Selected!": "Not Selected");} return FText{};})
			]

		];
		// clang-format on
	};
};

class SModioUIHasTextTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIHasTextTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;
	TSharedPtr<SEditableTextBox> InputText;
	TSharedPtr<STextBlock> OutputText;

	void SetTextOnTarget()
	{
		if (InputText)
		{
			IModioUIHasTextWidget::Execute_SetWidgetText(TargetObject.Get(), InputText->GetText());
		}
	}
	void GetTextFromTarget()
	{
		if (TargetObject.IsValid() && OutputText.IsValid())
		{
			OutputText->SetText(IModioUIHasTextWidget::Execute_GetWidgetText(TargetObject.Get()));

			// will need something here later to trigger a ForceRefresh on the details view observing the object
		}
	}

	void Construct(const FArguments& InArgs)
	{
		TargetObject = InArgs._TargetObject.Get();

		// clang-format off
		ChildSlot
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Set Text"))
				.OnClicked_Lambda([this](){SetTextOnTarget(); return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SAssignNew(InputText, SEditableTextBox)
				.MinDesiredWidth(200)
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Get Text"))
				.OnClicked_Lambda([this](){GetTextFromTarget(); return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SAssignNew(OutputText, STextBlock)
				.Text(FText::FromString("Widget text here"))
			]
		];
		// clang-format on
	};
};

class SModioUIStringInputTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIStringInputTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;
	TSharedPtr<SEditableTextBox> InputText;
	TSharedPtr<STextBlock> OutputText;

	void SetInputOnTarget()
	{
		if (InputText)
		{
			IModioUIStringInputWidget::Execute_SetInput(TargetObject.Get(), InputText->GetText().ToString());
		}
	}
	void GatherInputFromTarget()
	{
		if (TargetObject.IsValid() && OutputText.IsValid())
		{
			OutputText->SetText(FText::FromString(IModioUIStringInputWidget::Execute_GatherInput(TargetObject.Get())));

			// will need something here later to trigger a ForceRefresh on the details view observing the object
		}
	}

	void Construct(const FArguments& InArgs)
	{
		TargetObject = InArgs._TargetObject.Get();

		// clang-format off
		ChildSlot
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Set Input"))
				.OnClicked_Lambda([this](){SetInputOnTarget(); return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SAssignNew(InputText, SEditableTextBox)
				.MinDesiredWidth(200)
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Gather Input"))
				.OnClicked_Lambda([this](){GatherInputFromTarget(); return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SAssignNew(OutputText, STextBlock)
				.Text(FText::FromString("Widget text here"))
			]
		];
		// clang-format on
	}
};

UCLASS()
class UModioUIHoverableBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_TwoParams(FBridgedDelegate, UObject*, bool);

	UFUNCTION()
	void HandleHovered(UObject* HoverContext, bool bNewState)
	{
		OnHoverStateChanged.ExecuteIfBound(HoverContext, bNewState);
	}

public:
	FBridgedDelegate OnHoverStateChanged;
};

class SModioUIHoverableTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIHoverableTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUIHoverableBridge> DelegateBridge;

	FModioHoverableOnHoverStateChanged HoverListener;
	bool bHovered;

	void HandleHovered(UObject* ClickContext, bool bNewState)
	{
		bHovered = bNewState;
	}
	bool bHandlerAdded = false;
	bool bHoverEnabled = true;
	FText GetHandlerButtonLabel() const
	{
		return FText::FromString(bHandlerAdded ? "Remove Hover Handler" : "Add Hover Handler");
	}
	FText GetClickEventButtonLabel() const
	{
		return FText::FromString(bHoverEnabled ? "Disable Hover Events" : "Enable Hover Events");
	}

	void ToggleHandler()
	{
		if (TargetObject.IsValid())
		{
			if (bHandlerAdded)
			{
				IModioUIHoverableWidget::Execute_RemoveHoverStateChangedHandler(TargetObject.Get(), HoverListener);
			}
			else
			{
				IModioUIHoverableWidget::Execute_AddHoverStateChangedHandler(TargetObject.Get(), HoverListener);
			}
			bHandlerAdded = !bHandlerAdded;
		}
	}

	void ToggleEvents()
	{
		if (TargetObject.IsValid())
		{
			if (bHoverEnabled)
			{
				IModioUIHoverableWidget::Execute_DisableHoverEvents(TargetObject.Get());
			}
			else
			{
				IModioUIHoverableWidget::Execute_EnableHoverEvents(TargetObject.Get());
			}
			bHoverEnabled = !bHoverEnabled;
		}
	}

	void Construct(const FArguments& InArgs)
	{
		TargetObject = InArgs._TargetObject.Get();
		DelegateBridge = TStrongObjectPtr<UModioUIHoverableBridge>(NewObject<UModioUIHoverableBridge>());
		DelegateBridge->OnHoverStateChanged.BindSP(this, &SModioUIHoverableTestWidget::HandleHovered);
		HoverListener.BindUFunction(DelegateBridge.Get(), FName("HandleHovered"));

		IModioUIHoverableWidget::Execute_EnableHoverEvents(TargetObject.Get());

		// clang-format off
		ChildSlot
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
				.Text_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return GetHandlerButtonLabel();} return FText{};})
				.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ToggleHandler();} return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return TargetObject != nullptr;} return false;})
				.Text_Lambda([WeakThis = MakeWeakThis(this),this](){if (WeakThis.IsValid()){return GetClickEventButtonLabel();} return FText{};})
				.OnClicked_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){ToggleEvents();} return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(STextBlock)
				.Text_Lambda([WeakThis = MakeWeakThis(this), this](){if (WeakThis.IsValid()){return FText::FromString(bHovered? "Hovered!": "Not Hovered");} return FText{};})
			]

		];
		// clang-format on
	};
};

UCLASS()
class UModioUIObjectSelectorBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_OneParam(FBridgedDelegate, UObject*);

	UFUNCTION()
	void HandleSelectionChanged(UObject* NewSelection)
	{
		OnSelectionChanged.ExecuteIfBound(NewSelection);
	}

public:
	FBridgedDelegate OnSelectionChanged;
};

class SModioUIObjectSelectorTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIObjectSelectorTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUIObjectSelectorBridge> DelegateBridge;

	TSharedPtr<SNumericEntryBox<int32>> DesiredIndex;

	int32 DesiredIndexValue;

	FModioOnObjectSelectionChanged SelectionListener;
	bool bSelected;

	virtual void HandleSelectionChanged(UObject* NewSelection)
	{
		bSelected = (NewSelection != nullptr);
	}
	void ClearSelection()
	{
		if (TargetObject.IsValid())
		{
			IModioUIObjectSelector::Execute_ClearSelectedValues(TargetObject.Get());
		}
	}
	void SetSelectionFromIndex()
	{
		if (TargetObject.IsValid())
		{
			if (DesiredIndexValue >= 0)
			{
				IModioUIObjectSelector::Execute_SetSingleSelectionByIndex(TargetObject.Get(), DesiredIndexValue, true);
			}
		}
	}
	void Construct(const FArguments& InArgs)
	{
		TargetObject = InArgs._TargetObject.Get();
		DelegateBridge = TStrongObjectPtr<UModioUIObjectSelectorBridge>(NewObject<UModioUIObjectSelectorBridge>());
		DelegateBridge->OnSelectionChanged.BindSP(this, &SModioUIObjectSelectorTestWidget::HandleSelectionChanged);
		SelectionListener.BindUFunction(DelegateBridge.Get(), FName("HandleSelectionChanged"));

		IModioUIObjectSelector::Execute_AddSelectionChangedHandler(TargetObject.Get(), SelectionListener);

		// clang-format off
		ChildSlot
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Clear Selection"))
				.OnClicked_Lambda([ this](){ ClearSelection();return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Set Selected Index"))
				.OnClicked_Lambda([ this](){ IModioUIObjectSelector::Execute_SetSingleSelectionByIndex(TargetObject.Get(), DesiredIndexValue, true); return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SAssignNew(DesiredIndex, SNumericEntryBox<int32>)
				.Value_Lambda([this](){return DesiredIndexValue;})
				.OnValueChanged_Lambda([this](int32 NewValue){DesiredIndexValue = NewValue;})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(STextBlock)
				.Text_Lambda([ this](){return FText::FromString(bSelected? "Selected": "Not Selected");})
			]
		];
		// clang-format on
	};
};

UCLASS()
class UModioUIProgressBarBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_OneParam(FBridgedDelegate, float);

	UFUNCTION()
	void HandleValueChanged(float NewValue)
	{
		OnValuechanged.ExecuteIfBound(NewValue);
	}

public:
	FBridgedDelegate OnValuechanged;
};

class SModioUIProgressBarTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIProgressBarTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUIProgressBarBridge> DelegateBridge;

	TSharedPtr<SNumericEntryBox<float>> DesiredIndex;

	float DesiredFloatValue;

	FModioProgressWidgetValueChanged ValueChangedListener;
	float CurrentValue = 0.1f;

	bool bValueChanged = false;
	virtual void HandleValueChanged(float NewValue)
	{
		CurrentValue = NewValue;
		bValueChanged = true;
	}

	void Construct(const FArguments& InArgs)
	{
		TargetObject = InArgs._TargetObject.Get();
		DelegateBridge = TStrongObjectPtr<UModioUIProgressBarBridge>(NewObject<UModioUIProgressBarBridge>());
		DelegateBridge->OnValuechanged.BindSP(this, &SModioUIProgressBarTestWidget::HandleValueChanged);
		ValueChangedListener.BindUFunction(DelegateBridge.Get(), FName("HandleValueChanged"));

		IModioUIProgressWidget::Execute_AddValueChangedHandler(TargetObject.Get(), ValueChangedListener);

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
				.TrueText(FText::FromString("Disable Value Changed Handler"))
				.FalseText(FText::FromString("Enable Value Changed Handler"))
				.ClickedWhenTrue_Lambda([this](){IModioUIProgressWidget::Execute_RemoveValueChangedHandler(TargetObject.Get(), ValueChangedListener);})
				.ClickedWhenFalse_Lambda([this](){IModioUIProgressWidget::Execute_AddValueChangedHandler(TargetObject.Get(), ValueChangedListener);})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(8,0)
			[
				SNew(SModioToggleButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.TrueText(FText::FromString("Disable Marquee"))
				.FalseText(FText::FromString("Enable Marquee"))
				.ClickedWhenTrue_Lambda([this](){IModioUIProgressWidget::Execute_SetMarquee(TargetObject.Get(), false);})
				.ClickedWhenFalse_Lambda([this](){IModioUIProgressWidget::Execute_SetMarquee(TargetObject.Get(), true);})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Set Progress"))
				.OnClicked_Lambda([this](){IModioUIProgressWidget::Execute_SetProgress(TargetObject.Get(), DesiredFloatValue);return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SAssignNew(DesiredIndex, SNumericEntryBox<float>)
				.Value_Lambda([this](){return DesiredFloatValue;})
				.OnValueChanged_Lambda([this](float NewValue){DesiredFloatValue = NewValue;})
				.MinValue(0)
				.MaxValue(1)
				.MinSliderValue(0)
				.MaxSliderValue(1)
			]
			
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(STextBlock)
					.Text_Lambda([this]() {
					if (TargetObject.IsValid())
					{
						return FText::FromString("Value: " + FString::SanitizeFloat(IModioUIProgressWidget::Execute_GetProgress(TargetObject.Get())));
					}
					else
					{
						return FText::FromString("No Target Object");
					}
					})
				
			]+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(SButton)
				.IsEnabled_Lambda([this](){return TargetObject != nullptr;})
				.Text(FText::FromString("Reset Value Changed"))
				.OnClicked_Lambda([this](){bValueChanged = false; return FReply::Handled();})
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.Padding(8,0)
			[
				SNew(STextBlock)
					.Text_Lambda([this]() {
						return FText::FromString(bValueChanged? "Value Changed" : "Value not Changed");
					})
				
			]
		];
		// clang-format on
	};
};

UCLASS()
class UModioUIImageDisplayBridge : public UObject
{
	GENERATED_BODY()
	DECLARE_DELEGATE_TwoParams(FBridgedDelegate, UObject*, bool);

	UFUNCTION()
	void HandleImageLoaded(UObject* LoadContext, bool bLoadSuccess)
	{
		OnValuechanged.ExecuteIfBound(LoadContext, bLoadSuccess);
	}

public:
	FBridgedDelegate OnValuechanged;
};

class SModioUIImageDisplayTestWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SModioUIImageDisplayTestWidget) : _TargetObject(nullptr) {}

	SLATE_ATTRIBUTE(TObjectPtr<UObject>, TargetObject)

	SLATE_END_ARGS()

	TWeakObjectPtr<UObject> TargetObject;

	TStrongObjectPtr<UModioUIImageDisplayBridge> DelegateBridge;

	FModioImageDisplayOnLoadStateChanged ImageLoadedListener;

	FVector2D DesiredBrushSize = {32, 32};

	TOptional<bool> ImageLoadStatus;

	virtual void HandleImageLoaded(UObject* LoadContext, bool bLoadSuccess)
	{
		ImageLoadStatus = bLoadSuccess;
	}

	void Construct(const FArguments& InArgs)
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
};

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
	else
	{
		return SNullWidget::NullWidget;
	}
}