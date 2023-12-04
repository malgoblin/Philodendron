#pragma once

#include "CoreMinimal.h"
#include "WorkerTrait.h"
#include "ActionTree.h"
#include "SupportStructsForSaveLoad.h"
#include "WorkerAchivementStruct.h"

class AWorkerHub;
class AInteractableActor;

class UActionTree;
class IActionTreeListener;

enum class ERecipeType;
enum class EActionType;

struct ActionOnInteractableActor
{
	ActionOnInteractableActor() {}
	ActionOnInteractableActor(EActionType ActionType, TWeakObjectPtr<AInteractableActor> Actor = nullptr)
		: Action(ActionType)
		, InteractableActor(Actor)
	{
	}

	EActionType Action = EActionType::Invalid;
	TWeakObjectPtr<AInteractableActor> InteractableActor = nullptr;
	ERecipeType Recipe = ERecipeType::Invalid;
};

class PHILODENDRON_API UAIActionSelector
{
public:

	ActionOnInteractableActor GenerateNewAction(const TArray<ActionOnInteractableActor>& AvailableActions);

	void ActionCompleted(EActionType AType, EInteractableActorType ActorType);
	void MakeRecipeCompleted(ERecipeType Recipe);
	void CheckNewAvailableActions();

	void SetActionTree(TObjectPtr<UActionTree> ATree, IActionTreeListener* Listener);

	void AddNewKnownActorType(EInteractableActorType NewType);
	void AddNewKnownResourceType(EResourceType NewType);
	const TArray<EInteractableActorType>& GetKnownActorType() const;
	const TArray<EResourceType>& GetKnownResourceType() const;


	void RemoveActionPriority(EActionType RemovedAction);

	const FElementCost& GetWorkerAchievements() const;

	void AddNewWorkerAchivement(const FElementCost& NewElementCost);

	void ChangePriorityAfterDoAction(EActionType Type);

	void AddNewTreeElementToMemory(EActionTreeElementName Name, const FElementReward* Reward);

	TArray<EActionTreeElementName> GetAllUnknownTreeElementsListToTreeElement(EActionTreeElementName Name) const;
	TArray<EActionTreeElementName> GetAllDeletableTreeElementListToTreeElement(EActionTreeElementName Name) const;
	void RemoveTreeElementsFromList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const;
	void RemoveTreeElementsFromRemovableList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const;



	void ClearWorkerAchivements();

	void RemoveTreeElement(EActionTreeElementName Name);

	int32 GetSizeTreeToTreeElementWithoutTreeElementList(EActionTreeElementName Name, const TArray<EActionTreeElementName>& SuperfloursList) const;

	const FSupportAIInfo& GetAIInfo() const;
	void LoadAIInfo(FSupportAIInfo&& Info);

private:

	void UpdateAIByTree();

	void ClearAllRewards();
	void AddNewReward(const FElementReward* Reward);

	void UpdateActionAndRecipeMaxPriority();

private:
	UPROPERTY()
	FSupportAIInfo AIInfo;

	UPROPERTY()
	TObjectPtr<UActionTree> ActionTree = nullptr;

	IActionTreeListener* ATListener = nullptr;
};
