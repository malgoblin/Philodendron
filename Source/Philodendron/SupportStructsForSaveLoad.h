#pragma once

#include "CoreMinimal.h"
#include "EnumInteractableActorType.h"
#include "EnumActionList.h"
#include "EnumAnimationType.h"
#include "EnumTypeOfActionAndActor.h"
#include "EnumActionTreeElementName.h"
#include "EnumWorkerType.h"
#include "EnumWorkerHubStatus.h"
#include "EnumWorkerAddonType.h"
#include "EnumRecipeType.h"
#include "EnumBuildingType.h"
#include "WorkerTrait.h"
#include "WorkerAchivementStruct.h"
#include "SupportStructsForSaveLoad.generated.h"

USTRUCT()
struct FSupportKnownObjectInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FVector KnownObjectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	int32 DataSize = 0;

	UPROPERTY(EditAnywhere)
	EInteractableActorType ActorType = EInteractableActorType::Invalid;
	
	UPROPERTY(EditAnywhere)
	TArray<EActionType> ActionList;
};

USTRUCT()
struct FSupportInteractableActorInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EInteractableActorType ActorType = EInteractableActorType::Invalid;

	UPROPERTY(EditAnywhere)
	ETypeOfActionAndActor GlobalActorType;

	UPROPERTY(EditAnywhere)
	TMap<EResourceType, int32> Resources;

	UPROPERTY(EditAnywhere)
	EAnimationType CurrentState = EAnimationType::Final;

	UPROPERTY(EditAnywhere)
	TArray<EActionType> ActionList;
	
	UPROPERTY(EditAnywhere)
	bool bIsRemoveAfterEjectAllResources = false;

	UPROPERTY(EditAnywhere)
	int32 FlipbookNumber = 0;

	UPROPERTY(EditAnywhere)
	bool bIsObjectBusy = false;
};

USTRUCT()
struct FSupportInteractableActorChangedInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 CurrentFlipbookFromList = 0;

	UPROPERTY(EditAnywhere)
	float TimerForChange = 0.f;

	UPROPERTY(EditAnywhere)
	float TimeForState = 0.f;

	UPROPERTY(EditAnywhere)
	float TimeForTransitionProcess = 0.f;

	UPROPERTY(EditAnywhere)
	bool bIsTransitionProcess = false;

	UPROPERTY(EditAnywhere)
	bool bIsGrowthCompleted = true;
};

USTRUCT()
struct FSupportPawnInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString WorkerPawnName = "WorkerPawn";

	UPROPERTY(EditAnywhere)
	EWorkerType WorkerType = EWorkerType::Left;

	UPROPERTY(EditAnywhere)
	bool bIsReadOnly = false;

	UPROPERTY(EditAnywhere)
	bool bMemoryBroken = false;

	UPROPERTY(EditAnywhere)
	int32 PawnMemorySize = 0;

	UPROPERTY(EditAnywhere)
	int32 PawnMaxMemorySize = 0;
	
	UPROPERTY(EditAnywhere)
	int32 PawnID = -1;

	UPROPERTY(EditAnywhere)
	TMap<EActionTreeElementName, int32> FullTreeElementContainsInPawn;

	UPROPERTY(EditAnywhere)
	TMap<EActionTreeElementName, int32> PartOfTreeElement;
};
/*
USTRUCT()
struct FSupportHubBrainInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FElementCost WorkerAchievements;
	
	UPROPERTY(EditAnywhere)
	TArray<EInteractableActorType> KnownActorTypes;
	
	UPROPERTY(EditAnywhere)
	TArray<EResourceType> KnownResourceTypes;

	UPROPERTY(EditAnywhere)
	TMap<EActionType, float> ActionPriorities;

	UPROPERTY(EditAnywhere)
	TMap<ERecipeType, float> RecipePriorities;

	UPROPERTY(EditAnywhere)
	float ActionMaxPriority = 0.f;

	UPROPERTY(EditAnywhere)
	float RecipeMaxPriority = 0.f;
};
*/
USTRUCT()
struct FSupportHubInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int CPoint = 0;
	
	UPROPERTY(EditAnywhere)
	TArray<FVector> PathToDestination;
	
	UPROPERTY(EditAnywhere)
	FVector NextPoint = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector Velocity = FVector::ZeroVector;
	
	UPROPERTY(EditAnywhere)
	int32 DistanceToPoint = 0;


	UPROPERTY(EditAnywhere)
	TArray<EActionType> ActionList;

	UPROPERTY(EditAnywhere)
	TArray<ERecipeType> RecipeList;

	UPROPERTY(EditAnywhere)
	TMap<EWorkerAddonType, int32> AddonList;

	UPROPERTY(EditAnywhere)
	FString HubName;

	UPROPERTY(EditAnywhere)
	int32 CountConnectedWorkers = 0;

	UPROPERTY(EditAnywhere)
	int32 HubMemorySize = 0;

	UPROPERTY(EditAnywhere)
	int32 HubMaxMemorySize = 0;

	UPROPERTY(EditAnywhere)
	bool bTurnOnWorker = false;

	UPROPERTY(EditAnywhere)
	EWorkerHubStatus HubStatus = EWorkerHubStatus::Invalid;

	UPROPERTY(EditAnywhere)
	float CurrentWorkerRestCount = 0.f;

	UPROPERTY(EditAnywhere)
	float WorkerRestCount = 0.f;

	UPROPERTY(EditAnywhere)
	float DefaultWorkerSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	float DefaultWorkerSpottingRadius = 0.f;
	
	UPROPERTY(EditAnywhere)
	float WeightLimit = 0.f;
	
	UPROPERTY(EditAnywhere)
	int32 HubID = 0;
};

USTRUCT()
struct FSupportBuildingInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EBuildingType BuildingType = EBuildingType::Invalid;

	UPROPERTY(EditAnywhere)
	bool bBuildingBusy = false;

	UPROPERTY(EditAnywhere)
	float TimeToMakeWork = 0.f;

	UPROPERTY(EditAnywhere)
	int32 DistanceForCreatResources;

	UPROPERTY(EditAnywhere)
	ERecipeType RecipeForCreating = ERecipeType::Invalid;

};

USTRUCT()
struct FSupportAIInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FWorkerTrait WTrait;

	UPROPERTY(EditAnywhere)
	FElementCost WorkerAchievements;

	UPROPERTY(EditAnywhere)
	TArray<EInteractableActorType> KnownActorTypes;
	
	UPROPERTY(EditAnywhere)
	TArray<EResourceType> KnownResourceTypes;

	UPROPERTY(EditAnywhere)
	TMap<EActionType, float> ActionPriorities;

	UPROPERTY(EditAnywhere)
	TMap<ERecipeType, float> RecipePriorities;

	UPROPERTY(EditAnywhere)
	float ActionMaxPriority = 0.f;

	UPROPERTY(EditAnywhere)
	float RecipeMaxPriority = 0.f;
};