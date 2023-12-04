#include "AIActionSelector.h"
#include "InteractableActor.h"
#include "WorkerHub.h"
#include "EnumActionList.h"
#include "EnumRecipeType.h"
#include "ActionTreeListener.h"
#include "WorkerAchivementStruct.h"

void UAIActionSelector::SetActionTree(TObjectPtr<UActionTree> ATree, IActionTreeListener* Listener)
{
	ActionTree = ATree;
	ATListener = Listener;
}

ActionOnInteractableActor UAIActionSelector::GenerateNewAction(const TArray<ActionOnInteractableActor>& AvailableActions)
{
	ActionOnInteractableActor Result;

	if (AvailableActions.Num() == 0)
	{
		return Result;
	}

	float CurrentMaxPriority = -1.f;

	for (const auto& Action : AvailableActions)
	{
		if (AIInfo.ActionPriorities.Contains(Action.Action) && AIInfo.ActionPriorities[Action.Action] > CurrentMaxPriority)
		{
			if (Action.Action != EActionType::MakeRecipe || AIInfo.RecipePriorities.Num() != 0)
			{
				CurrentMaxPriority = AIInfo.ActionPriorities[Action.Action];
			}
		}
	}

	AIInfo.ActionMaxPriority = CurrentMaxPriority;

	TArray<ActionOnInteractableActor> ActionListWithMaxPriority;
	for (const auto& Action : AvailableActions)
	{
		float* ActionPriority = AIInfo.ActionPriorities.Find(Action.Action);
		if (ActionPriority && *ActionPriority == AIInfo.ActionMaxPriority)
		{
			if (Action.Action != EActionType::MakeRecipe || AIInfo.RecipePriorities.Num() != 0)
			{
				ActionListWithMaxPriority.Add(Action);
			}
		}
	}

	if (ActionListWithMaxPriority.Num() == 0)
	{
		UpdateActionAndRecipeMaxPriority();
		return Result;
	}

	int32 ActionNumber = FMath::RandRange(0, ActionListWithMaxPriority.Num() - 1) % ActionListWithMaxPriority.Num();

	if (ActionListWithMaxPriority[ActionNumber].Action != EActionType::MakeRecipe)
	{
		Result.Action = ActionListWithMaxPriority[ActionNumber].Action;
		Result.InteractableActor = ActionListWithMaxPriority[ActionNumber].InteractableActor;
		return Result;
	}

	TArray<ERecipeType> RecipeListWithMaxPriority;
	for (const auto& Recipe : AIInfo.RecipePriorities)
	{
		//if (Recipe.Value == AIInfo.RecipeMaxPriority)
		//{
			RecipeListWithMaxPriority.Add(Recipe.Key);
		//}
	}

	Result.Action = EActionType::MakeRecipe;
	Result.InteractableActor = nullptr;
	Result.Recipe = RecipeListWithMaxPriority[FMath::RandRange(0, RecipeListWithMaxPriority.Num() - 1)];

	return Result;
}

void UAIActionSelector::ActionCompleted(EActionType AType, EInteractableActorType ActorType)
{
	if (AType == EActionType::Research)
	{
		if (ActorType == EInteractableActorType::Invalid)
			return;

		//WorkerAchievements.IncrementTypeCost(IActorType);

		int32* IActorCount = AIInfo.WorkerAchievements.TypeCost.Find(ActorType);
		if (!IActorCount)
		{
			AIInfo.WorkerAchievements.TypeCost.Add(ActorType, 1);
		}
		else
		{
			*IActorCount += 1;
		}
	}

	int32* ActionCount = AIInfo.WorkerAchievements.ActionCost.Find(AType);
	if (!ActionCount)
	{
		AIInfo.WorkerAchievements.ActionCost.Add(AType, 1);
	}
	else
	{
		*ActionCount += 1;
	}

	CheckNewAvailableActions();
	ChangePriorityAfterDoAction(AType);
	UpdateActionAndRecipeMaxPriority();
}

