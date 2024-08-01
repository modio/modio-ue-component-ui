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

#include "CoreMinimal.h"
#include "UI/Components/ModioUIComponentBase.h"

#include "ModioUserDisplay.generated.h"

/**
 * @brief The User Display component is a widget that visualizes an FModioUser object.
 * @component_display_name User Display
 */
UCLASS(Abstract)
class MODIOUICORE_API UModioUserDisplay : public UModioUIComponentBase
{
	GENERATED_BODY()

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io|UI|Data Binding|User Display", meta = (BlueprintProtected))
	TObjectPtr<UObject> EditorDataSource;
#endif

	//~ Begin IModioUIDataSourceWidget Interface
	virtual void NativePreConstruct() override;
	//~ End IModioUIDataSourceWidget Interface
};
