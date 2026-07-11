#pragma once

#include "Misc/EngineVersionComparison.h"
#include "UObject/MetaData.h"
#include "UObject/Package.h"
 
/// @brief Namespace containing helper methods to simplify MetaData code
namespace ModioUICoreEditorMetadataEngineCompat
{
	FORCEINLINE static auto GetMetaData(UPackage* Package)
	{
#if UE_VERSION_OLDER_THAN(5, 6, 0)
		return Package->GetMetaData();
#else
		return &Package->GetMetaData();
#endif
	}
} // namespace ModioUiCoreEditorMetadataEngineCompat
