// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerResources.h"
#include "Resource.h"
#include "MainGameStateBase.h"

// Sets default values
AManagerResources::AManagerResources()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AManagerResources::BeginPlay()
{
	Super::BeginPlay();
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SetManagerResources(this);
}

void AManagerResources::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TObjectPtr<AResource> AManagerResources::CreateNewResource(EResourceType ResourceType, const FVector& Location, const FRotator& Rotation)
{
	if (!ResourcesParametrs.Contains(ResourceType))
	{
		return nullptr;
	}
	FVector ResLocation = Location;
	ResLocation.Z = 1;

	TObjectPtr<AResource> NewResource = this->GetWorld()->SpawnActor<AResource>(AResource::StaticClass(), ResLocation, Rotation);

	if (!NewResource)
	{
		return nullptr;
	}

	if (!NewResource->InitResource(ResourceType, *ResourcesParametrs.Find(ResourceType)))
	{
		return nullptr;
	}

	return NewResource;
}