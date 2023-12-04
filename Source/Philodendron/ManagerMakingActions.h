// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumActionList.h"
#include "EnumResourceType.h"
#include "EnumRecipeType.h"
#include "ManagerMakingActions.generated.h"

class AWorkerHub;
class AInteractableActor;
class AResource;
class ABuilding;

USTRUCT()
struct FInfoAboutAction
{
	GENERATED_BODY()

public:
	EActionType Type = EActionType::Invalid;
	EActionType SubActionType = EActionType::Invalid;
	float RemainingTime = 0.f;
	TWeakObjectPtr<AInteractableActor> CurrentInteractableActor = nullptr;
	TWeakObjectPtr<ABuilding> CurrentBuilding = nullptr;
	TWeakObjectPtr<AResource> CurrentResource = nullptr;
	ERecipeType RecipeType = ERecipeType::Invalid;
};

UCLASS()
class PHILODENDRON_API AManagerMakingActions : public AActor
{
	GENERATED_BODY()
	
public:	
	AManagerMakingActions();
	virtual void Tick(float DeltaTime) override;
	
	bool ContinueToPrepair(TObjectPtr<AWorkerHub>& WHub);
	bool PrepairToNewAction(TObjectPtr<AWorkerHub>& WHub, EActionType Type, TWeakObjectPtr<AInteractableActor> IActor, ERecipeType RecipeType);

	const TMap<TObjectPtr<AWorkerHub>, FInfoAboutAction>& GetCurrentPrepairAction() const;

	void LoadMakingActionList(TMap<TObjectPtr<AWorkerHub>, FInfoAboutAction>&& List);

protected:
	virtual void BeginPlay() override;

private:

	void TickAllActions(float DeltaTime);

	//PREPAIR TO ACTION
	bool PrepairToAction(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool PrepairToBackToCore(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool MovableToPointOnDistabce(TObjectPtr<AWorkerHub>& WHub, FVector Point, int32 Dist) const;
	bool MoveToInteractableActor(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool MoveToResource(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool MoveToEmptyPlace(TObjectPtr<AWorkerHub>& WHub, float EmptyArea);
	bool MoveToBuilding(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool PrepairToMakeRecipe(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool PrepairToTakeResource(TObjectPtr<AWorkerHub>& WHub, const TArray<EResourceType>& TypeList);


	//START ACTION
	bool StartMinerResource(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction) const;
	bool StartResearch(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction) const;
	bool StartHarvestAllResources(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction) const;
	bool StartTakeResource(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction) const;
	bool StartMakeRecipe(TObjectPtr<AWorkerHub>& WHub) const;
	bool StartMakeRecipeOnInteractableActor(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction) const;
	bool StartMakeRecipeOnBuilding(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);

	//FINISH ACTION
	void FinishAction(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool FinishResearch(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool FinishMinerResource(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction) const;
	bool FinishBackToCore(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool FinishHarvestAllResources(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool FinishPullOutBush(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool FinishTakeResource(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool FinishMakeRecipe(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool MakeRecipeForEmptyPlace(TObjectPtr<AWorkerHub>& WHub, ERecipeType Type);
	bool MakeRecipeOnInteractableActor(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);
	bool MakeRecipeInBuilding(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction);

	//SUPPORT
	void ClearObj(TObjectPtr<AWorkerHub>& WHub);
	int32 CountResourcesInList(const TArray<EResourceType> ResourceList, EResourceType Type) const;

private:
	UPROPERTY()
	TMap<TObjectPtr<AWorkerHub>, FInfoAboutAction> CurrentPrepairAction;
};
