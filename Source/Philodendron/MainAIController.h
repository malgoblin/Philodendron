#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIActionSelector.h"
#include "ActionTreeListener.h"
#include "EnumActionTreeElementName.h"
#include "EnumBuildingType.h"
#include "MainAIController.generated.h"

enum class EActionType;
enum class ERecipeType;
enum class EResourceType;
enum class EInteractableActorType;

class AInteractableActor;
class UActionTree;
class AMainPlayer;
class ABuilding;
class AResource;

UCLASS()
class PHILODENDRON_API AMainAIController : public AAIController, public IActionTreeListener
{
	GENERATED_BODY()

public:
	virtual bool AddNewTreeElementListToMemory(const TArray<EActionTreeElementName>& TreeElementList) override;

	bool GenerateNewAction();

	void CreateActionTree();

	void AddNewLogLine();

	void ActionCompleted(EActionType AType, EInteractableActorType ActorType);
	void MakeRecipeCompleted(ERecipeType Recipe);


	bool NeedInteractableActorForAction(EActionType Type) const;
	const TArray<EResourceType>* GetResourcesMinedAction(EActionType Type) const;

	int32 GetSizeTreeToTreeElementWithoutTreeElementList(EActionTreeElementName Name, const TArray<EActionTreeElementName>& SuperfloursList) const;

	void AddNewTreeElementToMemory(EActionTreeElementName Name, const FElementReward* Reward);

	TArray<EActionTreeElementName> GetAllUnknownTreeElementsListToTreeElement(EActionTreeElementName Name) const;

	TArray<EActionTreeElementName> GetAllDeletableTreeElementListToTreeElement(EActionTreeElementName Name) const;
	void RemoveTreeElementsFromList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const;
	void RemoveTreeElementsFromRemovableList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const;


	void ClearWorkerAchivements();
	void RemoveTreeElement(EActionTreeElementName Name);

	TArray<FVector> GetPathFromPointToPoint(const FVector& Source, const FVector& Destination) const;

	FVector GetRandomPointInRadiusAroundPoint(const FVector& Destination, int32 Radius) const;

	TWeakObjectPtr<AInteractableActor> FindInteractableActorForCreateRecipe(EInteractableActorType Type) const;
	TWeakObjectPtr<ABuilding> FindBuildingForCreateRecipe(EBuildingType Type) const;

	const FSupportAIInfo& GetAIInfo() const;
	void LoadAIInfo(FSupportAIInfo&& Info);
	void ChangePriorityAfterDoAction(EActionType Type);

private:
	UAIActionSelector ActionSelector;

	UPROPERTY()
	TObjectPtr<UActionTree> ActionTree = nullptr;
};
