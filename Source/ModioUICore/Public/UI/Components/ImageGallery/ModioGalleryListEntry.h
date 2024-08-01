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

#include "Blueprint/IUserObjectListEntry.h"
#include "UI/Components/ModioUIComponentBase.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUISelectableWidget.h"

#include "ModioGalleryListEntry.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioGalleryListEntry : public UModioUIComponentBase,
											   public IUserObjectListEntry,
											   public IModioUIClickableWidget,
											   public IModioUISelectableWidget
{
	GENERATED_BODY()
};
