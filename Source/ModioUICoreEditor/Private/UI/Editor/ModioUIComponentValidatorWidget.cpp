// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Editor/ModioUIComponentValidatorWidget.h"
#include "DetailsViewArgs.h"
#include "IDetailsView.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "Components/DetailsView.h"
#include "Editor/ScriptableEditorWidgets/Public/Components/SinglePropertyView.h"
#include "UI/Editor/ModioUIInterfaceValidationWidgets.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Views/ITableRow.h"
#include "Widgets/Views/SListView.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioUIComponentValidatorWidget)

enum class EModioUIComponentValidationLineType
{
	Success,
	Error,
	Warning
};

struct FModioUIComponentValidationStatusLine
{
	FText Message;
	EModioUIComponentValidationLineType LineType;

	FModioUIComponentValidationStatusLine(FText Message, EModioUIComponentValidationLineType LineType)
		: Message(Message),
		  LineType(LineType) {};
};

static TSharedPtr<FModioUIComponentValidationStatusLine> MakeErrorLine(FText Message)
{
	return MakeShared<FModioUIComponentValidationStatusLine>(Message, EModioUIComponentValidationLineType::Error);
}

static TSharedPtr<FModioUIComponentValidationStatusLine> MakeSuccessLine(FText Message)
{
	return MakeShared<FModioUIComponentValidationStatusLine>(Message, EModioUIComponentValidationLineType::Success);
}

static TSharedPtr<FModioUIComponentValidationStatusLine> MakeWarningLine(FText Message)
{
	return MakeShared<FModioUIComponentValidationStatusLine>(Message, EModioUIComponentValidationLineType::Warning);
}

TSharedRef<ITableRow> UModioUIComponentValidatorWidget::GenerateStatusLineWidget(
	TSharedPtr<struct FModioUIComponentValidationStatusLine> Message,
	const TSharedRef<class STableViewBase>& OwningTable)
{
	auto GetMessageIcon = [](EModioUIComponentValidationLineType MessageType) {
		switch (MessageType)
		{
			case EModioUIComponentValidationLineType::Success:
				return SNew(SImage)
					.Image(FAppStyle::GetBrush("Symbols.Check"))
					.ColorAndOpacity(FSlateColor(FLinearColor::Green))
					.DesiredSizeOverride(FVector2D(16, 16));
				break;
			case EModioUIComponentValidationLineType::Error:
				return SNew(SImage)
					.Image(FAppStyle::GetBrush("Symbols.X"))
					.ColorAndOpacity(FSlateColor(FLinearColor::Red))
					.DesiredSizeOverride(FVector2D(16, 16));
				break;
			case EModioUIComponentValidationLineType::Warning:
				return SNew(SImage).Image(FAppStyle::GetBrush("Icons.Warning")).DesiredSizeOverride(FVector2D(16, 16));
				break;
			default:
				return SNew(SImage).DesiredSizeOverride(FVector2D(16, 16));
		}
	};
	TSharedRef<SWidget> ContentWidget = SNullWidget::NullWidget;
	if (Message)
	{
		// clang-format off
		ContentWidget= SNew(SGridPanel)
		.FillColumn(0, 1.f)
		+SGridPanel::Slot(0,0)
		[
			SNew(STextBlock).Text(Message->Message)
		]
		+ SGridPanel::Slot(1,0)
		.Padding(16,0,0,0)
		[
			GetMessageIcon(Message->LineType)	
		];
		// clang-format on
	}
	return SNew(STableRow<TSharedPtr<FModioUIComponentValidationStatusLine>>, OwningTable).Content()
		[
			ContentWidget
		];
}

