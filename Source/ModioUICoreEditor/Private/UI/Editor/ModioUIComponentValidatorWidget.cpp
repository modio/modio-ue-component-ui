// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Editor/ModioUIComponentValidatorWidget.h"
#include "DetailsViewArgs.h"
#include "IDetailsView.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "UI/Editor/ModioUIInterfaceValidationWidgets.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Views/ITableRow.h"
#include "Widgets/Views/SListView.h"

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
	return SNew(STableRow<TSharedPtr<FModioUIComponentValidationStatusLine>>, OwningTable).Content()[ContentWidget];
}

TSharedRef<ITableRow> UModioUIComponentValidatorWidget::GenerateInterfaceTestWidget(
	UClass* InterfaceClass, const TSharedRef<class STableViewBase>& OwningTable, TObjectPtr<UObject> ImplementingWidget)
{
	if (InterfaceClass && ImplementingWidget)
	{
		return SNew(STableRow<UClass*>, OwningTable)
			.Content()[MakeInterfaceTestWidgetForClass(InterfaceClass, ImplementingWidget)];
	}
	else
	{
		return SNew(STableRow<UClass*>, OwningTable).Content()[SNullWidget::NullWidget];
	}
}

void UModioUIComponentValidatorWidget::OnStatusWidgetDesiredVisibilityChanged(ECheckBoxState NewState)
{
	bShowValidationStatusWidget = (NewState == ECheckBoxState::Checked);
	InvalidateLayoutAndVolatility();
	/*if (CachedValidationStatusWidget)
	{
		CachedValidationStatusWidget->SetVisibility(GetStatusWidgetVisibility());
		CachedValidationStatusWidget->Invalidate(EInvalidateWidgetReason::Visibility);
	}*/
}

