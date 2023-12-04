#include "MainAIController.h"


#include "WorkerHub.h"
#include "InteractableActor.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"
#include "MainGameStateBase.h"
#include "ActionTree.h"
#include "ActionTreeElement.h"
#include "KnownObjectInfo.h"
#include "WorkerAddon.h"
#include "CoreHUD.h"

#include "EnumActionList.h"
#include "EnumRecipeType.h"
#include "EnumResourceType.h"
#include "MainPlayer.h"

bool AMainAIController::AddNewTreeElementListToMemory(const TArray<EActionTreeElementName>& TreeElementList)
{
	return Cast<AWorkerHub>(this->GetPawn())->AddNewTreeElementListToMemory(TreeElementList);
}
#pragma optimize("", off)
bool AMainAIController::GenerateNewAction()
{

	AWorkerHub* WHub = Cast<AWorkerHub>(this->GetPawn());

	if (!WHub)
		return false;

	TArray<TWeakObjectPtr<AInteractableActor>> AllSpottingPossibleInteractableACtorForAction
		= Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->FindSpottingInteractableActorPossibleForAction(WHub->GetActorLocation(), WHub->GetWorkerSpottingRadiusSq());

	TArray<TObjectPtr<UKnownObjectInfo>> AllKnownObject = WHub->GetAllKnownInteractableActor();

	TArray<ActionOnInteractableActor> AvailableAction;

	for (const auto& Action : WHub->GetActionList())
	{
		if (!WHub->CanMakeAction(Action))
			continue;

		if (!NeedInteractableActorForAction(Action))
		{
			AvailableAction.Add(ActionOnInteractableActor(Action, nullptr));
			continue;
		}

		for (const auto& IActor : AllSpottingPossibleInteractableACtorForAction)
		{
			if (!IActor->HasAction(Action))
				continue;

			AvailableAction.Add(ActionOnInteractableActor(Action, IActor));
		}

		for (const auto& Object : AllKnownObject)
		{
			if (!Object->HasAction(Action))
				continue;
			
			bool AlreadyAdded = false;
			for (const auto& Struct : AvailableAction)
			{
				if (Struct.InteractableActor == Object->GetInteractableActor())
				{
					AlreadyAdded = true;
					break;
				}
			}

			if (!AlreadyAdded)
			{
				AvailableAction.Add(ActionOnInteractableActor(Action, Object->GetInteractableActor()));
			}
		}

	}

	ActionOnInteractableActor NewAction = ActionSelector.GenerateNewAction(AvailableAction);

	if (NewAction.Action == EActionType::Invalid)
		return false;

	Cast<AWorkerHub>(this->GetPawn())->PrepairToAction(NewAction.Action, NewAction.InteractableActor, NewAction.Recipe);


	return true;
}
#pragma optimize("", on)
void AMainAIController::CreateActionTree()
{
	ActionTree = NewObject<UActionTree>(this);
	ActionTree->CreateActionTree();

	ActionSelector.SetActionTree(ActionTree, this);
}

void AMainAIController::AddNewLogLine()
{
	/*
	const EActionType Action = Cast<AWorkerHub>(this->GetPawn())->GetCurrentAction()->GetActionType();
	
	FString Line;

	Line = EActionToFString(Action);
	*/
}

void AMainAIController::ActionCompleted(EActionType AType, EInteractableActorType ActorType)
{
	ActionSelector.ActionCompleted(AType, ActorType);
}

void AMainAIController::MakeRecipeCompleted(ERecipeType Recipe)
{
	ActionSelector.MakeRecipeCompleted(Recipe);
}

bool AMainAIController::NeedInteractableActorForAction(EActionType Type) const
{
	return Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->NeedInteractableActorForAction(Type);
}

const TArray<EResourceType>* AMainAIController::GetResourcesMinedAction(EActionType Type) const
{
	return Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetResourcesMinedAction(Type);
}

int32 AMainAIController::GetSizeTreeToTreeElementWithoutTreeElementList(EActionTreeElementName Name, const TArray<EActionTreeElementName>& SuperfloursList) const
{
	return ActionSelector.GetSizeTreeToTreeElementWithoutTreeElementList(Name, SuperfloursList);
}

void AMainAIController::AddNewTreeElementToMemory(EActionTreeElementName Name, const FElementReward* Reward)
{
	ActionSelector.AddNewTreeElementToMemory(Name, Reward);
}

TArray<EActionTreeElementName> AMainAIController::GetAllUnknownTreeElementsListToTreeElement(EActionTreeElementName Name) const
{
	return ActionSelector.GetAllUnknownTreeElementsListToTreeElement(Name);
}

