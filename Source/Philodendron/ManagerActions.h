// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManagerActions.generated.h"

enum class EActionType;
enum class EWorkerAddonType;
enum class ETypeOfActionAndActor;
enum class EResourceType;
enum class EPossibleLanguages;

USTRUCT()
struct FActionParametrs
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<EPossibleLanguages, FString> ActionName;

	UPROPERTY(EditAnywhere)
	TMap<EWorkerAddonType, int32> ConditionAddons;

	UPROPERTY(EditAnywhere)
	int32 MemorySize;

	UPROPERTY(EditAnywhere)
	float TimeToMake;

	UPROPERTY(EditAnywhere)
	ETypeOfActionAndActor GlobalType;

	UPROPERTY(EditAnywhere)
	bool bNeededInteractableActor;

	UPROPERTY(EditAnywhere)
	TArray<EResourceType> ResourcesMinedThisAction;

	UPROPERTY(EditAnywhere)
	int32 DistanceToMakeAction;

};

UCLASS()
class PHILODENDRON_API AManagerActions : public AActor
{
	GENERATED_BODY()
	
public:	
	AManagerActions();
	virtual void Tick(float DeltaTime) override;

	const TMap<EWorkerAddonType, int32>* GetConditionAddons(EActionType Type) const;
	int32 GetActionSize(EActionType Type) const;
	float GetActionTime(EActionType Type) const;
	bool NeedInteractableActorForAction(EActionType Type) const;
	const TArray<EResourceType>* GetResourcesMinedAction(EActionType Type) const;
	int32 GetDistanceToMakeAction(EActionType Type) const;
	const FString* GetActionName(EActionType Type, EPossibleLanguages Languages) const;

protected:
	virtual void BeginPlay() override;


public:
	UPROPERTY(EditAnywhere)
	TMap<EActionType, FActionParametrs> ActionParametrs;

};
