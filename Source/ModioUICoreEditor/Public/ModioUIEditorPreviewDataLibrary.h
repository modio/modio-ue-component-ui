
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Types/ModioModManagementEvent.h"
#include "Types/ModioModProgressInfo.h"

#include "ModioUIEditorPreviewDataLibrary.generated.h"

/**
 *
 */
UCLASS()
class MODIOUICOREEDITOR_API UModioUIEditorPreviewDataLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#if WITH_EDITOR

	/**
	 * Constructs a mock FModioModManagementEvent with dummy values. Only intended for use in editor previews, eg UMG
	 * editor
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|EditorUtilities")
	static FModioModManagementEvent ConstructPreviewManagementEvent(FModioModID ID, EModioModManagementEventType Event,
																	FModioErrorCode ec)
	{
		FModioModManagementEvent Out {};
		Out.ID = ID;
		Out.Event = Event;
		Out.Status = ec;
		return Out;
	}

	/**
	 * Constructs a mock FModioModProgressInfo with dummy values. Only intended for use in editor previews, eg UMG
	 * editor
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|EditorUtilities")
	static FModioModProgressInfo ConstructPreviewProgressInfo(EModioModProgressState CurrentState,
															  FModioUnsigned64 Current, FModioUnsigned64 Total)
	{
		FModioModProgressInfo Out {};
		switch (CurrentState)
		{
			case EModioModProgressState::Compressing:
				Out.CompressCurrent = Current.Underlying;
				Out.CompressTotal = Total.Underlying;
				break;
			case EModioModProgressState::Downloading:
				Out.DownloadCurrent = Current.Underlying;
				Out.DownloadTotal = Total.Underlying;
				break;
			case EModioModProgressState::Uploading:
				Out.UploadCurrent = Current.Underlying;
				Out.UploadTotal = Total.Underlying;
				break;
			case EModioModProgressState::Extracting:
				Out.ExtractCurrent = Current.Underlying;
				Out.ExtractTotal = Total.Underlying;
				break;
			default:
				break;
		}
		Out.CurrentState = CurrentState;
		return Out;
	}

#endif
};
