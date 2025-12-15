/*
*  Copyright (C) 2025 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue/blob/main/LICENSE>)
 *
 */

#include "UI/EventHandlers/IModioUIModCollectionInfoReceiver.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IModioUIModCollectionInfoReceiver)

void IModioUIModCollectionInfoReceiver::ModCollectionInfoRequestHandler(FModioModCollectionID ID,
                                                                        FModioErrorCode ErrorCode,
                                                                        TOptional<FModioModCollectionInfo> Info)
{
	bRoutedUIModCollectionInfoReceiver = false;
	NativeOnModCollectionInfoRequestCompleted(ID, ErrorCode, Info);
	checkf(bRoutedUIModCollectionInfoReceiver,
	       TEXT(
		       "NativeOnModCollectionInfoRequestCompleted should call Super::NativeOnModCollectionInfoRequestCompleted"
		       "to route events to blueprint"));
}

void IModioUIModCollectionInfoReceiver::ListModCollectionsRequestHandler(FString RequestIdentifier,
                                                                         FModioErrorCode ErrorCode,
                                                                         TOptional<FModioModCollectionInfoList> List)
{
	bRoutedUIModCollectionInfoReceiver = false;
	NativeOnListModCollectionsRequestCompleted(RequestIdentifier, ErrorCode, List);
	checkf(bRoutedUIModCollectionInfoReceiver,
	       TEXT(
		       "NativeOnListModCollectionsRequestCompleted should call Super::NativeOnListModCollectionsRequestCompleted "
		       "to route events to blueprint"));
}

void IModioUIModCollectionInfoReceiver::GetModCollectionModsRequestHandler(FModioModCollectionID CollectionID,
																		   FModioErrorCode ErrorCode,
																		   TOptional<FModioModInfoList> Mods)
{
	bRoutedUIModCollectionInfoReceiver = false;
	NativeOnModCollectionModsRequestCompleted(CollectionID, ErrorCode, Mods);
	checkf(
		bRoutedUIModCollectionInfoReceiver,
		TEXT("NativeOnListModCollectionsRequestCompleted should call Super::NativeOnListModCollectionsRequestCompleted "
			 "to route events to blueprint"));
}

void IModioUIModCollectionInfoReceiver::ModCollectionInfoRequestHandlerK2Helper(FModioModCollectionID ID,
	FModioErrorCode ErrorCode, TOptional<FModioModCollectionInfo> Info, TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		// Will return nullptr if the object does not implement the interface through C++
		void* RawInterfacePtr = ImplementingObject->GetNativeInterfaceAddress(
			UModioUIModCollectionInfoReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIModCollectionInfoReceiver* ConcretePtr = static_cast<IModioUIModCollectionInfoReceiver*>(
				RawInterfacePtr);
			// Invoke the C++ implementation, this will bubble up to blueprint if need be
			ConcretePtr->ModCollectionInfoRequestHandler(ID, ErrorCode, Info);
		}
		else
		{
			// the interface wasn't implemented in C++ anywhere so go straight to blueprint, this will silently and
			// safely fail if the implementing class doesn't implement the interface
			Execute_OnModCollectionInfoRequestCompleted(ImplementingObject.Get(), ID, ErrorCode,
			                                            FModioOptionalModCollectionInfo{Info});
		}
	}
}

void IModioUIModCollectionInfoReceiver::ListModCollectionsRequestHandlerK2Helper(FString RequestIdentifier,
	FModioErrorCode ErrorCode, TOptional<FModioModCollectionInfoList> List,
	TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		void* RawInterfacePtr = ImplementingObject->GetNativeInterfaceAddress(
			UModioUIModCollectionInfoReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIModCollectionInfoReceiver* ConcretePtr = static_cast<IModioUIModCollectionInfoReceiver*>(
				RawInterfacePtr);
			ConcretePtr->ListModCollectionsRequestHandler(RequestIdentifier, ErrorCode, List);
		}
		else
		{
			Execute_OnListModCollectionsRequestCompleted(ImplementingObject.Get(), RequestIdentifier, ErrorCode,
			                                             FModioOptionalModCollectionInfoList(MoveTemp(List)));
		}
	}
}

void IModioUIModCollectionInfoReceiver::GetModCollectionModsRequestHandlerK2Helper(
	FModioModCollectionID CollectionID, FModioErrorCode ErrorCode, TOptional<FModioModInfoList> Mods,
	TWeakObjectPtr<UObject> ImplementingObject)
{
	if (ImplementingObject.IsValid())
	{
		void* RawInterfacePtr =
			ImplementingObject->GetNativeInterfaceAddress(UModioUIModCollectionInfoReceiver::StaticClass());
		if (RawInterfacePtr != nullptr)
		{
			IModioUIModCollectionInfoReceiver* ConcretePtr =
				static_cast<IModioUIModCollectionInfoReceiver*>(RawInterfacePtr);
			ConcretePtr->GetModCollectionModsRequestHandler(CollectionID, ErrorCode, Mods);
		}
		else
		{
			Execute_OnGetModCollectionModsRequestCompleted(ImplementingObject.Get(), CollectionID, ErrorCode,
														 FModioOptionalModInfoList(MoveTemp(Mods)));
		}
	}
}

void IModioUIModCollectionInfoReceiver::NativeOnModCollectionInfoRequestCompleted(FModioModCollectionID ID,
	FModioErrorCode ErrorCode,
	TOptional<FModioModCollectionInfo> Info)
{
	bRoutedUIModCollectionInfoReceiver = true;
	Execute_OnModCollectionInfoRequestCompleted(Cast<UObject>(this), ID, ErrorCode,
	                                            FModioOptionalModCollectionInfo{Info});
}

void IModioUIModCollectionInfoReceiver::NativeOnListModCollectionsRequestCompleted(FString RequestIdentifier,
	FModioErrorCode ErrorCode,
	TOptional<FModioModCollectionInfoList> List)
{
	bRoutedUIModCollectionInfoReceiver = true;
	Execute_OnListModCollectionsRequestCompleted(Cast<UObject>(this), RequestIdentifier, ErrorCode,
	                                             FModioOptionalModCollectionInfoList(MoveTemp(List)));
}

void IModioUIModCollectionInfoReceiver::NativeOnModCollectionModsRequestCompleted(FModioModCollectionID CollectionID,
																				  FModioErrorCode ErrorCode,
																				  TOptional<FModioModInfoList> Mods)
{
	bRoutedUIModCollectionInfoReceiver = true;
	Execute_OnGetModCollectionModsRequestCompleted(Cast<UObject>(this), CollectionID, ErrorCode,
												 FModioOptionalModInfoList(MoveTemp(Mods)));
}

void UModioUIModCollectionInfoReceiverLibrary::RegisterModCollectionInfoReceiver(
	UObject* ObjectToRegister,
                                                                       EModioUIModCollectionInfoEventType EventType)
{
	if (ObjectToRegister && ObjectToRegister->Implements<UModioUIModCollectionInfoReceiver>())
	{
		IModioUIModCollectionInfoReceiver::RegisterFromK2(ObjectToRegister, EventType);
	}
}

void UModioUIModCollectionInfoReceiverLibrary::DeregisterModCollectionInfoReceiver(
	UObject* ObjectToDeregister,
                                                                         EModioUIModCollectionInfoEventType EventType)
{
	if (ObjectToDeregister && ObjectToDeregister->Implements<UModioUIModCollectionInfoReceiver>())
	{
		IModioUIModCollectionInfoReceiver::DeregisterFromK2(ObjectToDeregister, EventType);
	}
}