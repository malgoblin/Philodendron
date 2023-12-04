// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerMap.h"
#include "KnownObjectInfo.h"
#include "InteractableActor.h"

int32 UWorkerMap::GetSizeData() const
{
	return Size;
}

const TArray<TObjectPtr<UKnownObjectInfo>>& UWorkerMap::GetMapInfo() const
{
	return InfoList;
}

void UWorkerMap::AddInfoListToMap(const TArray<TObjectPtr<UKnownObjectInfo>>& List)
{
	InfoList = List;
}

bool UWorkerMap::HasObject(const TWeakObjectPtr<AInteractableActor> Object)  const
{
	for (const auto& Obj : InfoList)
	{
		if (Obj->GetInteractableActor() == Object)
			return true;
	}
	return false;
}

TArray<TWeakObjectPtr<AInteractableActor>> UWorkerMap::GetKnownInteractableActor(EInteractableActorType Type) const
{
	TArray<TWeakObjectPtr<AInteractableActor>> ActorList;
	for (const auto& Info : InfoList)
	{
		if (Info->GetActorType() == Type)
		{
			ActorList.Add(Info->GetInteractableActor());
		}
	}
	return ActorList;
}

int32 UWorkerMap::GetMapID() const
{
	return MapID;
}

void UWorkerMap::SetMapID(int32 ID)
{
	MapID = ID;
}