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

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "ModioUIComponentStatics.generated.h"

// May not need to be in a runtime module

UENUM(BlueprintType)
enum class EModioUIComponentID : uint8
{
	Button,
	StaticText,
	EditableText,
	MultilineEditableText,
	CodeInputText,
	EnumSelector,
	PresetFilterSelector,
	ModTile,
	ModalDialog,
	Image,
	ProgressWidget,
	CheckBox,
	ModList,
	TagDisplay,
	ObjectList,
	EnumSelectorEntry,
	CommandMenu,
	FilterSelectorEntry,
	ModPropertyCollection,
	UserDisplay
};

/**
 * @brief Internal-only base type for UI Component metadata
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIComponentMetadata
{
	GENERATED_BODY()

	/**
	 * @brief Array of Interface classes that must be implemented by the component
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "mod.io|UI|ComponentMetadata")
	TArray<UClass*> RequiredInterfaces;

	/**
	 * @brief The associated EModioUIComponentID for this component
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "mod.io|UI|ComponentMetadata")
	EModioUIComponentID ComponentID;

	/**
	 * @brief The display name for this component
	 */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "mod.io|UI|ComponentMetadata")
	FName ComponentDisplayName;
};

/**
 * @base_component
 * @brief The button component is a base component in the mod.io Component UI framework. It implements interfaces
 * to support data binding as well as click, selection and hover events.
 * @component_display_name Button
 * @required_interface IModioUIClickableWidget|Allows the widget to emit events when clicked
 * @required_interface IModioUISelectableWidget|Allows the widget to be configured as selectable
 * @required_interface IModioUIHoverableWidget|Allows the widget to emit events when hovered
 * @required_interface IModioUIHasTooltipWidget|Allows the widget to have its tooltip externally configured
 * @required_interface IModioUIDataSourceWidget|Allows the widget to data-bind to a UObject*
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIButtonComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIButtonComponentMetadata();
};

/**
 * @base_component
 * @brief The Text component is a base component in the mod.io Component UI framework for visualizing a piece of
 * text. The requirements for a Text component are very minimal - just the implementation of a single interface to allow
 * for the value of the text to be set or queried at runtime.
 * @component_display_name Text
 * @required_interface IModioUIHasTextWidget|Allows the widget to have its associated text externally configured
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUITextComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUITextComponentMetadata();
};

/**
 * @base_component
 * @brief The Editable Text component is a widget that gathers user-specified text input. It optionally can be
 * configured with validation rules to use to check the user input for conformity.
 * @component_display_name Editable Text
 * @required_interface IModioUIStringInputWidget|Allows the widget to have its user input retrieved, and the hint text configured
 * @required_interface IModioUITextValidator|Allows the widget to be externally configured with validation rules, or to
 * be queried for the results of validation
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIEditableTextComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIEditableTextComponentMetadata();
};

/**
 * @base_component
 * @brief The Image component is a widget that displays an externally-configurable image.
 * @component_display_name Image
 * @required_interface IModioUIImageDisplayWidget|Allows the widget to have its associated image/brush externally
 * configured
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIImageComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIImageComponentMetadata();
};

/**
 * @base_component
 * @brief The Progress component is a widget that visualizes a float value between 0-1 as progress, with 0 representing
 * no progress and 1 representing completion.
 * @component_display_name Progress
 * @required_interface IModioUIProgressWidget|Allows the widget to have its associated progress value set and queried
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIProgressBarComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIProgressBarComponentMetadata();
};

/**
 * @base_component
 * @brief The Tag component is a widget that visualizes a single tag from a mod.
 * @component_display_name Tag
 * @required_interface IModioUIHasTextWidget|Allows the widget to have its associated label configured with the text
 * representation of the tag
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUITagComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUITagComponentMetadata();
};

/**
 * @base_component
 * @brief The Object List component is a widget that visualizes a group of objects in some fashion. It allows external
 * registration of a delegate that will be called any time a widget is created to visualize an element in the associated
 * object array.
 * @component_display_name Object List
 * @required_interface IModioUIObjectListWidget|Allows the set of objects to visualize to be configured externally and
 * allows for callback to be registered for internal widget creation/destruction
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIObjectListComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIObjectListComponentMetadata();
};

/**
 * @base_component
 * @brief The Mod List component is a specialization of Object List that visualizes a group of mods in some fashion. It
 * requires the objects given to the component, to implement IModioModInfoUIDetails.
 * @component_display_name Mod List
 * @required_interface IModioUIObjectListWidget|Allows the set of objects to visualize to be configured externally and
 * allows for callback to be registered for internal widget creation/destruction
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIModListComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIModListComponentMetadata();
};

/**
 * @base_component
 * @brief The Enum Selector component is a specialization of the Object Selector widget. Value objects passed to the
 * selector should implement IModioEnumEntryUIDetails so that they can be queried for an associated entry in an enum.
 * @component_display_name Enum Selector
 * @required_interface IModioUIObjectSelector|Allows the widget to internally track the selection state of the passed-in
 * enum values, which can be externally queried
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIEnumSelectorComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIEnumSelectorComponentMetadata();
};

/**
 * @base_component
 * @brief The Enum Selector Entry component is a specialization of the Object Selector Entry that visualizes an enum
 * value and allows it to be selected.
 * @component_display_name Enum Selector Entry
 * @required_interface IUserObjectListEntry|Required to allow Entry components to be used as visualization widgets in
 * standard Unreal list and tile views
 * @required_interface IModioUIClickableWidget|Allows the widget to emit events when clicked to allow the enum selector
 * to trigger a selection change
 * @required_interface IModioUISelectableWidget|Allows the widget to internally track the selection state of the
 * bound enum value, which can be externally queried *
 * @required_interface IModioUIDataSourceWidget|Allows the widget to be data-bound to a UObject implementing
 * IModioEnumEntryUIDetails
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIEnumSelectorEntryComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIEnumSelectorEntryComponentMetadata();
};

/**
 * @base_component
 * @brief The Preset Filter Selector component is a specialization of Object Selector that visualizes a group of objects
 * that implement IModioModFilterUIDetails in some fashion. It allows other widgets to register for notifications when a
 * user selects a preset filter from the list.
 * @component_display_name Preset Filter Selector
 * @required_interface IModioUIObjectSelector|Allows the selection state of the preset filters to be queried or set
 * externally
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIPresetFilterSelectorComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIPresetFilterSelectorComponentMetadata();
};

/**
 * @base_component
 * @brief The Preset Filter Selector Entry component is a specialization of the Object Selector Entry that visualizes a
 * named preconfigured set of filter parameters and allows it to be selected.
 * @component_display_name Preset Filter Selector Entry
 * @required_interface IUserObjectListEntry|Required to allow Entry components to be used as visualization widgets in
 * standard Unreal list and tile views
 * @required_interface IModioUIClickableWidget|Allows the widget to emit events when clicked to allow the filter
 * selector to trigger a selection change
 * @required_interface IModioUISelectableWidget|Allows the widget to internally track the selection state of the
 * bound filter parameters, which can be externally queried
 * @required_interface IModioUIDataSourceWidget|Allows the widget to be data-bound to a UObject implementing
 * IModioModFilterUIDetails
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIPresetFilterSelectorEntryComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIPresetFilterSelectorEntryComponentMetadata();
};

/**
 * @base_component
 * @brief The Command Menu component is a widget that visualizes a set of UObjects implementing IModioUICommandObject.
 * @component_display_name Command Menu
 * @required_interface IModioUICommandMenu|Allows the implementing widget to be externally configured with menu builder
 * delegates, and external triggering of a refresh/rebuild of the set of commands via the menu builders.
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUICommandMenuComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUICommandMenuComponentMetadata();
};

/**
 * @base_component
 * @brief The Mod Property Collection Visualizer is a widget that visualizes a set of properties for a FModioModInfo
 * object.
 * @component_display_name Mod Property Collection Visualizer
 * @required_interface IModioUIDataSourceWidget|Allows the implementing widget to be externally configured with a UObject
 * that implements IModioModInfoUIDetails, which will be queried by the individual property visualizers this widget
 * manages.
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIModPropertyCollectionVisualizerComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIModPropertyCollectionVisualizerComponentMetadata();
};

/**
 * @base_component
 * @brief The Mod Tile is a specialization of Button that visualizes a FModioModInfo and is suitable for use as an item
 * widget in Mod List and Mod Tile View components.
 * @component_display_name Mod Tile
 * @required_interface IUserObjectListEntry|Required to allow Mod Tile components to be used as visualization widgets in
 * Mod List View and Mod Tile View components
 * @required_interface IModioUIDataSourceWidget|Allows the implementing widget to be externally configured with a UObject
 * that implements IModioModInfoUIDetails, which can then be displayed on this tile.
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIModTileComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIModTileComponentMetadata();
};

/**
 * @base_component
 * @brief The User Display component is a widget that visualizes an FModioUser object.
 * @component_display_name User Display
 * @required_interface IModioUIDataSourceWidget|Allows the implementing widget to be externally configured with a UObject
 * that implements IModioUserUIDetails, which will be queried for properties this widget needs to display.
 */
USTRUCT(BlueprintType, Meta=(NoClassDoc))
struct FModioUIUserDisplayComponentMetadata : public FModioUIComponentMetadata
{
	GENERATED_BODY()

	FModioUIUserDisplayComponentMetadata();
};

/**
 *
 */
UCLASS()
class MODIOUICORE_API UModioUIComponentStatics : public UObject
{
	GENERATED_BODY()
protected:
public:
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Component Statics")
	static const TMap<EModioUIComponentID, FModioUIComponentMetadata>& GetAllComponentMetadata();

	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Component Statics")
	static const FModioUIComponentMetadata& GetMetadataForComponent(EModioUIComponentID ComponentType,
																	bool& bComponentTypeFound);
};
