/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/Components/ModioUIComponentStatics.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "UI/Interfaces/IModioUIClickableWidget.h"
#include "UI/Interfaces/IModioUICommandMenu.h"
#include "UI/Interfaces/IModioUIDataSourceWidget.h"
#include "UI/Interfaces/IModioUIHasTextWidget.h"
#include "UI/Interfaces/IModioUIHasTooltipWidget.h"
#include "UI/Interfaces/IModioUIHoverableWidget.h"
#include "UI/Interfaces/IModioUIImageDisplayWidget.h"
#include "UI/Interfaces/IModioUIModListViewInterface.h"
#include "UI/Interfaces/IModioUIObjectListWidget.h"
#include "UI/Interfaces/IModioUIObjectSelector.h"
#include "UI/Interfaces/IModioUIProgressWidget.h"
#include "UI/Interfaces/IModioUISelectableWidget.h"
#include "UI/Interfaces/IModioUIStringInputWidget.h"
#include "UI/Interfaces/IModioUITextValidator.h"

const TMap<EModioUIComponentID, FModioUIComponentMetadata>& UModioUIComponentStatics::GetAllComponentMetadata()
{
	static TMap<EModioUIComponentID, FModioUIComponentMetadata> ComponentRequirementMetadata = []() {
		// clang-format off
		return TMap<EModioUIComponentID, FModioUIComponentMetadata>(
			{
				{
					EModioUIComponentID::Button,
					{{
						UModioUIClickableWidget::StaticClass(), 
						UModioUISelectableWidget::StaticClass(),
						UModioUIHasTextWidget::StaticClass(), 
						UModioUIHasTooltipWidget::StaticClass(),
						UModioUIHoverableWidget::StaticClass()
					}}
				},
				{
					EModioUIComponentID::StaticText, 
					{{
						UModioUIHasTextWidget::StaticClass()
					}}
				},
				{
					EModioUIComponentID::EditableText,
					{{
						UModioUIStringInputWidget::StaticClass(), 
						UModioUITextValidator::StaticClass()
					}}
				},
				{
					EModioUIComponentID::MultilineEditableText,
					{{
						UModioUIStringInputWidget::StaticClass(),
						UModioUITextValidator::StaticClass()
					}}
				},
				{
					EModioUIComponentID::CodeInputText,
					{{
						UModioUIStringInputWidget::StaticClass(),
						UModioUITextValidator::StaticClass()
					}}
				},
				{
					EModioUIComponentID::ModTile,
					{{
						UModioUIClickableWidget::StaticClass(),
						UModioUISelectableWidget::StaticClass(),
						UModioUIHoverableWidget::StaticClass(),
						UModioUIDataSourceWidget::StaticClass()
					}}
				},
				{
					EModioUIComponentID::PresetFilterSelector,
					{{
						UModioUIObjectSelector::StaticClass()
					}}
				},
				{
					EModioUIComponentID::Image,
					{{
						UModioUIImageDisplayWidget::StaticClass()
					}}
				},
				{
					EModioUIComponentID::ProgressWidget,
					{{
						UModioUIProgressWidget::StaticClass()
					}}
				},
				{
					EModioUIComponentID::CheckBox,
					{{
						UModioUISelectableWidget::StaticClass(),
						UModioUIHasTextWidget::StaticClass()
					}}
				},
				{
					EModioUIComponentID::ModList,
					{{
						UModioUIModListViewInterface::StaticClass(),
						UModioUIObjectListWidget::StaticClass()
					}}
				}
			});
		// clang-format on
	}();
	return ComponentRequirementMetadata;
}

const FModioUIComponentMetadata& UModioUIComponentStatics::GetMetadataForComponent(EModioUIComponentID ComponentType,
																				   bool& bComponentTypeFound)
{
	static FModioUIComponentMetadata Default {};

	bComponentTypeFound = GetAllComponentMetadata().Contains(ComponentType);
	if (bComponentTypeFound)
	{
		return GetAllComponentMetadata()[ComponentType];
	}
	else
	{
		return Default;
	}
}

// clang-format off

FModioUIButtonComponentMetadata::FModioUIButtonComponentMetadata()
	: FModioUIComponentMetadata 
	{
		{
			UModioUIClickableWidget::StaticClass(), 
			UModioUISelectableWidget::StaticClass(),
			UModioUIHoverableWidget::StaticClass(), 
			UModioUIHasTooltipWidget::StaticClass(),
			UModioUIDataSourceWidget::StaticClass()
		},
		EModioUIComponentID::Button,
		FName("Button")
	}
{}

