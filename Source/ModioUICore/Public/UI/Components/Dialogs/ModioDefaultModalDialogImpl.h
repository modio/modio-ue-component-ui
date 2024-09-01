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

#include "Blueprint/IUserObjectListEntry.h"
#include "Core/ModioDialogButtonInfo.h"
#include "Core/ModioPropertyOverrides.h"
#include "CoreMinimal.h"
#include "Internationalization/Text.h"
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/Interfaces/IModioUIDialog.h"

#include "ModioDefaultModalDialogImpl.generated.h"

/**
 * @brief Internal helper class for making the default component implementations of Dialog. Default Dialog
 * implementations use a Widget Blueprint deriving from this class to provide a consistent layout, the ability to
 * data-drive button configuration, etc. See `WBP_ModioDefaultModalDialogImpl` for more information.
 */
UCLASS()
class MODIOUICORE_API UModioDefaultModalDialogImpl : public UModioUIComponentBase, public IModioUIDialog
{
	GENERATED_BODY()
protected:
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category = "mod.io|UI|Dialog", meta = (BlueprintProtected))
	FModioDialogButtonClickedEventMulticast OnDialogButtonClicked;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Dialog", meta = (BlueprintProtected))
	bool bUseGeneratedButtonContent = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|Dialog",
			  meta = (EditCondition = "bUseGeneratedButtonContent", EditConditionHides, BlueprintProtected))
	TArray<FModioDialogButtonInfo> DialogButtonData;

	virtual void AddDialogConfirmedHandler_Implementation(const FModioDialogEvent& Handler) override;

	virtual void AddDialogCanceledHandler_Implementation(const FModioDialogEvent& Handler) override;

	virtual void AddDialogShownHandler_Implementation(const FModioDialogShownEvent& Handler) override;

	virtual void RemoveDialogConfirmedHandler_Implementation(const FModioDialogEvent& Handler) override;

	virtual void RemoveDialogCanceledHandler_Implementation(const FModioDialogEvent& Handler) override;

	virtual void RemoveDialogShownHandler_Implementation(const FModioDialogShownEvent& Handler) override;

	virtual void ShowModal_Implementation(bool bFocusCancelButton) override;

	virtual void CloseModal_Implementation() override;

	virtual void AddDialogButtonClickedHandler_Implementation(const FModioDialogButtonClickedEvent& Handler) override;

	virtual void RemoveDialogButtonClickedHandler_Implementation(
		const FModioDialogButtonClickedEvent& Handler) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
