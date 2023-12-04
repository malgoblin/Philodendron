// Fill out your copyright notice in the Description page of Project Settings.


#include "MapHelperToCreate.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"



AMapHelperToCreate::AMapHelperToCreate()
{

	PrimaryActorTick.bCanEverTick = false;

	TileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Map"));

	TileMapComponent->AddLocalRotation({ 180.f, 180.f, 90.f });
}

void AMapHelperToCreate::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMapHelperToCreate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapHelperToCreate::InitTile(const FVector& Possition, TObjectPtr<UPaperTileMap> TileMap, int32 NTMap)
{
	TileMapComponent->SetTileMap(TileMap);
	TileMapComponent->AddRelativeLocation(Possition);
	NumberTileMap = NTMap;
}

FVector AMapHelperToCreate::GetTilePossition() const
{
	return TileMapComponent->GetRelativeLocation();
}

int32 AMapHelperToCreate::GetTileNumber() const
{
	return NumberTileMap;
}