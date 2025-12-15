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

class UDetailsView;
class USinglePropertyView;

/**
 *
 */
UCLASS(BlueprintType)
class MODIOUICOREEDITOR_API UModioUIComponentValidatorWidget : public UWidget
{
	GENERATED_BODY()
public:
	// Widget Class to Validate
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ComponentValidator")
	TSubclassOf<UWidget> WidgetClassToValidate;

	// Component Type to Validate Against
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ComponentValidator")
	EModioUIComponentID ComponentToValidateAgainst = EModioUIComponentID::Button;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ComponentValidator")
	TEnumAsByte<EHorizontalAlignment> HorizontalAlignment = EHorizontalAlignment::HAlign_Center;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ComponentValidator")
	TEnumAsByte<EVerticalAlignment> VerticalAlignment = EVerticalAlignment::VAlign_Center;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|ComponentValidator")
	TMap<EModioUIComponentID, TSubclassOf<UWidget>> ModioComponentExampleWidgets;

	#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif

	void LoadExampleWidgetClass();
	bool ValidateConcreteWidget(FText& FailureReason);
	void ReleaseSlateResources(bool bReleaseChildren) override;
	
protected: // Rebuild Widget Tree
	TSharedRef<SWidget> RebuildWidget() override;
	TSharedRef<SWidget> RebuildWidgetClassPropertyView();
	TSharedRef<SWidget> RebuildComponentPropertyView();
	TSharedRef<SWidget> RebuildHAlignmentPropertyView();
	TSharedRef<SWidget> RebuildVAlignmentPropertyView();
	TSharedRef<SWidget> RebuildDetailsWidget();

private: // Dynamic Widget Tree
	void BuildWidgetToValidate();
	void BuildValidationStatusWidget();
	void BuildInterfaceValidationControls(EModioUIComponentID ComponentType, TObjectPtr<UObject> ImplementingObject);
	
protected: // Cached UWidgets
	UPROPERTY(Transient)
	TObjectPtr<UWidget> WidgetToValidate;
	UPROPERTY()
	TObjectPtr<USinglePropertyView> WidgetClassPropertyView;
	UPROPERTY()
	TObjectPtr<USinglePropertyView> ComponentPropertyView;
	UPROPERTY()
	TObjectPtr<USinglePropertyView> VAlignmentPropertyView;
	UPROPERTY()
	TObjectPtr<USinglePropertyView> HAlignmentPropertyView;
	UPROPERTY()
	TObjectPtr<UDetailsView> DetailsView;
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|ComponentValidator")
	TObjectPtr<UWidget> CustomValidationControls;
	
private: // Cached Slate Resources
	TSharedPtr<SWidget> RootSlateWidget;
	TSharedPtr<class SListView<TSharedPtr<struct FModioUIComponentValidationStatusLine>>> CachedValidationStatusWidget;
	TSharedPtr<SWidget> CachedWidgetToValidateSlate;
	TSharedPtr<SWidget> CachedDetailsViewSlate;
	
	TSharedPtr<SBox> WidgetToValidateSlot;
	TSharedPtr<SBox> ValidationStatusWidgetSlot;
	TSharedPtr<SBox> InterfaceValidationControlsSlot;

protected:
	TArray<TSharedPtr<struct FModioUIComponentValidationStatusLine>> ValidationMessages;

	static TSharedRef<ITableRow> GenerateStatusLineWidget(
		TSharedPtr<struct FModioUIComponentValidationStatusLine> Message,
		const TSharedRef<class STableViewBase>& OwningTable);

	static TSharedRef<ITableRow> GenerateInterfaceTestWidget(UClass* InterfaceClass,
															 const TSharedRef<class STableViewBase>& OwningTable,
															 TObjectPtr<UObject> ImplementingWidget);

	
	virtual void OnStatusWidgetDesiredVisibilityChanged(ECheckBoxState NewState);
	virtual ECheckBoxState GetStatusWidgetDesiredVisibility() const;
	virtual EVisibility GetStatusWidgetVisibility() const;
	
	UPROPERTY(BlueprintReadOnly, Category = "mod.io|UI|ComponentValidator")
	bool bValidationPassed = false;
	bool bShowValidationStatusWidget = true;
};
