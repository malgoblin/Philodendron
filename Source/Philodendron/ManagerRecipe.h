

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumRecipeType.h"
#include "EnumInteractableActorType.h"
#include "EnumBuildingType.h"
#include "EnumResourceType.h"
#include "EnumPossibleLanguages.h"
#include "ManagerRecipe.generated.h"



USTRUCT()
struct FRecipeParametrs
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<EPossibleLanguages, FString> RecipeName;

	UPROPERTY(EditAnywhere)
	TMap<EResourceType, int32> NeededResources;

	UPROPERTY(EditAnywhere)
	int32 MemorySize;

	UPROPERTY(EditAnywhere)
	float TimeToMake;

	UPROPERTY(EditAnywhere)
	EGlobalRecipeType GlobalRecipeType;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "GlobalRecipeType == EGlobalRecipeType::InteractableActor", EditConditionHides))
	EInteractableActorType InteractableActor;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "GlobalRecipeType == EGlobalRecipeType::Building", EditConditionHides))
	EBuildingType BuildingType;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "GlobalRecipeType == EGlobalRecipeType::Resource", EditConditionHides))
	EResourceType ResourceType;

	UPROPERTY(EditAnywhere)
	EPlaceWhereMakingRecipe PlaceWhereMakingRecipe;

	UPROPERTY(EditAnywhere, meta=(EditCondition="PlaceWhereMakingRecipe == EPlaceWhereMakingRecipe::Empty", EditConditionHides))
	float EmptyAreaAroundObj = 0.f;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "PlaceWhereMakingRecipe == EPlaceWhereMakingRecipe::Building", EditConditionHides))
	EBuildingType NeededBuilding;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "PlaceWhereMakingRecipe == EPlaceWhereMakingRecipe::Building", EditConditionHides))
	ENeedWorkerToMakeRecipe NeedWorkerToMake;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "PlaceWhereMakingRecipe == EPlaceWhereMakingRecipe::InteractableActor", EditConditionHides))
	EInteractableActorType NeededInteractableActorWhereCreateRecipe;
};

UCLASS()
class PHILODENDRON_API AManagerRecipe : public AActor
{
	GENERATED_BODY()
	
public:	
	AManagerRecipe();
	virtual void Tick(float DeltaTime) override;

	int32 GetRecipeSize(ERecipeType Type) const;
	float GetTimeToMakeRecipe(ERecipeType Type) const;

	EPlaceWhereMakingRecipe GetPlaceWhereMakingRecipe(ERecipeType Type) const;
	float GetEmptyAreaAroundEmptyPlace(ERecipeType Type) const;
	EBuildingType GetNeededBuildingForCretingRecipe(ERecipeType Type) const;
	EInteractableActorType GetNeededInteractableActorWhereCreateRecipe(ERecipeType Type) const;
	ENeedWorkerToMakeRecipe GetNeededWorkerToMakeRecipe(ERecipeType Type) const;

	EGlobalRecipeType GetGlobalRecipeType(ERecipeType Type) const;
	EInteractableActorType GetInteractableActorForCreating(ERecipeType Type) const;
	EBuildingType GetBuildingWhichCreating(ERecipeType Type) const;
	EResourceType GetResourceWhichCreating(ERecipeType Type) const;
	const FString* GetRecipeName(ERecipeType Type, EPossibleLanguages Languages) const;
	

	const TMap<EResourceType, int32>* GetNeededResourcesForRecipe(ERecipeType Type) const;

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TMap<ERecipeType, FRecipeParametrs> RecipeParametrs;
};
