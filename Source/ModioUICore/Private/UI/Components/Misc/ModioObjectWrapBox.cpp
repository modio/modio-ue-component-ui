/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/Misc/ModioObjectWrapBox.h"

#include "Editor/WidgetCompilerLog.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioObjectWrapBox)

void UModioObjectWrapBox::NativeSetObjects(const TArray<UObject*>& InObjects)
{
	BoundObjects = InObjects;
	ClearChildren();
	if (UClass* ConcreteWidgetClass = WidgetClass.Get())
	{
		if (ConcreteWidgetClass->ImplementsInterface(UModioUIDataSourceWidget::StaticClass()))
		{
			for (UObject* CurrentObject : InObjects)
			{
				UWidget* ChildWidget = NewObject<UWidget>(this, ConcreteWidgetClass);
				AddChildToWrapBox(ChildWidget);
				IModioUIDataSourceWidget::Execute_SetDataSource(ChildWidget, CurrentObject);
				OnWidgetCreated.Broadcast(ChildWidget, CurrentObject);
			}
		}
	}
}

TArray<UObject*> UModioObjectWrapBox::NativeGetObjects()
{
	return BoundObjects;
}

UObject* UModioObjectWrapBox::NativeGetObjectAt(int32 Index) const
{
	if (Index >= 0 && Index < BoundObjects.Num())
	{
		return BoundObjects[Index];
	}
	return nullptr;
}

#if WITH_EDITOR
void UModioObjectWrapBox::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	if (WidgetClass)
	{
		if (!WidgetClass->ImplementsInterface(UModioUIDataSourceWidget::StaticClass()))
		{
			CompileLog.Error(FText::Format(FTextFormat::FromString("'{0}' has WidgetClass property set to '{1}' "
																   "which does not implement ModioUIDataSourceWidget."),
										   FText::FromString(GetName()),
										   FText::FromString(WidgetClass->GetName())));
		}
	}
}
#endif

void UModioObjectWrapBox::NativeAddObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler)
{
	OnWidgetCreated.AddUnique(Handler);
}

void UModioObjectWrapBox::NativeRemoveObjectWidgetCreatedHandler(const FModioObjectListOnObjectWidgetCreated& Handler)
{
	OnWidgetCreated.Remove(Handler);
}

UWidget* UModioObjectWrapBox::NativeGetFirstEntryWidget() const
{
	return GetChildAt(0);
}
