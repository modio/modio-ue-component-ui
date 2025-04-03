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

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "Engine/DataTable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Styling/SlateBrush.h"
#include "UObject/Interface.h"

#include "IModioUICommandMenu.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FModioCommandListBuilder, TArray<UObject*>&, CommandList, UObject*, Context);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioCommandListBuilderMulticast, TArray<UObject*>, CommandList,
											 UObject*, Context);
// clang-format off


/**
 * @brief Interface indicating the implementing widget is responsible for visualising a list of dynamically generated
 * command objects
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Command Menu",
		   Meta = (DocGroup = "UI", Comment = "@brief Interface indicating the implementing widget is responsible for visualising a list of dynamically generated command objects"))
class MODIOUICORE_API UModioUICommandMenu : public UInterface
{
	GENERATED_BODY()
};

// clang-format on

class IModioUICommandMenu
{
	GENERATED_BODY()

public:
	/**
	 * @brief Registers a delegate to be invoked when RebuildCommandList is called
	 * @param Builder Delegate that will be invoked to allow the command list to be altered
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Menu")
	void AddCommandListBuilder(const FModioCommandListBuilder& Builder);

	/**
	 * @brief Unregisters a delegate so it will no longer be invoked when RebuildCommandList is called
	 * @param Builder Delegate that will no longer be invoked
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Menu")
	void RemoveCommandListBuilder(const FModioCommandListBuilder& Builder);

	/**
	 * @brief Requests the implementing widget invoke all the bound command list builders to regenerate the menu options
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Menu")
	void RebuildCommandList();

	// Getter method to retrieve the array?
};

/**
 * @modio_ui_component_interface
 * @brief Interface indicating the implementing object encapsulates information about a command that can be executed
 */
UINTERFACE(BlueprintType, Category = "mod.io|UI|Command Object")
class MODIOUICORE_API UModioUICommandObject : public UInterface
{
	GENERATED_BODY()
};

class IModioUICommandObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets an associated UObject that can be used as a data source/context object for this command
	 * @param InDataSource The object to use as a context
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Object")
	void SetDataSource(UObject* InDataSource);

	/**
	 * @brief Retrieves the text to display for this command
	 * @return Localized display string for this command
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Object")
	FText GetCommandDisplayString();

	/**
	 * @brief Retrieves the brush to use as an icon for this command
	 * @return The brush to use as an icon
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Object")
	FSlateBrush GetCommandBrush();

	/**
	 * @brief Indicates if the command can currently be executed
	 * @param Context Additional secondary context object for this command
	 * @return True if the command can be executed
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Object")
	bool CanExecute(UObject* Context);

	/**
	 * @brief Performs the command associated with this object
	 * @param Context Additional secondary context object for this command
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "mod.io|UI|Command Object")
	void Execute(UObject* Context);
};

/**
 * @brief Data table struct containing information for a specific mod.io UI Command
 */
USTRUCT(BlueprintType, Category = "mod.io")
struct MODIOUICORE_API FModioUICommandAssetEntry : public FTableRowBase
{
	GENERATED_BODY();

	/**
	 * @brief Map of command-specific key names to command-specific localized text
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io")
	TMap<FName, FText> CommandTexts;

	/**
	 * @brief Map of command-specific key names to command-specific brushes for use as icons
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "mod.io")
	TMap<FName, FSlateBrush> CommandIconBrushes;
};

/**
 * @brief Library allowing easy access to mod.io UI Command data on a per-command basis
 */
UCLASS()
class MODIOUICORE_API UModioUICommandLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Retrieves a data table row for the specified command
	 * @param CommandID ID for the command to retrieve
	 * @param bResolved Set to true if the command was found in the data table
	 * @return The resolved data table row, or an empty row if bResolved is false
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Commands")
	static const FModioUICommandAssetEntry& GetCommandAssetEntry(const FString& CommandID, bool& bResolved);

	/**
	 * @brief Retrieves a specific localized string for the specified command
	 * @param CommandID ID for the command to search within
	 * @param TextName Name of the string to retrieve
	 * @param bResolved Set to true if the string was found in the data table
	 * @return The resolved text, or an empty one if bResolved is false
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Commands")
	static const FText& GetCommandText(const FString& CommandID, const FName& TextName, bool& bResolved);

	/**
	 * @brief Retrieves a specific brush for the specified command
	 * @param CommandID ID for the command to search within
	 * @param BrushName Name of the brush to retrieve
	 * @param bResolved Set to true if the brush was found in the data table
	 * @return The resolved brush, or an empty one if bResolved is false
	 */
	UFUNCTION(BlueprintCallable, Category = "mod.io|UI|Commands")
	static const FSlateBrush& GetCommandBrush(const FString& CommandID, const FName& BrushName, bool& bResolved);
};
