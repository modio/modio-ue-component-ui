#include "ModioUICoreEditor.h"

#include "Settings/ProjectPackagingSettings.h"
#include "BlueprintEditorModule.h"
#include "DetailsCustomizations/BlueprintWidgetMemberDetailsCustomization.h"
#include "DetailsCustomizations/BlueprintWidgetDetailsCustomization.h"
#include "DetailsCustomizations/ClassPathOverrideDetailsCustomization.h"
#include "DetailsCustomizations/LinearColorRefDetailsCustomization.h"
#include "Factories/ModioModInfoUIFactory.h"
#include "Factories/ModioModProgressInfoUIFactory.h"
#include "Factories/ModioUserUIFactory.h"
#include "IAssetTools.h"
#include "K2Node_FunctionEntry.h"
#include "Templates/SharedPointer.h"
#include "UMGEditorModule.h"

DEFINE_LOG_CATEGORY(ModioUICoreEditor);

#define LOCTEXT_NAMESPACE "FModioUICoreEditor"

void FModioUICoreEditor::StartupModule()
{
	UpdatePackagingSettings();

	// Register custom asset actions for our preview data binding objects
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	AssetActions.Add(MakeShared<FModioModInfoUIAssetActions>());
	AssetActions.Add(MakeShared<FModioModProgressInfoUIAssetActions>());
	AssetActions.Add(MakeShared<FModioUserUIAssetActions>());
	for (const auto& CurrentAssetAction : AssetActions)
	{
		AssetTools.RegisterAssetTypeActions(CurrentAssetAction.ToSharedRef());
	}
	// Register property customizations for color refs and class path overrides
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout("LinearColorRef",
													FOnGetPropertyTypeCustomizationInstance::CreateLambda([]() {
														return MakeShared<FLinearColorRefDetailsCustomization>();
													}));
	PropertyModule.RegisterCustomPropertyTypeLayout("ClassPathOverride",
													FOnGetPropertyTypeCustomizationInstance::CreateLambda([]() {
														return MakeShared<FClassPathOverrideDetailsCustomization>();
													}));
	PropertyModule.RegisterCustomClassLayout(
		UWidgetBlueprint::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateLambda([]() {
			return MakeShared<ModioUI::FModioWidgetBlueprintClassDetailsCustomization>();
		}));

	// Register a customizer for UMG widgets so we can move the mod.io variable category to the top of the details panel
	IUMGEditorModule& UMGEditorModule = FModuleManager::LoadModuleChecked<IUMGEditorModule>(TEXT("UMGEditor"));
	ModioWidgetCustomizer = StaticCastSharedRef<IBlueprintWidgetCustomizationExtender>(
		MakeShared<ModioUI::FModioBlueprintWidgetCustomizationExtender>());
	if (ModioWidgetCustomizer)
	{
		UMGEditorModule.AddWidgetCustomizationExtender(ModioWidgetCustomizer.ToSharedRef());
	}
	// Register extensions for blueprint variables and functions so we can edit their comments
	if (FBlueprintEditorModule* BlueprintEditorModule = FModuleManager::GetModulePtr<FBlueprintEditorModule>("Kismet"))
	{
		BlueprintVariableCustomizationHandle = BlueprintEditorModule->RegisterVariableCustomization(
			FProperty::StaticClass(),
			FOnGetVariableCustomizationInstance::CreateStatic(&ModioUI::MakeBlueprintWidgetMemberDetailsCustomization,
															  ModioUI::EBlueprintWidgetMemberType::EBPM_Property));
		/*BlueprintFunctionCustomizationHandle = BlueprintEditorModule->RegisterFunctionCustomization(
			UK2Node_FunctionEntry::StaticClass(),
			FOnGetFunctionCustomizationInstance::CreateStatic(&FGraphFunctionDetailsCustomization::MakeInstance));*/
	}
}

void FModioUICoreEditor::ShutdownModule()
{
	if (AssetActions.Num())
	{
		FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
		if (AssetToolsModule)
		{
			IAssetTools& AssetTools = AssetToolsModule->Get();
			for (const auto& CurrentAssetAction : AssetActions)
			{
				AssetTools.UnregisterAssetTypeActions(CurrentAssetAction.ToSharedRef());
			}
		}
	}

	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.UnregisterCustomPropertyTypeLayout("LinearColorRef");
	PropertyModule.UnregisterCustomPropertyTypeLayout("ClassPathOverride");

	IUMGEditorModule& UMGEditorModule = FModuleManager::LoadModuleChecked<IUMGEditorModule>(TEXT("UMGEditor"));
	if (ModioWidgetCustomizer)
	{
		UMGEditorModule.RemoveWidgetCustomizationExtender(ModioWidgetCustomizer.ToSharedRef());
	}

	if (BlueprintVariableCustomizationHandle.IsValid() || BlueprintFunctionCustomizationHandle.IsValid())
	{
		if (FBlueprintEditorModule* BlueprintEditorModule =
				FModuleManager::GetModulePtr<FBlueprintEditorModule>("Kismet"))
		{
			BlueprintEditorModule->UnregisterVariableCustomization(FProperty::StaticClass(),
																   BlueprintVariableCustomizationHandle);
			/*BlueprintEditorModule->UnregisterFunctionCustomization(UK2Node_FunctionEntry::StaticClass(),
																   BlueprintFunctionCustomizationHandle);*/
		}
	}
}

bool FModioUICoreEditor::UpdatePackagingSettings() const
{
	UProjectPackagingSettings* PackagingSettings = GetMutableDefault<UProjectPackagingSettings>();
	if (!PackagingSettings)
	{
		UE_LOG(ModioUICoreEditor, Error, TEXT("Failed to load packaging settings"));
		return false;
	}

	const FString ComponentUIDataPath = TEXT("/ModioComponentUI/UI/Data");
	const bool bIsAlreadyInPath = PackagingSettings->DirectoriesToAlwaysCook.ContainsByPredicate([&ComponentUIDataPath](const FDirectoryPath& DirPath)
	{
		return FPaths::IsSamePath(DirPath.Path, ComponentUIDataPath);
	});

	if (bIsAlreadyInPath)
	{
		UE_LOG(ModioUICoreEditor, Log, TEXT("Directories to always cook already contains %s"), *ComponentUIDataPath);
		return true;
	}

	FDirectoryPath NewDirPath;
	NewDirPath.Path = ComponentUIDataPath;
	PackagingSettings->DirectoriesToAlwaysCook.Add(NewDirPath);
	PackagingSettings->TryUpdateDefaultConfigFile();

	UE_LOG(ModioUICoreEditor, Log, TEXT("Added %s to directories to always cook"), *ComponentUIDataPath);
	return true;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModioUICoreEditor, ModioUICoreEditor)
