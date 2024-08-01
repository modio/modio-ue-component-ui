/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "Core/ModioPropertyOverrides.h"

int32 UPropertyOverridesLibrary::ResolveIntOverride(const FIntegerOverride& Target, bool& bOverrideSet)
{
	bOverrideSet = Target.bOverride;
	if (bOverrideSet)
	{
		return Target.Value;
	}
	return 0;
}

int32 UPropertyOverridesLibrary::ResolveIntOverrideAsExec(const FIntegerOverride& Target, bool& bOverrideSet)
{
	return ResolveIntOverride(Target, bOverrideSet);
}

float UPropertyOverridesLibrary::ResolveFloatOverride(const FFloatOverride& Target, bool& bOverrideSet)
{
	bOverrideSet = Target.bOverride;
	if (bOverrideSet)
	{
		return Target.Value;
	}
	return 0;
}

float UPropertyOverridesLibrary::ResolveFloatOverrideAsExec(const FFloatOverride& Target, bool& bOverrideSet)
{
	return ResolveFloatOverride(Target, bOverrideSet);
}

float UPropertyOverridesLibrary::ApplyFloatOverride(const FFloatOverride& Target, float DefaultValue)
{
	return Target.bOverride ? Target.Value : DefaultValue;
}

int32 UPropertyOverridesLibrary::ApplyIntOverride(const FIntegerOverride& Target, int32 DefaultValue)
{
	return Target.bOverride ? Target.Value : DefaultValue;
}

FDataTableRowHandle UPropertyOverridesLibrary::ResolveInputActionOverride(const FInputActionOverride& Target,
																		  bool& bOverrideSet)
{
	bOverrideSet = Target.bOverride;
	if (bOverrideSet)
	{
		return Target.Value;
	}
	return {};
}

FDataTableRowHandle UPropertyOverridesLibrary::ResolveInputActionOverrideAsExec(const FInputActionOverride& Target,
																				bool& bOverrideSet)
{
	return ResolveInputActionOverride(Target, bOverrideSet);
}

FDataTableRowHandle UPropertyOverridesLibrary::ApplyInputActionOverride(const FInputActionOverride& Target,
																		FDataTableRowHandle DefaultValue)
{
	if (Target.bOverride)
	{
		return Target.Value;
	}
	return DefaultValue;
}

FText UPropertyOverridesLibrary::ResolveTextOverride(const FTextOverride& Target, bool& bOverrideSet)
{
	bOverrideSet = Target.bOverride;
	if (bOverrideSet)
	{
		return Target.Value;
	}
	return {};
}

FText UPropertyOverridesLibrary::ResolveTextOverrideAsExec(const FTextOverride& Target, bool& bOverrideSet)
{
	return ResolveTextOverride(Target, bOverrideSet);
}

FText UPropertyOverridesLibrary::ApplyTextOverride(const FTextOverride& Target, FText DefaultValue)
{
	if (Target.bOverride)
	{
		return Target.Value;
	}
	return DefaultValue;
}

FSoftClassPath UPropertyOverridesLibrary::ResolveClassPathOverride(const FClassPathOverride& Target, bool& bOverrideSet)
{
	bOverrideSet = Target.bOverride;
	if (bOverrideSet)
	{
		return Target.Value;
	}
	return {};
}

FSoftClassPath UPropertyOverridesLibrary::ResolveClassPathOverrideAsExec(const FClassPathOverride& Target,
																		 bool& bOverrideSet)
{
	return ResolveClassPathOverride(Target, bOverrideSet);
}

FSoftClassPath UPropertyOverridesLibrary::ApplyClassPathOverride(const FClassPathOverride& Target,
																 FSoftClassPath DefaultValue)
{
	if (Target.bOverride)
	{
		return Target.Value;
	}
	return DefaultValue;
}

UClass* UPropertyOverridesLibrary::ResolveClassPathOverride_Casted(const FClassPathOverride& Target,
																   TSubclassOf<UObject> BaseClass, bool& bOverrideSet)
{
	UClass* LoadedClass = ResolveClassPathOverride(Target, bOverrideSet).TryLoadClass<UObject>();
	if (LoadedClass && LoadedClass->IsChildOf(BaseClass.Get()))
	{
		return LoadedClass;
	}
	else
	{
		return nullptr;
	}
}

UClass* UPropertyOverridesLibrary::ResolveClassPathOverrideAsExec_Casted(const FClassPathOverride& Target,
																		 TSubclassOf<UObject> BaseClass,
																		 bool& bOverrideSet)
{
	return ResolveClassPathOverride_Casted(Target, BaseClass, bOverrideSet);
}

UClass* UPropertyOverridesLibrary::ApplyClassPathOverride_Casted(const FClassPathOverride& Target,
																 TSubclassOf<UObject> BaseClass, UClass* DefaultValue)
{
	bool bOverrideSet = false;
	UClass* LoadedClass = ResolveClassPathOverride(Target, bOverrideSet).TryLoadClass<UObject>();
	if (bOverrideSet && LoadedClass && LoadedClass->IsChildOf(BaseClass.Get()))
	{
		return LoadedClass;
	}
	else
	{
		return DefaultValue;
	}
}

EHorizontalAlignment UPropertyOverridesLibrary::ResolveHAlignOverride(const FHAlignOverride& Target, bool& bOverrideSet)
{
	bOverrideSet = Target.bOverride;
	if (bOverrideSet)
	{
		return Target.Value;
	}
	return {};
}

EHorizontalAlignment UPropertyOverridesLibrary::ResolveHAlignOverrideAsExec(const FHAlignOverride& Target,
																			bool& bOverrideSet)
{
	return ResolveHAlignOverride(Target, bOverrideSet);
}

EHorizontalAlignment UPropertyOverridesLibrary::ApplyHAlignOverride(const FHAlignOverride& Target,
																	EHorizontalAlignment DefaultValue)
{
	if (Target.bOverride)
	{
		return Target.Value;
	}
	return DefaultValue;
}

FSlateFontInfo UPropertyOverridesLibrary::ResolveFontInfoOverride(const FSlateFontInfoOverride& Target,
																  bool& bOverrideSet)
{
	bOverrideSet = Target.bOverride;
	if (bOverrideSet)
	{
		return Target.Value;
	}
	return {};
}

FSlateFontInfo UPropertyOverridesLibrary::ResolveFontInfoOverrideAsExec(const FSlateFontInfoOverride& Target,
																		bool& bOverrideSet)
{
	return ResolveFontInfoOverride(Target, bOverrideSet);
}

FSlateFontInfo UPropertyOverridesLibrary::ApplyFontInfoOverride(const FSlateFontInfoOverride& Target,
																FSlateFontInfo DefaultValue)
{
	if (Target.bOverride)
	{
		return Target.Value;
	}
	return DefaultValue;
}

ETextTransformPolicy UPropertyOverridesLibrary::ResolveTextTransformOverride(const FTextTransformOverride& Target,
																			 bool& bOverrideSet)
{
	bOverrideSet = Target.bOverride;
	if (bOverrideSet)
	{
		return Target.Value;
	}
	return {};
}

ETextTransformPolicy UPropertyOverridesLibrary::ResolveTextTransformOverrideAsExec(const FTextTransformOverride& Target,
																				   bool& bOverrideSet)
{
	return ResolveTextTransformOverride(Target,bOverrideSet);
}

ETextTransformPolicy UPropertyOverridesLibrary::ApplyTextTransformOverride(const FTextTransformOverride& Target,
																		   ETextTransformPolicy DefaultValue)
{
	if (Target.bOverride)
	{
		return Target.Value;
	}
	return DefaultValue;
}
