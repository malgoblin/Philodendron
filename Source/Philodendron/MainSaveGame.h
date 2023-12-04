// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EnumActionList.h"
#include "EnumRecipeType.h"
#include "SupportStructsForSaveLoad.h"
#include "EnumWorkerAddonType.h"
#include "EnumWorkerType.h"
#include "EnumResourceType.h"
#include "MainSaveGame.generated.h"

USTRUCT()
struct FWorkerMapInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 MapID = -1;

	UPROPERTY(EditAnywhere)
	TArray<FSupportKnownObjectInfo> KnownObjectInfo;
};

USTRUCT()
struct FCoreInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FVector CorePossition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	TArray<EActionType> ExistedActions;

	UPROPERTY(EditAnywhere)
	TArray<ERecipeType> ExistedRecipe;

	UPROPERTY(EditAnywhere)
	TArray<FSupportKnownObjectInfo> KnownObjectInfo;

	UPROPERTY(EditAnywhere)
	TArray<int32> AttachedPawns;

	UPROPERTY(EditAnywhere)
	TArray<int32> AttachedHubs;

	UPROPERTY(EditAnywhere)
	TArray<FWorkerMapInfo> WorkerMapList;

	UPROPERTY(EditAnywhere)
	TArray<EWorkerAddonType> AddonList;

	UPROPERTY(EditAnywhere)
	int32 MapID = 0;

	UPROPERTY(EditAnywhere)
	int32 HubID = 0;
};

USTRUCT()
struct FInteractableActorInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FSupportInteractableActorInfo IAInfo;

	UPROPERTY(EditAnywhere)
	FVector IActorPossition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	TMap<int32, EResourceType> LockedResources;

	UPROPERTY(EditAnywhere)
	int32 IDWorkerHubLockActor = 0;
};

USTRUCT()
struct FInteractableActorChangedInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FInteractableActorInfo IAInfo;
	
	UPROPERTY(EditAnywhere)
	FSupportInteractableActorChangedInfo IAChangedInfo;
};

USTRUCT()
struct FPartOfKnownObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FSupportKnownObjectInfo PartOfObject;

	UPROPERTY(EditAnywhere)
	int32 PartOfObjectSize = 0;
};


USTRUCT()
struct FPawnInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FSupportPawnInfo PawnInfo;

	UPROPERTY(EditAnywhere)
	TMap<EAddonPossitionType, EWorkerAddonType> ConnectedAddons;

	UPROPERTY(EditAnywhere)
	TArray<FSupportKnownObjectInfo> KnownObjectInfo;

	UPROPERTY(EditAnywhere)
	TArray<FPartOfKnownObject> PartOfKnownObjectInfo;

	UPROPERTY(EditAnywhere)
	int32 WorkerMapID = -1;

	UPROPERTY(EditAnywhere)
	int32 PartOfWorkerMapID = -1;

	UPROPERTY(EditAnywhere)
	int32 PartOfWorkerMapSize = 0;

	UPROPERTY(EditAnywhere)
	int32 IDMasterHub = -1;

};

USTRUCT()
struct FResourceInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FVector Location;

	UPROPERTY(EditAnywhere)
	EResourceType Type;
};

USTRUCT()
struct FHubInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FSupportHubInfo Info;

	UPROPERTY(EditAnywhere)
	TArray<EResourceType> PossessedResources;

	UPROPERTY(EditAnywhere)
	TMap<EWorkerType, int32> ConnectedPawns;

	UPROPERTY(EditAnywhere)
	FVector Location;

	UPROPERTY(EditAnywhere)
	FSupportAIInfo AIInfo;

};

USTRUCT()
struct FBuildingSaveInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FSupportBuildingInfo Info;

	UPROPERTY(EditAnywhere)
	FVector Location;
};

USTRUCT()
struct FMakingActionInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 HubID = -1;

	UPROPERTY(EditAnywhere)
	EActionType Type = EActionType::Invalid;

	UPROPERTY(EditAnywhere)
	EActionType SubActionType = EActionType::Invalid;

	UPROPERTY(EditAnywhere)
	float RemainingTime = 0.f;

	UPROPERTY(EditAnywhere)
	FVector IActorLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector BuildingLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector ResourceLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	ERecipeType RecipeType = ERecipeType::Invalid;
};

UCLASS()
class PHILODENDRON_API UMainSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 SeedForGenerateTileMap = 0;

	UPROPERTY(EditAnywhere)
	FVector SunPossition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FCoreInfo CoreInfo;
	
	UPROPERTY(EditAnywhere)
	TArray<FInteractableActorInfo> InteractableActorList;

	UPROPERTY(EditAnywhere)
	TArray<FInteractableActorChangedInfo> InteractableActorChangedList;

	UPROPERTY(EditAnywhere)
	TArray<FPawnInfo> PawnList;

	UPROPERTY(EditAnywhere)
	TArray<EWorkerAddonType> AddonList;

	UPROPERTY(EditAnywhere)
	TArray<FResourceInfo> ResourceList;

	UPROPERTY(EditAnywhere)
	TArray<FHubInfo> HubList;

	UPROPERTY(EditAnywhere)
	TArray<FBuildingSaveInfo> BuildingList;

	UPROPERTY(EditAnywhere)
	TArray<FMakingActionInfo> MakingActionList;
};
