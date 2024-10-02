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

#include "ModioUISettings.h"

/** Modio Asset Paths
 *	This namespace exists to consolidate any hard-coded asset paths we may
 *	need in the plugin into a single location for ease of access/editing.
 *
 *	Should one of these need to be developer-exposed, the asset itself
 *	should be optionally overriden in an appropriate settings file, then
 *	the relevant Getter here should check if that setting is set and
 *	return it instead of the default.
 */
namespace ModioUI::AssetPaths
{
	static UModioUISettings* GetSettings()
	{
		return GetMutableDefault<UModioUISettings>();
	}

	static FSoftObjectPath GetUiColorsTablePath()
	{
		if (GetSettings()->ColorsTable.IsValid())
			return GetSettings()->ColorsTable.ToSoftObjectPath();
		else
			return FSoftObjectPath("/ModioComponentUI/UI/Data/DT_UIColors.DT_UIColors");
	};

	static FSoftObjectPath GetUiCommandsTablePath()
	{
		if (GetSettings()->CommandsTable.IsValid())
			return GetSettings()->CommandsTable.ToSoftObjectPath();
		else
			return FSoftObjectPath("/ModioComponentUI/UI/Data/DT_UICommands.DT_UICommands");
	};

	static FSoftObjectPath GetUnderlineMaterialPath()
	{
		if (GetSettings()->UnderlineMaterial.IsValid())
			return GetSettings()->UnderlineMaterial.ToSoftObjectPath();
		else
			return FSoftObjectPath("/ModioComponentUI/UI/Materials/M_UI_Underline.M_UI_Underline");
	};

	static FSoftObjectPath GetFakeCaretMaterialPath()
	{
		if (GetSettings()->FakeCaretMaterial.IsValid())
			return GetSettings()->FakeCaretMaterial.ToSoftObjectPath();
		else
			return FSoftObjectPath("/ModioComponentUI/UI/Materials/M_UI_FakeCaret.M_UI_FakeCaret");
	};
};