ECheckBoxState UModioUIComponentValidatorWidget::GetStatusWidgetDesiredVisibility() const
{
	return bShowValidationStatusWidget ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

EVisibility UModioUIComponentValidatorWidget::GetStatusWidgetVisibility() const
{
	return bShowValidationStatusWidget ? EVisibility::SelfHitTestInvisible : EVisibility::Collapsed;
}

void UModioUIComponentValidatorWidget::OnComponentPropertyChanged(const FPropertyChangedEvent& Event)
{
	if (ConcreteManagedWidget)
	{
		ConcreteManagedWidget->InvalidateLayoutAndVolatility();
		InvalidateLayoutAndVolatility();
	}
}

bool UModioUIComponentValidatorWidget::ValidateConcreteWidget(FText& FailureReason)
{
	bValidationPassed = false;
	FailureReason = {};
	ValidationMessages.Empty();

	if (ConcreteComponentClassToValidate)
	{
		bool bFoundComponentMetadata;
		FModioUIComponentMetadata Metadata =
			UModioUIComponentStatics::GetMetadataForComponent(ComponentToValidateAgainst, bFoundComponentMetadata);
		if (bFoundComponentMetadata)
		{
			bool bMeetsComponentRequirements = true;
			for (const UClass* InterfaceClass : Metadata.RequiredInterfaces)
			{
				if (ConcreteComponentClassToValidate->ImplementsInterface(InterfaceClass))
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
	.Orientation(EOrientation::Orient_Vertical)
	+SSplitter::Slot().MinSize(300).Value(20)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.FillHeight(1)
		[
			SNew(SOverlay)
			+SOverlay::Slot()
			.HAlign(TargetWidgetHAlign)
			.VAlign(TargetWidgetVAlign)
			[
				RebuildConcreteWidget()
			]
			+SOverlay::Slot()
			.HAlign(EHorizontalAlignment::HAlign_Right)
			.VAlign(EVerticalAlignment::VAlign_Top)
			.Padding(32)
			[
				RebuildValidationStatusWidget()
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(EHorizontalAlignment::HAlign_Right)
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
		.Orientation(EOrientation::Orient_Horizontal)
		+SSplitter::Slot()
		.MinSize(300)
		[
			RebuildInterfaceValidationControls(ComponentToValidateAgainst, ConcreteManagedWidget) 
		]
		+SSplitter::Slot()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				CustomValidationControls? CustomValidationControls->TakeWidget() : SNullWidget::NullWidget
			]
			+SVerticalBox::Slot()
			[
				RebuildDetailsWidget()
			]
		]
	];
	// clang-format on
	return RootSlateWidget.ToSharedRef();
}

TSharedRef<SWidget> UModioUIComponentValidatorWidget::RebuildConcreteWidget()
{
	FText FailureReason;
	if (ValidateConcreteWidget(FailureReason))
	{
		if (!ConcreteManagedWidget || (ConcreteManagedWidget->GetClass() != ConcreteComponentClassToValidate))
		{
			ConcreteManagedWidget = NewObject<UWidget>(this, ConcreteComponentClassToValidate);
		}

		if (ComponentDetailsWidget)
		{
			ComponentDetailsWidget->SetObject(ConcreteManagedWidget);
		}

		CachedConcreteSlateWidget = ConcreteManagedWidget->TakeWidget();
		return CachedConcreteSlateWidget.ToSharedRef();
	}
	else
	{
		if (ComponentDetailsWidget)
		{
			ComponentDetailsWidget->SetObject(nullptr);
		}
		return SAssignNew(CachedConcreteSlateWidget, STextBlock).Text(FailureReason);
	}
}

TSharedRef<SWidget> UModioUIComponentValidatorWidget::RebuildValidationStatusWidget()
{
	TSharedRef<SWidget> RebuiltWidget =
		SAssignNew(CachedValidationStatusWidget, SListView<TSharedPtr<FModioUIComponentValidationStatusLine>>)
			.Orientation(EOrientation::Orient_Vertical)
			.ListItemsSource(&ValidationMessages)
			.OnGenerateRow_Static(&UModioUIComponentValidatorWidget::GenerateStatusLineWidget)
			.Visibility_UObject(this, &UModioUIComponentValidatorWidget::GetStatusWidgetVisibility);
	return RebuiltWidget;
}

TSharedRef<SWidget> UModioUIComponentValidatorWidget::RebuildDetailsWidget()
{
	FPropertyEditorModule& PropertyEditorModule =
		FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bHideSelectionTip = true;
	DetailsViewArgs.bAllowSearch = false;
	DetailsViewArgs.DefaultsOnlyVisibility = EEditDefaultsOnlyNodeVisibility::Hide;
	TSharedPtr<IDetailsView> ComponentDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	ComponentDetailsView->SetObject(ConcreteManagedWidget);
	ComponentDetailsView->OnFinishedChangingProperties().AddUObject(
		this, &UModioUIComponentValidatorWidget::OnComponentPropertyChanged);
	ComponentDetailsWidget = ComponentDetailsView;
	return ComponentDetailsWidget.ToSharedRef();
}

TSharedRef<SWidget> UModioUIComponentValidatorWidget::RebuildInterfaceValidationControls(
	EModioUIComponentID ComponentType, TObjectPtr<UObject> ImplementingObject)
{
	bool bFoundComponentMetadata;
	const FModioUIComponentMetadata& Metadata =
		UModioUIComponentStatics::GetMetadataForComponent(ComponentType, bFoundComponentMetadata);
	if (bFoundComponentMetadata)
	{
		return SNew(SListView<UClass*>)
			.Orientation(EOrientation::Orient_Vertical)
			.ListItemsSource(&Metadata.RequiredInterfaces)
			.OnGenerateRow_Static(&UModioUIComponentValidatorWidget::GenerateInterfaceTestWidget, ImplementingObject);
	}
	else
	{
		return SNullWidget::NullWidget;
	}
}

void UModioUIComponentValidatorWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	RebuildWidget();
}

void UModioUIComponentValidatorWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	ConcreteManagedWidget = nullptr;
	CachedConcreteSlateWidget.Reset();
	RootSlateWidget.Reset();
	CachedValidationStatusWidget.Reset();
}

UWidget* UModioUIComponentValidatorWidget::GetWidgetBeingValidated() const
{
	return ConcreteManagedWidget;
}

void UModioUIComponentValidatorWidget::GetSlotNames(TArray<FName>& SlotNames) const
{
	SlotNames.Add(CustomControlsSlotName);
}

UWidget* UModioUIComponentValidatorWidget::GetContentForSlot(FName SlotName) const
{
	if (SlotName == CustomControlsSlotName)
	{
		return CustomValidationControls;
	}
	return nullptr;
}

void UModioUIComponentValidatorWidget::SetContentForSlot(FName SlotName, UWidget* Content)
{
	if (SlotName == CustomControlsSlotName)
	{
		if (CustomValidationControls)
		{
			CustomValidationControls->ReleaseSlateResources(true);
		}
		CustomValidationControls = Content;
	}
}
