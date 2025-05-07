/*
 *  Copyright (C) 2024 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */


#include "Core/ModioRetryCounter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ModioRetryCounter)

#if WITH_EDITORONLY_DATA
	#define DEBUG_RETRY_MESSAGE(Text) \
UE_LOG(LogTemp, Display, TEXT(#Text " %s"), *DebugName.ToString())
#else
	#define DEBUG_RETRY_MESSAGE(...)
#endif

void FModioRetryCounter::ResetImpl()
{
	RetriesRemaining = 3;

	const UModioSettings* SettingsInstance = GetDefault<UModioSettings>();
	if (SettingsInstance)
	{
		RetriesRemaining = SettingsInstance->GlobalRetryLimit;
	}
}

int32 FModioRetryCounter::GetRetriesRemaining() const
{
	return RetriesRemaining;
}

bool FModioRetryCounter::Retry()
{
	if (RetriesRemaining > 0)
	{
		DEBUG_RETRY_MESSAGE(Retry OK for);
		RetriesRemaining--;
		return true;
	}
	DEBUG_RETRY_MESSAGE(Retry failed for);
	return false;
}

void FModioRetryCounter::Reset()
{
	DEBUG_RETRY_MESSAGE(Resetting);

	ResetImpl();
}

FModioRetryCounter::FModioRetryCounter()
{
	ResetImpl();
}


bool UModioRetryLibrary::CanRetry(const FModioRetryCounter& Target) 
{
	return Target.GetRetriesRemaining() > 0;
}

bool UModioRetryLibrary::CanRetryAsExec(const FModioRetryCounter& Target) 
{
	return CanRetry(Target);
}

bool UModioRetryLibrary::Retry(FModioRetryCounter& Target) 
{
	return Target.Retry();
}

FModioRetryCounter& UModioRetryLibrary::ResetRetryCountPure(FModioRetryCounter& Target) 
{
	Target.Reset();
	return Target;
}

void UModioRetryLibrary::ResetRetryCount(FModioRetryCounter& Target) 
{
	Target.Reset();
}