TSharedRef<ITableRow> UModioUIComponentValidatorWidget::GenerateInterfaceTestWidget(
	UClass* InterfaceClass, const TSharedRef<class STableViewBase>& OwningTable, TObjectPtr<UObject> ImplementingWidget)
{
	if (InterfaceClass && ImplementingWidget)
	{
		return SNew(STableRow<UClass*>, OwningTable)
			.Content()
			[
				MakeInterfaceTestWidgetForClass(InterfaceClass, ImplementingWidget)
			];
	}
	return SNew(STableRow<UClass*>, OwningTable).Content()
		[
			SNullWidget::NullWidget
		];
}

void UModioUIComponentValidatorWidget::OnStatusWidgetDesiredVisibilityChanged(ECheckBoxState NewState)
{
	bShowValidationStatusWidget = (NewState == ECheckBoxState::Checked);
	InvalidateLayoutAndVolatility();
}

ECheckBoxState UModioUIComponentValidatorWidget::GetStatusWidgetDesiredVisibility() const
{
	return bShowValidationStatusWidget ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

EVisibility UModioUIComponentValidatorWidget::GetStatusWidgetVisibility() const
{
	return bShowValidationStatusWidget ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed;
}

#if WITH_EDITOR
void UModioUIComponentValidatorWidget::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	const FName PropertyName = PropertyChangedEvent.GetPropertyName();
	const bool ComponentArchetypeChanged = (PropertyName == GET_MEMBER_NAME_CHECKED(
		                                        UModioUIComponentValidatorWidget, ComponentToValidateAgainst));
	const bool WidgetClassChanged = (PropertyName == GET_MEMBER_NAME_CHECKED(UModioUIComponentValidatorWidget,
	                                                                         WidgetClassToValidate));

	if (ComponentArchetypeChanged || WidgetClassChanged)
	{
		BuildWidgetToValidate();
		BuildValidationStatusWidget();
		BuildInterfaceValidationControls(ComponentToValidateAgainst, WidgetToValidate);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(UModioUIComponentValidatorWidget, HorizontalAlignment))
	{
		WidgetToValidateSlot->SetHAlign(HorizontalAlignment);
	}
	else if (PropertyName == GET_MEMBER_NAME_CHECKED(UModioUIComponentValidatorWidget, VerticalAlignment))
	{
		WidgetToValidateSlot->SetVAlign(VerticalAlignment);
	}

	InvalidateLayoutAndVolatility();
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void UModioUIComponentValidatorWidget::LoadExampleWidgetClass()
{
	if (!ModioComponentExampleWidgets.Contains(ComponentToValidateAgainst))
	{
		return;
	}
	if (!IsValid(ModioComponentExampleWidgets[ComponentToValidateAgainst]))
	{
		return;
	}

	WidgetClassToValidate = ModioComponentExampleWidgets[ComponentToValidateAgainst];

	// Manually trigger property changed response
	FProperty* prop = FindFProperty<FProperty>(
		GetClass(), GET_MEMBER_NAME_CHECKED(UModioUIComponentValidatorWidget, WidgetClassToValidate));
	FPropertyChangedEvent event(prop, EPropertyChangeType::ValueSet);
	PostEditChangeProperty(event);
}

void UModioUIComponentValidatorWidget::BuildWidgetToValidate()
{
	FText FailureReason;
	if (ValidateConcreteWidget(FailureReason))
	{
		WidgetToValidate = NewObject<UWidget>(this, WidgetClassToValidate);
		DetailsView->SetObject(WidgetToValidate);
		CachedWidgetToValidateSlate = WidgetToValidate->TakeWidget();
		WidgetToValidateSlot->SetContent(CachedWidgetToValidateSlate.ToSharedRef());
	}
	else
	{
		DetailsView->SetObject(nullptr);
		WidgetToValidate = nullptr;
		WidgetToValidateSlot->SetContent(SAssignNew(CachedWidgetToValidateSlate, STextBlock).Text(FailureReason));
	}
}

void UModioUIComponentValidatorWidget::BuildValidationStatusWidget()
{
	TSharedRef<SWidget> StatusWidget =
		SAssignNew(CachedValidationStatusWidget, SListView<TSharedPtr<FModioUIComponentValidationStatusLine>>)
		.Orientation(Orient_Vertical)
		.ListItemsSource(&ValidationMessages)
		.OnGenerateRow_Static(&UModioUIComponentValidatorWidget::GenerateStatusLineWidget)
		.Visibility_UObject(this, &UModioUIComponentValidatorWidget::GetStatusWidgetVisibility);

	ValidationStatusWidgetSlot->SetContent(StatusWidget);
}

void UModioUIComponentValidatorWidget::BuildInterfaceValidationControls(EModioUIComponentID ComponentType,
                                                                        TObjectPtr<UObject> ImplementingObject)
{
	bool bFoundComponentMetadata;
	const FModioUIComponentMetadata& Metadata =
		UModioUIComponentStatics::GetMetadataForComponent(ComponentType, bFoundComponentMetadata);
	if (bFoundComponentMetadata && ImplementingObject)
	{
		InterfaceValidationControlsSlot->SetContent(SNew(SListView<UClass*>)
		.Orientation(Orient_Vertical)
		.ListItemsSource(&ObjectPtrDecay(Metadata.RequiredInterfaces))
		.OnGenerateRow_Static(&UModioUIComponentValidatorWidget::GenerateInterfaceTestWidget, ImplementingObject));
	}
	else
	{
		InterfaceValidationControlsSlot->SetContent(SNullWidget::NullWidget);
	}
}

bool UModioUIComponentValidatorWidget::ValidateConcreteWidget(FText& FailureReason)
{
	bValidationPassed = false;
	FailureReason = {};
	ValidationMessages.Empty();

	if (WidgetClassToValidate)
	{
		bool bFoundComponentMetadata;
		FModioUIComponentMetadata Metadata =
			UModioUIComponentStatics::GetMetadataForComponent(ComponentToValidateAgainst, bFoundComponentMetadata);
		if (bFoundComponentMetadata)
		{
			bool bMeetsComponentRequirements = true;
			for (const UClass* InterfaceClass : Metadata.RequiredInterfaces)
			{
				if (WidgetClassToValidate->ImplementsInterface(InterfaceClass))
				{
					ValidationMessages.Add(MakeSuccessLine(FText::FormatOrdered(
						FTextFormat::FromString("Implements {0}"), FText::FromName(InterfaceClass->GetFName()))));
				}
				else
				{
					bMeetsComponentRequirements = false;
					ValidationMessages.Add(
						MakeErrorLine(FText::FormatOrdered(FTextFormat::FromString("Does not implement {0}"),
						                                   FText::FromName(InterfaceClass->GetFName()))));
				}
			}
			if (bMeetsComponentRequirements)
			{
				bValidationPassed = true;
			}
			else
			{
				FailureReason = FText::FromString("Specified component failed validation, please check that you have "
					"implemented all the required interfaces.");
			}
		}
		else
		{
			FailureReason =
				FText::FromString("Please ensure component metadata for the specified component type is set in "
					"ModioUIComponentStatics");
		}
	}
	else
	{
		FailureReason = FText::FromString("Please set a widget class to validate.");
	}
	if (CachedValidationStatusWidget)
	{
		CachedValidationStatusWidget->RebuildList();
	}
	return bValidationPassed;
}

TSharedRef<SWidget> UModioUIComponentValidatorWidget::RebuildWidget()
{
	// clang-format off
	RootSlateWidget = SNew(SSplitter) 
	.Orientation(Orient_Vertical)
	+SSplitter::Slot().MinSize(300).Value(20)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.FillHeight(1)
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SAssignNew(WidgetToValidateSlot, SBox)
				.HAlign(HorizontalAlignment)
				.VAlign(VerticalAlignment)
			]
			+SOverlay::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Top)
			.Padding(32)
			[
				SAssignNew(ValidationStatusWidgetSlot, SBox)
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Right)
		.Padding(0,0,8,0)
		[
			SNew(SCheckBox)
			.OnCheckStateChanged_UObject(this, &UModioUIComponentValidatorWidget::OnStatusWidgetDesiredVisibilityChanged)
			.IsChecked_UObject(this, &UModioUIComponentValidatorWidget::GetStatusWidgetDesiredVisibility)
			.Content()
			[
				SNew(STextBlock).Text(FText::FromString("Display Validation Status Overlay"))
			]
		]
	]
	+SSplitter::Slot().MinSize(300).Value(20)
	[
		SNew(SSplitter)
		.Orientation(Orient_Horizontal)
		+SSplitter::Slot()
		.MinSize(300)
		[
			SAssignNew(InterfaceValidationControlsSlot, SBox)
		]
		+SSplitter::Slot()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				RebuildComponentPropertyView()
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				RebuildWidgetClassPropertyView()
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				RebuildHAlignmentPropertyView()
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				RebuildVAlignmentPropertyView()
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SButton)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.OnClicked_Lambda([this]() {
					LoadExampleWidgetClass();
					return FReply::Handled();
				})
				[
					SNew(STextBlock)
					.Text(FText::FromString("Load Example Widget Class for Component"))
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				CustomValidationControls ? CustomValidationControls->TakeWidget() : SNullWidget::NullWidget
			]
			+SVerticalBox::Slot()
			[
				RebuildDetailsWidget()
			]
		]
	];
	// clang-format on

	BuildWidgetToValidate();
	return RootSlateWidget.ToSharedRef();
}