// rename -> OnMakeRecipeCompletion or CompleteMakingRecipe
void UAIActionSelector::MakeRecipeCompleted(ERecipeType Recipe)
{


	int32* RecipeCount = AIInfo.WorkerAchievements.RecipeCost.Find(Recipe);
	if (!RecipeCount)
	{
		AIInfo.WorkerAchievements.RecipeCost.Add(Recipe, 1);
		return;
	}
	*RecipeCount += 1;
}
#pragma optimize("", off)
void UAIActionSelector::CheckNewAvailableActions()
{
	TArray<EActionTreeElementName> NewTreeElementList = ActionTree->GetNewAvailableTreeElement(*ActionTree->GetTreeRoot(), AIInfo.WorkerAchievements);

	while (NewTreeElementList.Num() > 0)
	{
		if (!ATListener->AddNewTreeElementListToMemory(NewTreeElementList))
			return;

		NewTreeElementList = ActionTree->GetNewAvailableTreeElement(*ActionTree->GetTreeRoot(), AIInfo.WorkerAchievements);
	}
}
#pragma optimize("", on)
void UAIActionSelector::AddNewTreeElementToMemory(EActionTreeElementName Name, const FElementReward* Reward)
{
	ActionTree->SetAlreadyKnownTreeElement(Name);
	
	AddNewReward(Reward);
}

void UAIActionSelector::AddNewKnownActorType(EInteractableActorType NewType)
{
	if (AIInfo.KnownActorTypes.Contains(NewType))
		return;

	AIInfo.KnownActorTypes.Add(NewType);
}

void UAIActionSelector::AddNewKnownResourceType(EResourceType NewType)
{
	if (AIInfo.KnownResourceTypes.Contains(NewType))
		return;

	AIInfo.KnownResourceTypes.Add(NewType);
}

const TArray<EInteractableActorType>& UAIActionSelector::GetKnownActorType() const
{
	return AIInfo.KnownActorTypes;
}

const TArray<EResourceType>& UAIActionSelector::GetKnownResourceType() const
{
	return AIInfo.KnownResourceTypes;
}

void UAIActionSelector::ChangePriorityAfterDoAction(EActionType Type)
{
	for (auto& Action : AIInfo.ActionPriorities)
	{
		Action.Value += Action.Key == Type ? AIInfo.WTrait.GetDeltaPriorityOnActionDone(Type) : AIInfo.WTrait.GetDeltaPriorityOnOtherActionDone(Type);
		
		if (Action.Value < 0)
		{
			Action.Value = 0;
		}
	}
}

void UAIActionSelector::RemoveActionPriority(EActionType RemovedAction)
{
	AIInfo.ActionPriorities.Remove(RemovedAction);
}

const FElementCost& UAIActionSelector::GetWorkerAchievements() const
{
	return AIInfo.WorkerAchievements;
}

void UAIActionSelector::AddNewWorkerAchivement(const FElementCost& NewElementCost)
{
	for (const auto& NewTypeCost : NewElementCost.TypeCost)
	{
		if (int32* CountExistedActorType = AIInfo.WorkerAchievements.TypeCost.Find(NewTypeCost.Key))
		{
			*CountExistedActorType += NewTypeCost.Value;
			continue;
		}
		AIInfo.WorkerAchievements.TypeCost.Add(NewTypeCost);
	}

	for (const auto& NewActionCost : NewElementCost.ActionCost)
	{
		if (int32* CountExistedActionType = AIInfo.WorkerAchievements.ActionCost.Find(NewActionCost.Key))
		{
			*CountExistedActionType += NewActionCost.Value;
			continue;
		}
		AIInfo.WorkerAchievements.ActionCost.Add(NewActionCost);
	}

	for (const auto& NewRecipeCost : NewElementCost.RecipeCost)
	{
		if (int32* CountMakedRecipe = AIInfo.WorkerAchievements.RecipeCost.Find(NewRecipeCost.Key))
		{
			*CountMakedRecipe += NewRecipeCost.Value;
			continue;
		}
		AIInfo.WorkerAchievements.RecipeCost.Add(NewRecipeCost);
	}
}

TArray<EActionTreeElementName> UAIActionSelector::GetAllUnknownTreeElementsListToTreeElement(EActionTreeElementName Name) const
{
	return ActionTree->GetAllUnknownTreeElementsListToTreeElement(Name);
}

TArray<EActionTreeElementName> UAIActionSelector::GetAllDeletableTreeElementListToTreeElement(EActionTreeElementName Name) const
{
	return ActionTree->GetAllDeletableTreeElementListToTreeElement(Name);
}

void UAIActionSelector::ClearWorkerAchivements()
{
	AIInfo.WorkerAchievements.ActionCost.Empty();
	AIInfo.WorkerAchievements.RecipeCost.Empty();
	AIInfo.WorkerAchievements.TypeCost.Empty();
}

