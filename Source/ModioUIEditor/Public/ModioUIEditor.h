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

#include "Widgets/SWindow.h"
#include "Templates/SharedPointer.h"

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(ModioUIEditor, All, All);

class FModioUIEditor : public IModuleInterface
{
	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;

};