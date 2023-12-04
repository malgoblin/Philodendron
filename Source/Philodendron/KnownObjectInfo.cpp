// Fill out your copyright notice in the Description page of Project Settings.


#include "KnownObjectInfo.h"
#include "EnumInteractableActorType.h"
#include "InteractableActor.h"

UKnownObjectInfo::UKnownObjectInfo()
{
}

const FVector& UKnownObjectInfo::GetKnownObjectLocation() const
{
	return KnownInfo.KnownObjectLocation;
}

int32 UKnownObjectInfo::GetSizeData() const
{
	return KnownInfo.DataSize;
}

void UKnownObjectInfo::SetInfo(FSupportKnownObjectInfo&& Info)
{
	KnownInfo = Info;
}

void UKnownObjectInfo::SetWeakObjectPtr(TWeakObjectPtr<AInteractableActor> Actor)
{
	IActor = Actor;
}

EInteractableActorType UKnownObjectInfo::GetActorType() const
{
	return KnownInfo.ActorType;
}

const TWeakObjectPtr<AInteractableActor> UKnownObjectInfo::GetInteractableActor() const
{
	return IActor;
}

bool UKnownObjectInfo::HasAction(EActionType Type) const
{
	return KnownInfo.ActionList.Contains(Type);
}

const FSupportKnownObjectInfo& UKnownObjectInfo::GetKnownObjectInfo() const
{
	return KnownInfo;
}