void UAIActionSelector::RemoveTreeElement(EActionTreeElementName Name)
{
	ActionTree->RemoveAlreadyKnownTreeElement(Name);
	UpdateAIByTree();
}

void UAIActionSelector::UpdateAIByTree()
{
	TArray<EActionTreeElementName> AllKnownTreeElements = ActionTree->GetAllKnownTreeElementList(ActionTree->GetTreeRoot());

	ClearAllRewards();

	for (const auto& TreeElement : AllKnownTreeElements)
	{
		const FElementReward* Reward = ActionTree->GetRewardForTreeElement(TreeElement);
		AddNewReward(Reward);
	}
}

void UAIActionSelector::AddNewReward(const FElementReward* Reward)
{
	for (const auto& Action : Reward->ActionList)
	{
		if (AIInfo.ActionPriorities.Contains(Action))
			continue;

		AIInfo.ActionPriorities.Add(Action, 0);
	}

	for (const auto& Type : Reward->KnownActorType)
	{
		AddNewKnownActorType(Type);
	}

	for (const auto& Recipe : Reward->RecipeList)
	{
		if (AIInfo.RecipePriorities.Contains(Recipe))
			continue;

		AIInfo.RecipePriorities.Add(Recipe, 0);
	}

	for (const auto& Resource : Reward->KnownResourceType)
	{
		AddNewKnownResourceType(Resource);
	}

	for (const auto& Delta : Reward->DeltaPriority)
	{
		if (float* Priority = AIInfo.ActionPriorities.Find(Delta.Key))
		{
			*Priority += Delta.Value;
			if (*Priority < 0)
			{
				*Priority = 0;
			}
		}
	}

	for (const auto& Delta : Reward->DeltaPriorityRecipe)
	{
		if (float* Priority = AIInfo.RecipePriorities.Find(Delta.Key))
		{
			*Priority += Delta.Value;
			if (*Priority < 0)
			{
				*Priority = 0;
			}
		}
	}

	for (const auto& Delta : Reward->DeltaPriorityActionAfterDo)
	{
		AIInfo.WTrait.ChangePriorityAfterDoAction(Delta.Key, Delta.Value);
	}

	for (const auto& Delta : Reward->DeltaPriorityActionAfterDoAnother)
	{
		AIInfo.WTrait.ChangePriorityAfterDoAnotherAction(Delta.Key, Delta.Value);
	}
}

void UAIActionSelector::ClearAllRewards()
{
	AIInfo.ActionPriorities.Empty();
	AIInfo.RecipePriorities.Empty();
	AIInfo.KnownActorTypes.Empty();
	AIInfo.KnownResourceTypes.Empty();
}

int32 UAIActionSelector::GetSizeTreeToTreeElementWithoutTreeElementList(EActionTreeElementName Name, const TArray<EActionTreeElementName>& SuperfloursList) const
{
	return ActionTree->GetSizeTreeToTreeElementWithoutTreeElementList(Name, SuperfloursList);
}

void UAIActionSelector::UpdateActionAndRecipeMaxPriority()
{
	AIInfo.ActionMaxPriority = 0;
	AIInfo.RecipeMaxPriority = 0;

	for (const auto& Recipe : AIInfo.RecipePriorities)
	{
		if (Recipe.Value > AIInfo.RecipeMaxPriority)
		{
			AIInfo.RecipeMaxPriority = Recipe.Value;
		}
	}

	//if (AIInfo.ActionPriorities.Contains(EActionType::MakeRecipe))
	//{
	//	AIInfo.ActionPriorities[EActionType::MakeRecipe] = AIInfo.RecipeMaxPriority;
	//}

	for (const auto& Action : AIInfo.ActionPriorities)
	{
		if (Action.Value > AIInfo.ActionMaxPriority)
		{
			AIInfo.ActionMaxPriority = Action.Value;
		}
	}


}

void UAIActionSelector::RemoveTreeElementsFromList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const
{
	ActionTree->RemoveTreeElementsFromList(List, ElementForRemove);
}

void UAIActionSelector::RemoveTreeElementsFromRemovableList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const
{
	ActionTree->RemoveTreeElementsFromRemovableList(List, ElementForRemove);
}

const FSupportAIInfo& UAIActionSelector::GetAIInfo() const
{
	return AIInfo;
}

void UAIActionSelector::LoadAIInfo(FSupportAIInfo&& Info)
{
	AIInfo = Info;
}