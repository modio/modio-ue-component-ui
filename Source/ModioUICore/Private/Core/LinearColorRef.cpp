/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Core/LinearColorRef.h"
#include "Core/ModioAssetPaths.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LinearColorRef)

FLinearColorRefColorChangeDelegate FLinearColorRef::ColorChangeHandlers;

FLinearColor ULinearColorRefLibrary::Resolve(const FLinearColorRef& Target, bool& bResolved)
{
	bResolved = false;
	FSoftObjectPath DefaultColorTablePath = ModioUI::AssetPaths::GetUiColorsTablePath();
	UDataTable* DefaultColorTable = Cast<UDataTable>(DefaultColorTablePath.TryLoad());
	if (DefaultColorTable)
	{
		const UScriptStruct* ColorTableStruct = DefaultColorTable->GetRowStruct();
		if (ColorTableStruct)
		{
			void* RowPtr = DefaultColorTable->FindRowUnchecked(Target.ReferencedColorName);

			if (RowPtr != nullptr)
			{
				const UScriptStruct* StructType = DefaultColorTable->GetRowStruct();

				if (StructType != nullptr)
				{
					FProperty* RawProp = DefaultColorTable->FindTableProperty(FName("Color"));
					FStructProperty* ColorProp = CastField<FStructProperty>(RawProp);
					if (ColorProp != nullptr && ColorProp->Struct == TBaseStructure<FLinearColor>::Get())
					{
						bResolved = true;
						FLinearColor* const Value = ColorProp->ContainerPtrToValuePtr<FLinearColor>(RowPtr);
						return *Value;
					}
				}
			}
		}
	}
	return Target.FallbackColor;
}

FLinearColor ULinearColorRefLibrary::ResolveOverride(const FLinearColorRefOverride& Target, bool& bOverrideSet)
{
	bOverrideSet = Target.bOverride;
	if (bOverrideSet)
	{
		bool bUnused;
		return Resolve(Target.Color, bUnused);
	}
	return FLinearColor {};
}

FLinearColor ULinearColorRefLibrary::ResolveOverrideAsExec(const FLinearColorRefOverride& Target, bool& bOverrideSet)
{
	return ResolveOverride(Target, bOverrideSet);
}

TArray<FName> ULinearColorRefLibrary::GetValidColorKeys()
{
	TArray<FName> ColorKeys;
	FSoftObjectPath DefaultColorTablePath = ModioUI::AssetPaths::GetUiColorsTablePath();
	UDataTable* DefaultColorTable = Cast<UDataTable>(DefaultColorTablePath.TryLoad());
	if (DefaultColorTable)
	{
		ColorKeys = DefaultColorTable->GetRowNames();
	}
	return ColorKeys;
}

void ULinearColorRefLibrary::AddColorChangeHandler(const FLinearColorRefColorChangeHandler& Handler)
{
	static bool bDataTableChangeDelegateBound = false;
	// Only ever bind this once
	if (!bDataTableChangeDelegateBound)
	{
		FSoftObjectPath DefaultColorTablePath =
			FSoftObjectPath("/ModioComponentUI/UI/Data/DT_UIColors.DT_UIColors");
		UDataTable* DefaultColorTable = Cast<UDataTable>(DefaultColorTablePath.TryLoad());
		if (DefaultColorTable)
		{
			DefaultColorTable->OnDataTableChanged().AddLambda(
				[]() { FLinearColorRef::ColorChangeHandlers.Broadcast(); });
			bDataTableChangeDelegateBound = true;
		}
	}
	FLinearColorRef::ColorChangeHandlers.AddUnique(Handler);
}

void ULinearColorRefLibrary::RemoveColorChangeHandler(const FLinearColorRefColorChangeHandler& Handler)
{
	FLinearColorRef::ColorChangeHandlers.Remove(Handler);
}