TArray<EActionTreeElementName> AMainAIController::GetAllDeletableTreeElementListToTreeElement(EActionTreeElementName Name) const
{
	return ActionSelector.GetAllDeletableTreeElementListToTreeElement(Name);
}

void AMainAIController::ClearWorkerAchivements()
{
	ActionSelector.ClearWorkerAchivements();
}

void AMainAIController::RemoveTreeElement(EActionTreeElementName Name)
{
	ActionSelector.RemoveTreeElement(Name);
}

#pragma optimize("", off)
FVector AMainAIController::GetRandomPointInRadiusAroundPoint(const FVector& Destination, int32 Radius) const
{
	FVector Result;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this->GetWorld());

	if (!NavSys->K2_GetRandomPointInNavigableRadius(this->GetWorld(), Destination, Result, Radius))
	{
		return FVector::ZeroVector;
	}

	Result = NavSys->GetRandomPointInNavigableRadius(this->GetWorld(), Destination, Radius);

	return Result;
}
#pragma optimize("", on)
TArray<FVector> AMainAIController::GetPathFromPointToPoint(const FVector& Source, const FVector& Destination) const
{
	TArray<FVector> Result;
	FNavPathSharedPtr Path;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this->GetWorld());

	const ANavigationData* NavData = NavSys->GetNavDataForProps(this->GetNavAgentPropertiesRef(), this->GetNavAgentLocation());

	const FSharedConstNavQueryFilter NavFilter = UNavigationQueryFilter::GetQueryFilter(*NavData, this, nullptr);
	FPathFindingQuery Query = FPathFindingQuery(*this, *NavData, Source, Destination, NavFilter);
	Query.SetAllowPartialPaths(true);

	FPathFindingResult PathResult = NavSys->FindPathSync(Query);
	PathResult.Path->EnableRecalculationOnInvalidation(true);
	Path = PathResult.Path;

	const TArray<FNavPathPoint>& Points = Path->GetPathPoints();

	for (const auto& Point : Points)
	{
		Result.Add(Point.Location);
	}
	return Result;
}

TWeakObjectPtr<AInteractableActor> AMainAIController::FindInteractableActorForCreateRecipe(EInteractableActorType Type) const
{
	AWorkerHub* WHub = Cast<AWorkerHub>(this->GetPawn());

	TArray<TWeakObjectPtr<AInteractableActor>> ActorsInSpottingArea = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->
		FindInteractableActorInArea(Type, WHub->GetActorLocation(), WHub->GetWorkerSpottingRadiusSq());

	TArray<TWeakObjectPtr<AInteractableActor>> ActorKnownWHub = WHub->GetKnownInteractableActorByType(Type);

	int32 SumCount = ActorsInSpottingArea.Num() + ActorKnownWHub.Num();

	if (SumCount == 0)
		return nullptr;

	int32 RandObj = FMath::RandRange(0, SumCount - 1);

	if (RandObj < ActorsInSpottingArea.Num())
		return ActorsInSpottingArea[RandObj];

	RandObj -= ActorsInSpottingArea.Num();

	return ActorKnownWHub[RandObj];
}

TWeakObjectPtr<ABuilding> AMainAIController::FindBuildingForCreateRecipe(EBuildingType Type) const
{
	AWorkerHub* WHub = Cast<AWorkerHub>(this->GetPawn());

	TArray<TWeakObjectPtr<ABuilding>> BuildingInArea = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->
		FindBuildingInArea(Type, WHub->GetActorLocation(), WHub->GetWorkerSpottingRadiusSq());

	if (BuildingInArea.Num() == 0)
		return nullptr;

	return BuildingInArea[FMath::RandRange(0, BuildingInArea.Num() - 1)];
}

void AMainAIController::RemoveTreeElementsFromList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const
{
	ActionSelector.RemoveTreeElementsFromList(List, ElementForRemove);
}

void AMainAIController::RemoveTreeElementsFromRemovableList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const
{
	ActionSelector.RemoveTreeElementsFromRemovableList(List, ElementForRemove);
}

const FSupportAIInfo& AMainAIController::GetAIInfo() const
{
	return ActionSelector.GetAIInfo();
}

void AMainAIController::LoadAIInfo(FSupportAIInfo&& Info)
{
	ActionSelector.LoadAIInfo(MoveTemp(Info));
}

void AMainAIController::ChangePriorityAfterDoAction(EActionType Type)
{
	ActionSelector.ChangePriorityAfterDoAction(Type);
}