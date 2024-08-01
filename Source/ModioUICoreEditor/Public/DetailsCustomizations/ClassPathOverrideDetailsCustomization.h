/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "Algo/Transform.h"
#include "Brushes/SlateColorBrush.h"
#include "Containers/Array.h"
#include "Containers/UnrealString.h"
#include "Core/ModioPropertyOverrides.h"
#include "Core/ModioUIHelpers.h"
#include "EditorClassUtils.h"
#include "PropertyCustomizationHelpers.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IDetailCustomization.h"
#include "IPropertyUtilities.h"
#include "PropertyHandle.h"

#include "UObject/UnrealNames.h"
#include "Widgets/DeclarativeSyntaxSupport.h"


class FClassPathOverrideDetailsCustomization : public IPropertyTypeCustomization
{
	TSharedPtr<IPropertyHandle> UnderlyingColorRefPropHandle;

public:
	FClassPathOverrideDetailsCustomization() {

	};

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		HeaderRow.NameContent()[PropertyHandle->CreatePropertyNameWidget()];
	}

	static void OnSetClass(const UClass* NewClass, TSharedRef<IPropertyHandle> PropertyHandle)
	{
		PropertyHandle->SetValueFromFormattedString((NewClass) ? NewClass->GetPathName() : "None");
	}
	static const UClass* OnGetClass(TSharedRef<IPropertyHandle> PropertyHandle)
	{
		FString ClassName;
		PropertyHandle->GetValueAsFormattedString(ClassName);
		UClass* ResolvedClass = FEditorClassUtils::GetClassFromString(ClassName);
		return ResolvedClass;
	}
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
								   IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		TSharedPtr<IPropertyHandle> ValueHandle = PropertyHandle->GetChildHandle("Value");
		if (ValueHandle)
		{
			const FString MetaClassName = PropertyHandle->GetMetaData("MetaClass");
			UClass* MetaClass = !MetaClassName.IsEmpty() ? FEditorClassUtils::GetClassFromString(MetaClassName)
														 : UObject::StaticClass();
			IDetailPropertyRow& ValueProp = ChildBuilder.AddProperty(ValueHandle.ToSharedRef());
			TSharedPtr<SWidget> NameWidget;
			TSharedPtr<SWidget> ContentWidget;
			ValueProp.GetDefaultWidgets(NameWidget, ContentWidget,false);
			FDetailWidgetRow& WidgetRow = ValueProp.CustomWidget();
			WidgetRow.NameContent()[ValueHandle->CreatePropertyNameWidget()];
			WidgetRow.ValueContent().HAlign(HAlign_Fill)[SNew(SClassPropertyEntryBox)
										 .MetaClass(MetaClass)
										 .SelectedClass_Static(FClassPathOverrideDetailsCustomization::OnGetClass,
															ValueHandle.ToSharedRef())
										 .OnSetClass_Static(FClassPathOverrideDetailsCustomization::OnSetClass,
															ValueHandle.ToSharedRef())];
		}
		else
		{
			ChildBuilder.AddCustomRow(FText::FromString("Error"))
				.WholeRowContent()[SNew(STextBlock).Text(FText::FromString("Unable to find property handle!"))];
		}
	};
};
