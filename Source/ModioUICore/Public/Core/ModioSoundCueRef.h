/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Containers/Map.h"
#include "Delegates/Delegate.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/UnrealNames.h"

#include "ModioSoundCueRef.generated.h"

class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FModioSoundCueRefChangeDelegate);
DECLARE_DYNAMIC_DELEGATE(FModioSoundCueRefChangeHandler);

/**
 * @brief Struct representing a sound entry in a data table
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|SoundCueRef")
struct MODIOUICORE_API FModioUISoundEntry : public FTableRowBase
{
	GENERATED_BODY();

	/**
	 * @brief The sound to use for the entry
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, category = "mod.io UI Sound Entry")
	TObjectPtr<USoundBase> Sound = nullptr;
};

/**
 * @brief Blueprint-compatible struct representing a sound reference
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|SoundCueRef")
struct MODIOUICORE_API FModioSoundCueRef
{
	GENERATED_BODY();

	/**
	 * @brief The name of the sound to reference
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, category = "mod.io|UI|SoundCueRef")
	FName ReferencedSoundName;

	/**
	 * @brief The sound to use if the reference is not found
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "mod.io|UI|SoundCueRef", AdvancedDisplay)
	TObjectPtr<USoundBase> FallbackSound = nullptr;

	bool IsValid() const
	{
		return ReferencedSoundName != NAME_None;
	}
	FModioSoundCueRef() : ReferencedSoundName(NAME_None), FallbackSound(nullptr) {};
	FModioSoundCueRef(FName SoundName) : ReferencedSoundName(SoundName), FallbackSound(nullptr) {};

protected:
	friend class UModioSoundCueRefLibrary;

	/**
	 * @brief Handler for sound table change events
	 */
	static FModioSoundCueRefChangeDelegate SoundChangeHandlers;
};

/**
 * @brief Blueprint-compatible struct representing a sound reference override
 */
USTRUCT(BlueprintType, Category = "mod.io|UI|SoundCueRef Override")
struct FModioSoundCueRefOverride
{
	GENERATED_BODY()

	/**
	 * @brief Whether the sound reference is overridden
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle),
			  category = "mod.io|UI|SoundCueRef Override")
	bool bOverride = false;

	/**
	 * @brief The sound to use if the reference is overridden
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (EditCondition = "bOverride", ShowOnlyInnerProperties),
			  category = "mod.io|UI|SoundCueRef Override")
	FModioSoundCueRef Sound;
};

/**
 * @brief Library of functions for working with sound references
 */
UCLASS()
class MODIOUICORE_API UModioSoundCueRefLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Resolves a sound reference to a sound asset
	 * @param Target The sound reference to resolve
	 * @param bResolved True if the sound reference was resolved, false if the fallback sound was used
	 * @return The resolved sound asset
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|SoundCueRef")
	static USoundBase* Resolve(const FModioSoundCueRef& Target, bool& bResolved);

	/**
	 * @brief Resolves a sound reference override to a sound asset
	 * @param Target The sound reference override to resolve
	 * @param bOverrideSet True if the sound reference was overridden, false otherwise
	 * @return The resolved sound asset, or null if the override is not set
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|SoundCueRef",
			  meta = (DisplayName = "Resolve Sound Override"))
	static USoundBase* ResolveOverride(const FModioSoundCueRefOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Resolves a sound reference override to a sound asset
	 * @param Target The sound reference override to resolve
	 * @param bOverrideSet True if the sound reference was overridden, false otherwise
	 * @return The resolved sound asset, or null if the override is not set
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|SoundCueRef",
			  meta = (DisplayName = "Resolve Sound Override As Exec", ExpandEnumAsExecs = "bOverrideSet"))
	static USoundBase* ResolveOverrideAsExec(const FModioSoundCueRefOverride& Target, bool& bOverrideSet);

	/**
	 * @brief Gets the list of valid sound keys
	 * @return The list of valid sound keys
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|SoundCueRef")
	static TArray<FName> GetValidSoundKeys();

	/**
	 * @brief Adds a handler to be called when the sound table changes
	 * @param Handler The handler to add
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|SoundCueRef")
	static void AddSoundChangeHandler(const FModioSoundCueRefChangeHandler& Handler);

	/**
	 * @brief Removes a handler from the list of handlers to be called when the sound table changes
	 * @param Handler The handler to remove
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "mod.io|UI|SoundCueRef")
	static void RemoveSoundChangeHandler(const FModioSoundCueRefChangeHandler& Handler);
};
