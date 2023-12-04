// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorkerAchivementStruct.generated.h"


enum class EInteractableActorType;
enum class EActionType;
enum class ERecipeType;
enum class EResourceType;

USTRUCT()
struct FElementCost
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<EInteractableActorType, int32> TypeCost;
	
	UPROPERTY(EditAnywhere)
	TMap<EActionType, int32> ActionCost;

	UPROPERTY(EditAnywhere)
	TMap<ERecipeType, int32> RecipeCost;
};

USTRUCT()
struct FElementReward
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<EInteractableActorType> KnownActorType;
	
	UPROPERTY(EditAnywhere)
	TArray<EResourceType> KnownResourceType;

	UPROPERTY(EditAnywhere)
	TArray<EActionType> ActionList;

	UPROPERTY(EditAnywhere)
	TArray<ERecipeType> RecipeList;

	UPROPERTY(EditAnywhere)
	TMap<EActionType, float> DeltaPriority;

	UPROPERTY(EditAnywhere)
	TMap<ERecipeType, float> DeltaPriorityRecipe;

	UPROPERTY(EditAnywhere)
	TMap<EActionType, float> DeltaPriorityActionAfterDo;

	UPROPERTY(EditAnywhere)
	TMap<EActionType, float> DeltaPriorityActionAfterDoAnother;
};