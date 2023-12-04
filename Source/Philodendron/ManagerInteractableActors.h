// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumInteractableActorType.h"
#include "EnumPossibleLanguages.h"
#include "ManagerInteractableActors.generated.h"

enum class EActionType;

enum class EResourceType;
enum class ETypeOfActionAndActor;

struct FPlantAnimationStruct;

class AInteractableActor;


USTRUCT()
struct FIActorParametrs
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<EPossibleLanguages, FString> IActorName;

	UPROPERTY(EditAnywhere)
	TArray<FPlantAnimationStruct> InteractableActorFlipbooks;

	UPROPERTY(EditAnywhere)
	TArray<EActionType> AvailableActions;

	UPROPERTY(EditAnywhere)
	int32 SizeInfoAboutActor;

	UPROPERTY(EditAnywhere)
	float DistanceForCreateResources;

	UPROPERTY(EditAnywhere)
	bool IsActorRemoveAfterEjectAllResources;

	UPROPERTY(EditAnywhere)
	ETypeOfActionAndActor GlobalType;

	UPROPERTY(EditAnywhere)
	EInteractableActorCreatingType CreatingType;

	UPROPERTY(EditAnywhere)
	bool IsItHorizontalObject = false;
};


UCLASS()
class PHILODENDRON_API AManagerInteractableActors : public AActor
{
	GENERATED_BODY()
	
public:	
	AManagerInteractableActors();
	virtual void Tick(float DeltaTime) override;

	const TArray<EActionType>* GetAvailableActions(EInteractableActorType Type) const;
	int32 GetSizeInfoAboutIActor(EInteractableActorType Type) const;
	const TArray<EActionType>& GetKnownActionInCore() const;
	float GetDistanceForCreateResources(EInteractableActorType Type) const;
	const FPlantAnimationStruct* GetAnimationForIActor(EInteractableActorType Type, int32 ObjectNumber) const;
	FVector3d GetBoxSizeFinalFlipbook(EInteractableActorType Type, int32 ObjectNumber) const;

	int32 GenerateRandomObject(EInteractableActorType Type) const;

	TObjectPtr<AInteractableActor> CreateNewInteractableActor(EInteractableActorType Type, FVector Location, int32 ObjectNumber, EInteractableActorCreatingType CreatingType) const;

	const FString* GetInteractableActorName(EInteractableActorType Type, EPossibleLanguages Language) const;

	TObjectPtr<AInteractableActor> GenerateIActorObject(EInteractableActorType Type, FVector Location, EInteractableActorCreatingType CreatingType) const;

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<AInteractableActor> CreateInteractableActorWithoutGrowth(EInteractableActorType Type, FVector Location, int32 ObjectNumber) const;
	TObjectPtr<AInteractableActor> CreateInteractableActorWithGrowth(EInteractableActorType Type, FVector Location, int32 ObjectNumber, EInteractableActorCreatingType CreatingType) const;

	TObjectPtr<AInteractableActor> GenerateIActorWithGrowthObject(EInteractableActorType Type, FVector Location) const;
	TObjectPtr<AInteractableActor> GenerateIActorWithoutGrowthObject(EInteractableActorType Type, FVector Location) const;

public:
	UPROPERTY(EditAnywhere)
	TMap<EInteractableActorType, FIActorParametrs> InteractableActorParametrs;

	UPROPERTY(EditAnywhere)
	TArray<EActionType> DefaultsKnownActionInCore;

};
