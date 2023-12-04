

#include "ManagerBuilding.h"
#include "Building.h"
#include "MainGameStateBase.h"


AManagerBuilding::AManagerBuilding()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AManagerBuilding::BeginPlay()
{
	Super::BeginPlay();
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SetManagerBuilding(this);
}

void AManagerBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TObjectPtr<ABuilding> AManagerBuilding::CreateNewBuilding(EBuildingType Type, const FVector& Location) const
{
	if (!BuildingParametrs.Contains(Type))
		return nullptr;

	FVector BuildLocation = Location;
	BuildLocation.Z = 1;

	TObjectPtr<ABuilding> NewBuilding = this->GetWorld()->SpawnActor<ABuilding>(ABuilding::StaticClass(), BuildLocation, {0, 0, 0});
	
	if (!NewBuilding)
		return nullptr;

	NewBuilding->InitBuilding(Type, &BuildingParametrs[Type].BuildingFlipbooks, BuildingParametrs[Type].DistanceForCreatResource);

	return NewBuilding;
}