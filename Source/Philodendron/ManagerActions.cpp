// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerActions.h"
#include "MainGameStateBase.h"

AManagerActions::AManagerActions()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AManagerActions::BeginPlay()
{
	Super::BeginPlay();
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SetManagerActions(this);
}

void AManagerActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AManagerActions::GetActionSize(EActionType Type) const
{
	if (!ActionParametrs.Contains(Type))
	{
		return -1;
	}
	return (*ActionParametrs.Find(Type)).MemorySize;
}

float AManagerActions::GetActionTime(EActionType Type) const
{
	if (!ActionParametrs.Contains(Type))
	{
		return -1;
	}
	return (*ActionParametrs.Find(Type)).TimeToMake;
}

int32 AManagerActions::GetDistanceToMakeAction(EActionType Type) const
{
	if (!ActionParametrs.Contains(Type))
	{
		return -1;
	}
	return (*ActionParametrs.Find(Type)).DistanceToMakeAction;
}

bool AManagerActions::NeedInteractableActorForAction(EActionType Type) const
{
	if (!ActionParametrs.Contains(Type))
	{
		return false;
	}
	return (*ActionParametrs.Find(Type)).bNeededInteractableActor;
}

const TMap<EWorkerAddonType, int32>* AManagerActions::GetConditionAddons(EActionType Type) const
{
	if (!ActionParametrs.Contains(Type))
	{
		return nullptr;
	}
	return &((*ActionParametrs.Find(Type)).ConditionAddons);
}

const TArray<EResourceType>* AManagerActions::GetResourcesMinedAction(EActionType Type) const
{
	if (!ActionParametrs.Contains(Type))
	{
		return nullptr;
	}
	return &((*ActionParametrs.Find(Type)).ResourcesMinedThisAction);
}

const FString* AManagerActions::GetActionName(EActionType Type, EPossibleLanguages Languages) const
{
	if (!ActionParametrs.Contains(Type))
	{
		return nullptr;
	}

	return ActionParametrs[Type].ActionName.Find(Languages);
}