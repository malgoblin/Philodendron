// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource.h"
#include "MainGameStateBase.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "WorkerHub.h"
#include "EnumActionList.h"
#include "EnumResourceType.h"
#include "EnumInteractableActorType.h"
#include "ManagerResources.h"

AResource::AResource()
{
	PrimaryActorTick.bCanEverTick = true;
	this->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ResourceFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Resource"));
}

void AResource::BeginPlay()
{
	Super::BeginPlay();
}


void AResource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AResource::InitResource(EResourceType Type, const FResourceParametrs& ResourceParam)
{
	ResourceType = Type;

	ResourceFlipbook = ResourceParam.ResourceFlipbook;
	ResourceFlipbookComponent->CastShadow = true;

	if (!ResourceFlipbook)
	{
		return false;
	}

	ResourceFlipbookComponent->SetFlipbook(ResourceFlipbook);

	ResourceWeight = ResourceParam.ResourceWeight;

	return true;
}

FVector AResource::GetLocation() const
{
	return this->GetActorLocation();
}

EResourceType AResource::GetResourceType() const
{
	return ResourceType;
}

float AResource::GetResourceWeight() const
{
	return ResourceWeight;
}

void AResource::RemoveResource()
{
	this->SetActorHiddenInGame(true);
	//Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->RemoveExistedResource(this);
}

const TObjectPtr<UPaperFlipbook> AResource::GetIActorPaperFlipbook() const
{
	return ResourceFlipbookComponent->GetFlipbook();
}
