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

#if WITH_EDITOR
	#include "Blueprint/WidgetTree.h"
	#include "Editor/WidgetCompilerLog.h"
	#include "UObject/Class.h"
#endif

#include "Internationalization/StringTable.h"
#include "ModioSettings.h"
#include "ModioUICore.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"

#if WITH_EDITOR

struct FModioTScriptInterfaceValidator
{
	template<typename InterfaceType>
	static bool __declspec(noinline)
		Validate(class IWidgetCompilerLog& CompileLog, TScriptInterface<InterfaceType> ImplementingObject)
	{
		if (ImplementingObject.GetObject())
		{
			if (ImplementingObject.GetObject()->GetClass()->ImplementsInterface(
					InterfaceType::UClassType::StaticClass()))
			{
				return true;
			}
			else
			{
				CompileLog.Error(
					FText::Format(FTextFormat(FText::FromString("{0} does not implement required interface {1}")),
								  FText::FromName(ImplementingObject.GetObject()->GetFName()),
								  FText::FromString(InterfaceType::UClassType::StaticClass()->GetName())));
				return false;
			}
		}
		else
		{
			CompileLog.Warning(FText::Format(
				FTextFormat(FText::FromString("WidgetGetter returning interface {0} is currently returning nullptr!")),
				FText::FromString(InterfaceType::UClassType::StaticClass()->GetName())));
			return false;
		}
	}

	static bool ValidateWidgetGetters(class IWidgetCompilerLog& CompileLog)
	{
		return true;
	}

	template<typename... InterfaceTypes>
	static bool ValidateWidgetGetters(class IWidgetCompilerLog& CompileLog,
									  TScriptInterface<InterfaceTypes>... InterfaceObjects)
	{
		return (Validate(CompileLog, InterfaceObjects) && ...);
	}
};

	#define IMPLEMENT_MODIO_WIDGET_VALIDATION()                                                                       \
		void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) \
			const override                                                                                            \
		{                                                                                                             \
			Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);                                       \
			for (TFieldIterator<FProperty> PropIterator(GetClass()); PropIterator; ++PropIterator)                    \
			{                                                                                                         \
				FObjectProperty* ObjectProperty = CastField<FObjectProperty>(*PropIterator);                          \
				if (ObjectProperty && ObjectProperty->PropertyClass &&                                                \
					ObjectProperty->PropertyClass->IsChildOf<UWidget>())                                              \
				{                                                                                                     \
					if ((ObjectProperty->HasMetaData("BindWidget") ||                                                 \
						 ObjectProperty->HasMetaData("BindWidgetOptional")) &&                                        \
						ObjectProperty->HasMetaData("MustImplement"))                                                 \
					{                                                                                                 \
						UWidget* Widget = BlueprintWidgetTree.FindWidget(ObjectProperty->GetFName());                 \
						const FString& RequiredInterfaceString = ObjectProperty->GetMetaData("MustImplement");        \
						TArray<FString> ClassNames;                                                                   \
						RequiredInterfaceString.ParseIntoArrayWS(ClassNames, TEXT(","));                              \
						for (const FString& CurrentClassName : ClassNames)                                            \
						{                                                                                             \
							if (Widget && !Widget->GetClass()->ImplementsInterface(                                   \
											  UClass::TryFindTypeSlow<UClass>(*CurrentClassName)))                    \
							{                                                                                         \
								CompileLog.Error(FText::Format(                                                       \
									FTextFormat(FText::FromString("{0} does not implement required interface {1}")),  \
									FText::FromName(ObjectProperty->GetFName()),                                      \
									FText::FromString(CurrentClassName)));                                            \
							}                                                                                         \
						}                                                                                             \
					}                                                                                                 \
				}                                                                                                     \
			}                                                                                                         \
		}

	#define IMPLEMENT_MODIO_WIDGET_GETTER_VALIDATION(...)                                                             \
                                                                                                                      \
		void ValidateCompiledWidgetTree(const UWidgetTree& BlueprintWidgetTree, class IWidgetCompilerLog& CompileLog) \
			const override                                                                                            \
		{                                                                                                             \
			Super::ValidateCompiledWidgetTree(BlueprintWidgetTree, CompileLog);                                       \
			FModioTScriptInterfaceValidator::ValidateWidgetGetters(CompileLog, __VA_ARGS__);                          \
		}

	#define MODIO_WIDGET_PALETTE_OVERRIDE()               \
		virtual const FText GetPaletteCategory() override \
		{                                                 \
			return FText::FromString("mod.io");           \
		}
#else
	#define IMPLEMENT_MODIO_WIDGET_VALIDATION()
	#define IMPLEMENT_MODIO_WIDGET_GETTER_VALIDATION(...)
	#define MODIO_WIDGET_PALETTE_OVERRIDE()
#endif

namespace ModioUI
{

	/// These functions are templated helpers to make it easier to retrieve a widget from a TScriptInterface where that
	/// widget may not implement the interface via native code.

	template<typename WidgetType = class UWidget, typename InterfaceType>
	inline WidgetType* GetInterfaceWidgetChecked(TScriptInterface<InterfaceType> TargetWidget)
	{
		UObject* RawWidgetPtr = TargetWidget.GetObject();

		if (RawWidgetPtr && RawWidgetPtr->GetClass()->ImplementsInterface(InterfaceType::UClassType::StaticClass()))
		{
			return Cast<WidgetType>(RawWidgetPtr);
		}
		else
		{
			return nullptr;
		}
	}

	template<typename WidgetType = class UWidget, typename InterfaceType>
	inline WidgetType* GetInterfaceWidgetAsDataSourceWidget(TScriptInterface<InterfaceType> TargetWidget)
	{
		UObject* RawWidgetPtr = TargetWidget.GetObject();

		if (RawWidgetPtr && RawWidgetPtr->GetClass()->ImplementsInterface(UModioUIDataSourceWidget::StaticClass()))
		{
			return Cast<WidgetType>(RawWidgetPtr);
		}
		else
		{
			return nullptr;
		}
	}

	inline void EmitUnsupportedInterfaceCallWarning(UObject* ImplementingObject, const ANSICHAR* FuncName,
													TOptional<FString> AdditionalText)
	{
		checkf(ImplementingObject != nullptr, TEXT("Do not call this method with a null ImplementingObject."));
		checkf(FuncName != nullptr, TEXT("Do not call this method with a null FuncName."));

		UE_LOG(ModioUICore, Warning, TEXT("Class %s does not implement interface method %hs. %s"),
			   *ImplementingObject->GetClass()->GetFName().ToString(), FuncName,
			   AdditionalText.IsSet() ? **AdditionalText : TEXT(""));
	}

} // namespace ModioUI

#if WITH_EDITOR
	#define MODIO_INTERFACE_UNIMPLEMENTED_FUNCTION_MSG(...) \
		ModioUI::EmitUnsupportedInterfaceCallWarning(this, __func__, {__VA_ARGS__})
	#define MODIO_INTERFACE_UNIMPLEMENTED_FUNCTION(...) ModioUI::EmitUnsupportedInterfaceCallWarning(this, __func__, {})
#else
	#define MODIO_INTERFACE_UNIMPLEMENTED_FUNCTION_MSG(...)
	#define MODIO_INTERFACE_UNIMPLEMENTED_FUNCTION(...)
#endif