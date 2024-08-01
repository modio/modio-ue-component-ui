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

#include "Components/ContentWidget.h"
#include "Components/NamedSlot.h"
#include "Components/Widget.h"
#include "CoreMinimal.h"
// #include "Helpers/PlatformInfoSubsystem.h"
#include "UObject/SoftObjectPtr.h"

#include "ModioPlatformSpecificWidget.generated.h"

// Unimplemented - intended use case is to allow dynamic loading of a specific widget based on platform at runtime via
// soft class pointer
//  This allows widgets that call platform specific functionality to not cause problems on platforms where they
//  shouldn't be cooked/loaded/referenced.

// TODO: Implement this class

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioPlatformSpecificWidget : public UContentWidget
{
	GENERATED_BODY()
	/*protected:
		UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|PlatformSpecificWidget", EditFixedSize, meta =
	   (BlueprintProtected)) TMap<EPlatformName, TSoftClassPtr<UWidget>> PlatformWidgetReplacements;

		UPROPERTY()
		UWidget* ConcreteWidget = nullptr;

		TSharedPtr<SWidget> WrappedWidget = nullptr;
		bool bContentRequiresUpdate = false;

		void UpdateContent();
		*/
	virtual TSharedRef<SWidget> RebuildWidget() override;
	/* virtual void ReleaseSlateResources(bool bReleaseChildren) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	public:
	UModioPlatformSpecificWidget(const FObjectInitializer& Initializer);
	*/
};
