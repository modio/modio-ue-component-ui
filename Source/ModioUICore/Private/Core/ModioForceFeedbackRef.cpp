/*
 *  Copyright (C) 2026 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Core/ModioForceFeedbackRef.h"
#include "Core/ModioAssetPaths.h"
#include "GameFramework/ForceFeedbackEffect.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioForceFeedbackRef)

FModioForceFeedbackRefChangeDelegate FModioForceFeedbackRef::ForceFeedbackChangeHandlers;

UForceFeedbackEffect* UModioForceFeedbackRefLibrary::Resolve(const FModioForceFeedbackRef& Target, bool& bResolved)
{
	bResolved = false;
	FSoftObjectPath DefaultForceFeedbackTablePath = ModioUI::AssetPaths::GetUiForceFeedbackTablePath();
	UDataTable* DefaultForceFeedbackTable = Cast<UDataTable>(DefaultForceFeedbackTablePath.TryLoad());
	if (DefaultForceFeedbackTable)
	{
		const UScriptStruct* ForceFeedbackTableStruct = DefaultForceFeedbackTable->GetRowStruct();
		if (ForceFeedbackTableStruct)
		{
			void* RowPtr = DefaultForceFeedbackTable->FindRowUnchecked(Target.ReferencedForceFeedbackName);

			if (RowPtr != nullptr)
			{
				FProperty* RawProp = DefaultForceFeedbackTable->FindTableProperty(FName("ForceFeedback"));
				FObjectPropertyBase* ForceFeedbackProp = CastField<FObjectPropertyBase>(RawProp);
				if (ForceFeedbackProp != nullptr && ForceFeedbackProp->PropertyClass != nullptr &&
					ForceFeedbackProp->PropertyClass->IsChildOf(UForceFeedbackEffect::StaticClass()))
				{
					void* ValuePtr = ForceFeedbackProp->ContainerPtrToValuePtr<void>(RowPtr);
					if (UForceFeedbackEffect* Value =
							Cast<UForceFeedbackEffect>(ForceFeedbackProp->GetObjectPropertyValue(ValuePtr)))
					{
						bResolved = true;
						return Value;
					}
				}
			}
		}
	}
	return Target.FallbackForceFeedback;
}

UForceFeedbackEffect* UModioForceFeedbackRefLibrary::ResolveOverride(const FModioForceFeedbackRefOverride& Target,
																	 bool& bOverrideSet)
{
	bOverrideSet = Target.bOverride;
	if (bOverrideSet)
	{
		bool bUnused;
		return Resolve(Target.ForceFeedback, bUnused);
	}
	return nullptr;
}

UForceFeedbackEffect* UModioForceFeedbackRefLibrary::ResolveOverrideAsExec(const FModioForceFeedbackRefOverride& Target,
																		   bool& bOverrideSet)
{
	return ResolveOverride(Target, bOverrideSet);
}

TArray<FName> UModioForceFeedbackRefLibrary::GetValidForceFeedbackKeys()
{
	TArray<FName> ForceFeedbackKeys;
	FSoftObjectPath DefaultForceFeedbackTablePath = ModioUI::AssetPaths::GetUiForceFeedbackTablePath();
	UDataTable* DefaultForceFeedbackTable = Cast<UDataTable>(DefaultForceFeedbackTablePath.TryLoad());
	if (DefaultForceFeedbackTable)
	{
		ForceFeedbackKeys = DefaultForceFeedbackTable->GetRowNames();
	}
	return ForceFeedbackKeys;
}

void UModioForceFeedbackRefLibrary::AddForceFeedbackChangeHandler(const FModioForceFeedbackRefChangeHandler& Handler)
{
	static bool bDataTableChangeDelegateBound = false;
	// Only ever bind this once
	if (!bDataTableChangeDelegateBound)
	{
		FSoftObjectPath DefaultForceFeedbackTablePath =
			FSoftObjectPath("/ModioComponentUI/UI/Data/DT_UIForceFeedback.DT_UIForceFeedback");
		UDataTable* DefaultForceFeedbackTable = Cast<UDataTable>(DefaultForceFeedbackTablePath.TryLoad());
		if (DefaultForceFeedbackTable)
		{
			DefaultForceFeedbackTable->OnDataTableChanged().AddLambda(
				[]() { FModioForceFeedbackRef::ForceFeedbackChangeHandlers.Broadcast(); });
			bDataTableChangeDelegateBound = true;
		}
	}
	FModioForceFeedbackRef::ForceFeedbackChangeHandlers.AddUnique(Handler);
}

void UModioForceFeedbackRefLibrary::RemoveForceFeedbackChangeHandler(const FModioForceFeedbackRefChangeHandler& Handler)
{
	FModioForceFeedbackRef::ForceFeedbackChangeHandlers.Remove(Handler);
}
