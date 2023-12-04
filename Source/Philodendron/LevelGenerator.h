// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumInteractableActorType.h"
#include "EnumAnimationType.h"
#include "EnumWorkerAddonType.h"
#include "ManagerInteractableActors.h"
#include "LevelGenerator.generated.h"

class UPaperTileMap;
class UPaperTileMapComponent;
class AMapHelperToCreate;

USTRUCT()
struct FPriorityForNeighbours
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 UpperBoundOfTheGroupCountNeighbours = 0;

	UPROPERTY(EditAnywhere)
	int32 Priority = 0;
};

USTRUCT()
struct FGroupingInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float GroupingRadius = 0.f;

	UPROPERTY(EditAnywhere)
	TArray<FPriorityForNeighbours> PriorityForNeighbours;
};

UCLASS()
class PHILODENDRON_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelGenerator();
	void GenerateLevel();
	virtual void Tick(float DeltaTime) override;

	const TArray<TObjectPtr<AMapHelperToCreate>>& GetMapHelperList() const;
	int32 GetSeedForMapTileGeneration() const;

	void GenerateMap(int32 RandomSeed);

protected:
	virtual void BeginPlay() override;

private:	
	void ChangeIncreaseGridElementsInRadius(TArray<TArray<int32>>& Grid, int32 X, int32 Y, int32 RadiusSq, int32 Value, bool IsChange);
	bool TryCreateObj(TArray<TArray<int32>>& Grid, int32 X, int32 Y, int32 RadiusSq, EInteractableActorType IAType, int32 ObjectNumber, EInteractableActorCreatingType CreatingType);
	bool CheckGridPlace(TArray<TArray<int32>>& Grid, int32 X, int32 Y, int32 RadiusSq);
	void ChangeObjPriority(TArray<int32>& ObjPriority, TArray<TArray<int32>>& Grid, EInteractableActorType AType);
	void GenerateInteractableActors(TArray<TArray<int32>>& Grid);
	void ClearGridAfterCreateIActor(TArray<TArray<int32>>& Grid);


private:
	UPROPERTY(EditAnywhere)
	TMap<EInteractableActorType, int32> ObjectCountForGenerate;

	UPROPERTY(EditAnywhere)
	TMap<EInteractableActorType, EInteractableActorCreatingType> TypeToGenerateObject;

	UPROPERTY(EditAnywhere)
	float RadiusForGenerate = 100.f;

	UPROPERTY(EditAnywhere)
	float DeathAreaAroundCore = 50.f;

	UPROPERTY(EditAnywhere)
	TMap<EInteractableActorType, FGroupingInfo> Neighbours;

	UPROPERTY(EditAnywhere)
	int32 SmallRadiusForObject = 25.f;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UPaperTileMap>> TileMap;

	UPROPERTY(EditAnywhere)
	int32 MapSizeInCountTale = 10;

	UPROPERTY(EditAnywhere)
	int32 SizeOneTale= 64;

	UPROPERTY(EditAnywhere)
	TMap<EWorkerAddonType, int32> StartedAddons;

	UPROPERTY(EditAnywhere)
	int32 StartedWorkers = 5;

	UPROPERTY()
	TArray<TObjectPtr<AMapHelperToCreate>> MapHelpers;

	UPROPERTY()
	int32 SeedForMapTileGeneration = 0;
};