TSharedRef<SWidget> UModioUIComponentValidatorWidget::RebuildWidgetClassPropertyView()
{
	WidgetClassPropertyView = NewObject<USinglePropertyView>(this);
	WidgetClassPropertyView->SetPropertyName("WidgetClassToValidate");
	WidgetClassPropertyView->SetObject(this);

	return WidgetClassPropertyView->TakeWidget();
}

TSharedRef<SWidget> UModioUIComponentValidatorWidget::RebuildComponentPropertyView()
{
	ComponentPropertyView = NewObject<USinglePropertyView>(this);
	ComponentPropertyView->SetPropertyName("ComponentToValidateAgainst");
	ComponentPropertyView->SetObject(this);

	return ComponentPropertyView->TakeWidget();
}

TSharedRef<SWidget> UModioUIComponentValidatorWidget::RebuildHAlignmentPropertyView()
{
	HAlignmentPropertyView = NewObject<USinglePropertyView>(this);
	HAlignmentPropertyView->SetPropertyName("HorizontalAlignment");
	HAlignmentPropertyView->SetObject(this);

	return HAlignmentPropertyView->TakeWidget();
}

TSharedRef<SWidget> UModioUIComponentValidatorWidget::RebuildVAlignmentPropertyView()
{
	VAlignmentPropertyView = NewObject<USinglePropertyView>(this);
	VAlignmentPropertyView->SetPropertyName("VerticalAlignment");
	VAlignmentPropertyView->SetObject(this);

	return VAlignmentPropertyView->TakeWidget();
}

TSharedRef<SWidget> UModioUIComponentValidatorWidget::RebuildDetailsWidget()
{
	DetailsView = NewObject<UDetailsView>(this);
	DetailsView->bAllowFiltering = false;
	DetailsView->SetObject(WidgetToValidate);
	CachedDetailsViewSlate = DetailsView->TakeWidget();
	return CachedDetailsViewSlate.ToSharedRef();
}

void UModioUIComponentValidatorWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	WidgetToValidate = nullptr;
	RootSlateWidget.Reset();
	CachedValidationStatusWidget.Reset();
	CachedWidgetToValidateSlate.Reset();
	CachedDetailsViewSlate.Reset();
	WidgetToValidateSlot.Reset();
	ValidationStatusWidgetSlot.Reset();
	InterfaceValidationControlsSlot.Reset();
}