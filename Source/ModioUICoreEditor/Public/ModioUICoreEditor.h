#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(ModioUICoreEditor, All, All);

class FModioUICoreEditor : public IModuleInterface
{
public:
	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;

protected:
	bool UpdatePackagingSettings() const;

	TArray<TSharedPtr<class IAssetTypeActions>> AssetActions;
	TSharedPtr<class IBlueprintWidgetCustomizationExtender> ModioWidgetCustomizer;

	FDelegateHandle BlueprintVariableCustomizationHandle;
	FDelegateHandle BlueprintFunctionCustomizationHandle;
};