FModioUITextComponentMetadata::FModioUITextComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UModioUIHasTextWidget::StaticClass()
		},
		EModioUIComponentID::StaticText,
		FName("Text")
	}
{}


FModioUIEditableTextComponentMetadata::FModioUIEditableTextComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UModioUIStringInputWidget::StaticClass(),
			UModioUITextValidator::StaticClass()
		},
		EModioUIComponentID::EditableText,
		FName("Editable Text")
	}
{}
	
	
FModioUIImageComponentMetadata::FModioUIImageComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UModioUIImageDisplayWidget::StaticClass()
		},
		EModioUIComponentID::Image,
		FName("Image")
	}
{}


 FModioUIProgressBarComponentMetadata::FModioUIProgressBarComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UModioUIProgressWidget::StaticClass()
		},
		EModioUIComponentID::ProgressWidget,
		FName("Progress")
	}
{}

 FModioUITagComponentMetadata::FModioUITagComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UModioUIHasTextWidget::StaticClass()
		},
		EModioUIComponentID::TagDisplay,
		FName("Tag")
	}
{}

 FModioUIObjectListComponentMetadata::FModioUIObjectListComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UModioUIObjectListWidget::StaticClass()
		},
		EModioUIComponentID::ObjectList,
		FName("Object List")
	}
{}


 FModioUIModListComponentMetadata::FModioUIModListComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UModioUIObjectListWidget::StaticClass(),
			UModioUIModListViewInterface::StaticClass()
		},
		EModioUIComponentID::ModList,
		FName("Mod List")
	}
{}

FModioUIObjectSelectorComponentMetadata::FModioUIObjectSelectorComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UModioUIObjectSelector::StaticClass()
		},
		EModioUIComponentID::ObjectSelector,
		FName("Object Selector")
	}
{}

FModioUIEnumSelectorComponentMetadata::FModioUIEnumSelectorComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UModioUIObjectSelector::StaticClass()
		},
		EModioUIComponentID::EnumSelector,
		FName("Enum Selector")
	}
{}

 FModioUIEnumSelectorEntryComponentMetadata::FModioUIEnumSelectorEntryComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UUserObjectListEntry::StaticClass(),
			UModioUIClickableWidget::StaticClass(),
			UModioUISelectableWidget::StaticClass(),
			UModioUIDataSourceWidget::StaticClass()
		},
		EModioUIComponentID::EnumSelectorEntry,
		FName("Enum Selector Entry")
	}
{}

 FModioUIPresetFilterSelectorComponentMetadata::FModioUIPresetFilterSelectorComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UModioUIObjectSelector::StaticClass()
		},
		EModioUIComponentID::PresetFilterSelector,
		FName("Preset Filter Selector")
	}
{}

 FModioUICommandMenuComponentMetadata::FModioUICommandMenuComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UModioUICommandMenu::StaticClass()
		},
		EModioUIComponentID::CommandMenu,
		FName("Command Menu")
	}
{}

 FModioUIPresetFilterSelectorEntryComponentMetadata::FModioUIPresetFilterSelectorEntryComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UUserObjectListEntry::StaticClass(),
			UModioUIClickableWidget::StaticClass(),
			UModioUISelectableWidget::StaticClass(),
			UModioUIDataSourceWidget::StaticClass()
		},
		EModioUIComponentID::FilterSelectorEntry,
		FName("Filter Selector Entry")
	}
{}

FModioUIModPropertyCollectionVisualizerComponentMetadata::FModioUIModPropertyCollectionVisualizerComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UModioUIDataSourceWidget::StaticClass()
		},
		EModioUIComponentID::ModPropertyCollection,
		FName("Mod Property Collection Visualizer")
	}
{}


 FModioUIModTileComponentMetadata::FModioUIModTileComponentMetadata()
	: FModioUIComponentMetadata
	{	
		{
			UUserObjectListEntry::StaticClass(),
			UModioUIClickableWidget::StaticClass(),
			UModioUISelectableWidget::StaticClass(),
			UModioUIDataSourceWidget::StaticClass()
		},
		EModioUIComponentID::ModTile,
		FName("Mod Tile")
	}
{}

 FModioUIUserDisplayComponentMetadata::FModioUIUserDisplayComponentMetadata()
 : FModioUIComponentMetadata
	{	
		{
			UModioUIDataSourceWidget::StaticClass()
		},
		EModioUIComponentID::UserDisplay,
		FName("User Display")
	}
{
	
}
