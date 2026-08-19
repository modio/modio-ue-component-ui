/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Core/ModioSoundCueRef.h"
#include "Core/ModioAssetPaths.h"
#include "Sound/SoundBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioSoundCueRef)

FModioSoundCueRefChangeDelegate FModioSoundCueRef::SoundChangeHandlers;

USoundBase* UModioSoundCueRefLibrary::Resolve(const FModioSoundCueRef& Target, bool& bResolved)
{
	bResolved = false;
	FSoftObjectPath DefaultSoundTablePath = ModioUI::AssetPaths::GetUiSoundsTablePath();
	UDataTable* DefaultSoundTable = Cast<UDataTable>(DefaultSoundTablePath.TryLoad());
	if (DefaultSoundTable)
	{
		const UScriptStruct* SoundTableStruct = DefaultSoundTable->GetRowStruct();
		if (SoundTableStruct)
		{
			void* RowPtr = DefaultSoundTable->FindRowUnchecked(Target.ReferencedSoundName);

			if (RowPtr != nullptr)
			{
				FProperty* RawProp = DefaultSoundTable->FindTableProperty(FName("Sound"));
				FObjectPropertyBase* SoundProp = CastField<FObjectPropertyBase>(RawProp);
				if (SoundProp != nullptr && SoundProp->PropertyClass != nullptr &&
					SoundProp->PropertyClass->IsChildOf(USoundBase::StaticClass()))
				{
					void* ValuePtr = SoundProp->ContainerPtrToValuePtr<void>(RowPtr);
					if (USoundBase* Value = Cast<USoundBase>(SoundProp->GetObjectPropertyValue(ValuePtr)))
					{
						bResolved = true;
						return Value;
					}
				}
			}
		}
	}
	return Target.FallbackSound;
}

USoundBase* UModioSoundCueRefLibrary::ResolveOverride(const FModioSoundCueRefOverride& Target, bool& bOverrideSet)
{
	bOverrideSet = Target.bOverride;
	if (bOverrideSet)
	{
		bool bUnused;
		return Resolve(Target.Sound, bUnused);
	}
	return nullptr;
}

USoundBase* UModioSoundCueRefLibrary::ResolveOverrideAsExec(const FModioSoundCueRefOverride& Target, bool& bOverrideSet)
{
	return ResolveOverride(Target, bOverrideSet);
}

TArray<FName> UModioSoundCueRefLibrary::GetValidSoundKeys()
{
	TArray<FName> SoundKeys;
	FSoftObjectPath DefaultSoundTablePath = ModioUI::AssetPaths::GetUiSoundsTablePath();
	UDataTable* DefaultSoundTable = Cast<UDataTable>(DefaultSoundTablePath.TryLoad());
	if (DefaultSoundTable)
	{
		SoundKeys = DefaultSoundTable->GetRowNames();
	}
	return SoundKeys;
}

void UModioSoundCueRefLibrary::AddSoundChangeHandler(const FModioSoundCueRefChangeHandler& Handler)
{
	static bool bDataTableChangeDelegateBound = false;
	// Only ever bind this once
	if (!bDataTableChangeDelegateBound)
	{
		FSoftObjectPath DefaultSoundTablePath =
			FSoftObjectPath("/ModioComponentUI/UI/Data/DT_UISounds.DT_UISounds");
		UDataTable* DefaultSoundTable = Cast<UDataTable>(DefaultSoundTablePath.TryLoad());
		if (DefaultSoundTable)
		{
			DefaultSoundTable->OnDataTableChanged().AddLambda(
				[]() { FModioSoundCueRef::SoundChangeHandlers.Broadcast(); });
			bDataTableChangeDelegateBound = true;
		}
	}
	FModioSoundCueRef::SoundChangeHandlers.AddUnique(Handler);
}

void UModioSoundCueRefLibrary::RemoveSoundChangeHandler(const FModioSoundCueRefChangeHandler& Handler)
{
	FModioSoundCueRef::SoundChangeHandlers.Remove(Handler);
}
