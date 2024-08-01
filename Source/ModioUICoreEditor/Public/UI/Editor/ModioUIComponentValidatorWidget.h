// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/NamedSlot.h"
#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "IDetailsView.h"
#include "UI/Components/ModioUIComponentStatics.h"
#include "Widgets/Views/ITableRow.h"
#include "Widgets/Views/SListView.h"

#include "ModioUIComponentValidatorWidget.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICOREEDITOR_API UModioUIComponentValidatorWidget : public UWidget, public INamedSlotInterface
{
	GENERATED_BODY()

protected:
	TArray<TSharedPtr<struct FModioUIComponentValidationStatusLine>> ValidationMessages;

	static TSharedRef<ITableRow> GenerateStatusLineWidget(
		TSharedPtr<struct FModioUIComponentValidationStatusLine> Message,
		const TSharedRef<class STableViewBase>& OwningTable);

	static TSharedRef<ITableRow> GenerateInterfaceTestWidget(UClass* InterfaceClass,
															 const TSharedRef<class STableViewBase>& OwningTable,
															 TObjectPtr<UObject> ImplementingWidget);

	bool bShowValidationStatusWidget = true;
	virtual void OnStatusWidgetDesiredVisibilityChanged(ECheckBoxState NewState);
	virtual ECheckBoxState GetStatusWidgetDesiredVisibility() const;
	virtual EVisibility GetStatusWidgetVisibility() const;

	void OnComponentPropertyChanged(const FPropertyChangedEvent& Event);

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|ComponentValidator")
	bool bValidationPassed = false;

	bool ValidateConcreteWidget(FText& FailureReason);

	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|ComponentValidator")
	TObjectPtr<UWidget> CustomValidationControls;

	FName CustomControlsSlotName = FName("Custom Validation Controls");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ComponentValidator")
	EModioUIComponentID ComponentToValidateAgainst;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ComponentValidator")
	TSubclassOf<UWidget> ConcreteComponentClassToValidate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ComponentValidator")
	TEnumAsByte<EHorizontalAlignment> TargetWidgetHAlign = EHorizontalAlignment::HAlign_Center;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ComponentValidator")
	TEnumAsByte<EVerticalAlignment> TargetWidgetVAlign = EVerticalAlignment::VAlign_Center;

	UPROPERTY(Transient)
	TObjectPtr<UWidget> ConcreteManagedWidget;

	TSharedPtr<SWidget> CachedConcreteSlateWidget;

	TSharedPtr<SWidget> RootSlateWidget;

	TSharedPtr<class SListView<TSharedPtr<struct FModioUIComponentValidationStatusLine>>> CachedValidationStatusWidget;

	TSharedPtr<class IDetailsView> ComponentDetailsWidget;

	TSharedRef<SWidget> RebuildWidget() override;

	TSharedRef<SWidget> RebuildConcreteWidget();

	TSharedRef<SWidget> RebuildValidationStatusWidget();

	TSharedRef<SWidget> RebuildDetailsWidget();

	TSharedRef<SWidget> RebuildInterfaceValidationControls(EModioUIComponentID ComponentType,
														   TObjectPtr<UObject> ImplementingObject);
	void SynchronizeProperties() override;

	void ReleaseSlateResources(bool bReleaseChildren) override;

public:
	UWidget* GetWidgetBeingValidated() const;
	void GetSlotNames(TArray<FName>& SlotNames) const override;

	UWidget* GetContentForSlot(FName SlotName) const override;

	void SetContentForSlot(FName SlotName, UWidget* Content) override;
};
