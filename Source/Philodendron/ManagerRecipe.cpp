// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerRecipe.h"
#include "MainGameStateBase.h"

AManagerRecipe::AManagerRecipe()
{
	PrimaryActorTick.bCanEverTick = false;
}


void AManagerRecipe::BeginPlay()
{
	Super::BeginPlay();
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SetManagerRecipe(this);
	
}

void AManagerRecipe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AManagerRecipe::GetRecipeSize(ERecipeType Type) const
{
	if (!RecipeParametrs.Contains(Type))
	{
		return -1;
	}
	return (*RecipeParametrs.Find(Type)).MemorySize;
}

const TMap<EResourceType, int32>* AManagerRecipe::GetNeededResourcesForRecipe(ERecipeType Type) const
{
	if (!RecipeParametrs.Contains(Type))
		return nullptr;

	return &RecipeParametrs[Type].NeededResources;
}

float AManagerRecipe::GetTimeToMakeRecipe(ERecipeType Type) const
{
	if (!RecipeParametrs.Contains(Type))
		return -1.f;

	return RecipeParametrs[Type].TimeToMake;
}

EPlaceWhereMakingRecipe AManagerRecipe::GetPlaceWhereMakingRecipe(ERecipeType Type) const
{
	if (!RecipeParametrs.Contains(Type))
		return EPlaceWhereMakingRecipe::Invalid;

	return RecipeParametrs[Type].PlaceWhereMakingRecipe;
}

float AManagerRecipe::GetEmptyAreaAroundEmptyPlace(ERecipeType Type) const
{
	if (!RecipeParametrs.Contains(Type))
		return -1.f;

	return RecipeParametrs[Type].EmptyAreaAroundObj;
}

EGlobalRecipeType AManagerRecipe::GetGlobalRecipeType(ERecipeType Type) const
{
	if (!RecipeParametrs.Contains(Type))
		return EGlobalRecipeType::Invalid;

	return RecipeParametrs[Type].GlobalRecipeType;
}

EInteractableActorType AManagerRecipe::GetInteractableActorForCreating(ERecipeType Type) const
{
	if (!RecipeParametrs.Contains(Type))
		return EInteractableActorType::Invalid;

	return RecipeParametrs[Type].InteractableActor;
}

EBuildingType AManagerRecipe::GetNeededBuildingForCretingRecipe(ERecipeType Type) const
{
	if (!RecipeParametrs.Contains(Type))
		return EBuildingType::Invalid;

	return RecipeParametrs[Type].NeededBuilding;
}

EInteractableActorType AManagerRecipe::GetNeededInteractableActorWhereCreateRecipe(ERecipeType Type) const
{
	if (!RecipeParametrs.Contains(Type))
		return EInteractableActorType::Invalid;

	return RecipeParametrs[Type].NeededInteractableActorWhereCreateRecipe;
}

EBuildingType AManagerRecipe::GetBuildingWhichCreating(ERecipeType Type) const
{
	if (!RecipeParametrs.Contains(Type))
		return EBuildingType::Invalid;

	return RecipeParametrs[Type].BuildingType;
}

EResourceType AManagerRecipe::GetResourceWhichCreating(ERecipeType Type) const
{
	if (!RecipeParametrs.Contains(Type))
		return EResourceType::Invalid;

	return RecipeParametrs[Type].ResourceType;
}

ENeedWorkerToMakeRecipe AManagerRecipe::GetNeededWorkerToMakeRecipe(ERecipeType Type) const
{
	if (!RecipeParametrs.Contains(Type))
		return ENeedWorkerToMakeRecipe::Invalid;

	return RecipeParametrs[Type].NeedWorkerToMake;
}

const FString* AManagerRecipe::GetRecipeName(ERecipeType Type, EPossibleLanguages Languages) const
{
	if (!RecipeParametrs.Contains(Type))
	{
		return nullptr;
	}

	return RecipeParametrs[Type].RecipeName.Find(Languages);
}