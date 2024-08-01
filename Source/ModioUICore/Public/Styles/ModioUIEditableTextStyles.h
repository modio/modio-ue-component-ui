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

#include "UObject/Interface.h"
#include "Styling/SlateTypes.h"

#include "ModioUIEditableTextStyles.generated.h"

// TODO: Delete these classes if they are not used

UCLASS(Blueprintable, BlueprintType)
class MODIOUICORE_API UModioEditableTextBoxStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "mod.io|UI|Editable Text Box Style")
	FEditableTextBoxStyle Style;
};


UCLASS(Blueprintable)
class MODIOUICORE_API UModioMultiLineEditableTextBoxStyle : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "mod.io|UI|Multiline Editable Text Box Style")
	FEditableTextBoxStyle Style;

	/** The text style */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "mod.io|UI|Multiline Editable Text Box Style")
	FTextBlockStyle TextStyle;
};
