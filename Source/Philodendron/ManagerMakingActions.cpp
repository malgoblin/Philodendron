// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerMakingActions.h"
#include "WorkerHub.h"
#include "InteractableActor.h"
#include "KnownObjectInfo.h"
#include "MainPlayer.h"
#include "MainGameStateBase.h"
#include "Resource.h"
#include "EnumBuildingType.h"
#include "Building.h"
#include "SupportStructsForSaveLoad.h"

// Sets default values
AManagerMakingActions::AManagerMakingActions()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AManagerMakingActions::BeginPlay()
{
	Super::BeginPlay();
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SetManagerMakingActions(this);
}

void AManagerMakingActions::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickAllActions(DeltaTime);
}

void AManagerMakingActions::TickAllActions(float DeltaTime)
{
	TArray<AWorkerHub*> HubCompletedAction;

	for (auto& Action : CurrentPrepairAction)
	{
		if (Action.Key->GetHubStatus() != EWorkerHubStatus::MakingAction)
			continue;

		Action.Value.RemainingTime -= DeltaTime;

		if (Action.Value.RemainingTime <= 0.f)
		{
			FinishAction(Action.Key, Action.Value);
			HubCompletedAction.Add(Action.Key);
		}
	}

	for (const auto& WHub : HubCompletedAction)
	{
		CurrentPrepairAction.Remove(WHub);
	}
}

// !!!!!!!!!!!!!!!! PREPAIR TO ACTION !!!!!!!!!!!!!
#pragma optimize("", off)
bool AManagerMakingActions::PrepairToNewAction(TObjectPtr<AWorkerHub>& WHub, EActionType Type, TWeakObjectPtr<AInteractableActor> IActor, ERecipeType RecipeType)
{
	if (CurrentPrepairAction.Contains(WHub))
	{
		ClearObj(WHub);
		return false;
	}

	FInfoAboutAction AboutAction;

	AboutAction.CurrentInteractableActor = IActor;

	if (Type != EActionType::MakeRecipe)
	{
		AboutAction.RemainingTime = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetActionTime(Type);
	}
	else
	{
		AboutAction.RemainingTime = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetTimeToMakeRecipe(RecipeType);
	}

	AboutAction.Type = Type;
	AboutAction.RecipeType = RecipeType;
	CurrentPrepairAction.Add(WHub, AboutAction);


	if (!PrepairToAction(WHub, AboutAction))
	{
		ClearObj(WHub);
		return false;
	}

	return true;
}
#pragma optimize("", on)
bool AManagerMakingActions::ContinueToPrepair(TObjectPtr<AWorkerHub>& WHub)
{
	if (!CurrentPrepairAction.Contains(WHub))
		return false;

	if (!PrepairToAction(WHub, CurrentPrepairAction[WHub]))
	{
		ClearObj(WHub);
		return false;
	}

	return true;
}

bool AManagerMakingActions::PrepairToAction(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	switch (AboutAction.Type)
	{
	case EActionType::Research: 
	case EActionType::HarvestBush:
	case EActionType::HarvestGrass:
	case EActionType::HarvestTree:
	case EActionType::PullOutBush:
	case EActionType::HarvestClay:
	case EActionType::MiningStone:
	case EActionType::MiningGypsum:
	case EActionType::MiningCoal:
	case EActionType::MiningIron:
		return MoveToInteractableActor(WHub, AboutAction);
	case EActionType::BackToCore:
		return PrepairToBackToCore(WHub, AboutAction);
	case EActionType::MakeRecipe:
		return PrepairToMakeRecipe(WHub, AboutAction);
	default: return false;
	}
}

#pragma optimize("", off)
bool AManagerMakingActions::PrepairToBackToCore(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	int32 Distance = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetDistanceToMakeAction(AboutAction.Type);



	if (Distance < 0)
	{
		ClearObj(WHub);
		return false;
	}

	FVector Size = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetCore()->GetCoreFlipbook()->GetRenderBounds().GetBox().GetSize();
	Size.Z = 0;

	int32 MaxSize = Size.GetMax();
	Distance += MaxSize / 2;

	FVector CoreLocation = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetCoreLocation();

	auto test = FVector::DistSquared2D(WHub->GetActorLocation(), CoreLocation);
	auto test1 = WHub->GetActorLocation();


	if (FVector::DistSquared2D(WHub->GetActorLocation(), CoreLocation) <= (Distance * Distance))
	{
		WHub->SetHubStatus(EWorkerHubStatus::MakingAction);
		return true;
	}
	else if (MovableToPointOnDistabce(WHub, CoreLocation, Distance))
	{
		WHub->SetHubStatus(EWorkerHubStatus::PrepairToAction);
		return true;
	}

	ClearObj(WHub);
	return false;
}
#pragma optimize("", on)
#pragma optimize("", off)
bool AManagerMakingActions::MoveToInteractableActor(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub)
		return false;

	if (!AboutAction.CurrentInteractableActor.IsValid())
	{
		ClearObj(WHub);
		return false;
	}

	FVector Size = AboutAction.CurrentInteractableActor->GetIActorPaperFlipbook()->GetRenderBounds().GetBox().GetSize();
	Size.Z = 0;

	int32 MaxSize = Size.GetMax();

	int32 Distance = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetDistanceToMakeAction(AboutAction.Type) + MaxSize;// / 2;

	if (Distance < 0)
	{
		ClearObj(WHub);
		return false;
	}

	auto test = FVector::DistSquared2D(WHub->GetActorLocation(), AboutAction.CurrentInteractableActor->GetLocation());

	if (FVector::DistSquared2D(WHub->GetActorLocation(), AboutAction.CurrentInteractableActor->GetLocation()) <= (Distance * Distance))
	{
		bool ResultStart = false;
		switch (AboutAction.Type)
		{
		case EActionType::Research: ResultStart = StartResearch(WHub, AboutAction); break;
		case EActionType::HarvestBush: 
		case EActionType::HarvestTree: 
		case EActionType::MiningStone:
		case EActionType::HarvestClay:
		case EActionType::MiningGypsum:
		case EActionType::MiningCoal:
		case EActionType::MiningIron:
			ResultStart = StartMinerResource(WHub, AboutAction); 
			break;
		case EActionType::HarvestGrass:
		case EActionType::PullOutBush:
			ResultStart = StartHarvestAllResources(WHub, AboutAction);
			break;
		case EActionType::MakeRecipe:
			ResultStart = StartMakeRecipeOnInteractableActor(WHub, AboutAction);
			break;
		}

		if (ResultStart)
		{
			WHub->SetHubStatus(EWorkerHubStatus::MakingAction);
			return true;
		}
	}
	else if (MovableToPointOnDistabce(WHub, AboutAction.CurrentInteractableActor->GetLocation(), Distance))
	{
		WHub->SetHubStatus(EWorkerHubStatus::PrepairToAction);
		return true;
	}

	ClearObj(WHub);
	return false;
}
#pragma optimize("", on)
#pragma optimize("", off)
bool AManagerMakingActions::MoveToEmptyPlace(TObjectPtr<AWorkerHub>& WHub, float EmptyArea)
{
	if (!WHub)
		return false;

	

	int32 Distance = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetDistanceToMakeAction(CurrentPrepairAction[WHub].Type);

	if (Distance < 0)
	{
		ClearObj(WHub);
		return false;
	}

	if (Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->IsPointEmpty(WHub->GetActorLocation(), EmptyArea))
	{
		bool ResultStart = false;
		switch (CurrentPrepairAction[WHub].Type)
		{
		case EActionType::MakeRecipe: 
			ResultStart = StartMakeRecipe(WHub);
			break;
		}

		if (ResultStart)
		{
			WHub->SetHubStatus(EWorkerHubStatus::MakingAction);
			return true;
		}
	}
	else 
	{
		FVector Destination = WHub->GetRandomPointInRadiusAroundPoint(WHub->GetActorLocation(), WHub->GetWorkerSpottingRadius());

		if (Destination.IsZero())
		{
			ClearObj(WHub);
			return false;
		}

		if (MovableToPointOnDistabce(WHub, Destination, Distance))
		{
			WHub->SetHubStatus(EWorkerHubStatus::PrepairToAction);
			return true;
		}
	}

	ClearObj(WHub);
	return false;
}
#pragma optimize("", on)
#pragma optimize("", off)
bool AManagerMakingActions::PrepairToMakeRecipe(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub || AboutAction.RecipeType == ERecipeType::Invalid)
		return false;

	if (AboutAction.RecipeType == ERecipeType::IronBlock)
	{
		int32 t = 0;
		++t;
	}

	const TMap<EResourceType, int32>* NeededResources = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetNeededResourcesForRecipe(AboutAction.RecipeType);

	if (!NeededResources)
		return false;

	TArray<EResourceType> NeededToFind;

	for (const auto& Resource : *NeededResources)
	{
		int32 NeededCount = Resource.Value - WHub->GetCountPossessedResource(Resource.Key);

		for (int32 i = 0; i < NeededCount; ++i)
		{
			NeededToFind.Add(Resource.Key);
		}
	}

	if (NeededToFind.Num() == 0)
	{
		EPlaceWhereMakingRecipe PlaceToFind = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetPlaceWhereMakingRecipe(AboutAction.RecipeType);

		if (PlaceToFind == EPlaceWhereMakingRecipe::Invalid)
			return false;

		switch (PlaceToFind)
		{
		case EPlaceWhereMakingRecipe::Empty: 
			return MoveToEmptyPlace(WHub, Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetEmptyAreaAroundEmptyPlace(AboutAction.RecipeType));

		case EPlaceWhereMakingRecipe::InteractableActor:

			if (!AboutAction.CurrentInteractableActor.IsValid())
			{
				EInteractableActorType NeededIActor = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetNeededInteractableActorWhereCreateRecipe(AboutAction.RecipeType);
				if (NeededIActor == EInteractableActorType::Invalid)
					return false;
				AboutAction.CurrentInteractableActor = WHub->FindInteractableActorForCreateRecipe(NeededIActor);
				if (!AboutAction.CurrentInteractableActor.IsValid())
					return false;
			}
			return MoveToInteractableActor(WHub, AboutAction);

		case EPlaceWhereMakingRecipe::Building:
			if (!AboutAction.CurrentBuilding.IsValid())
			{
				EBuildingType NeededBuilding = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetNeededBuildingForCretingRecipe(AboutAction.RecipeType);
				if (NeededBuilding == EBuildingType::Invalid)
					return false;
				AboutAction.CurrentBuilding = WHub->FindBuildingForCreateRecipe(NeededBuilding);
				if (!AboutAction.CurrentBuilding.IsValid())
					return false;
			}
			return MoveToBuilding(WHub, AboutAction);
		}

	}



	return PrepairToTakeResource(WHub, NeededToFind);
}
#pragma optimize("", on)
bool AManagerMakingActions::PrepairToTakeResource(TObjectPtr<AWorkerHub>& WHub, const TArray<EResourceType>& TypeList)
{
	if (!WHub || !CurrentPrepairAction.Contains(WHub))
		return false;

	if (!CurrentPrepairAction[WHub].CurrentResource.IsValid())
	{
		TWeakObjectPtr<AResource> ClosestResource = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->FindClosestResourceInArea(TypeList, WHub->GetActorLocation(), WHub->GetWorkerSpottingRadiusSq());

		if (!ClosestResource.IsValid())
		{
			//ADD CREATE NEEDED RESOURCE
			return false;
		}

		CurrentPrepairAction[WHub].CurrentResource = ClosestResource;
	}


	CurrentPrepairAction[WHub].SubActionType = EActionType::TakeResource;

	return MoveToResource(WHub, CurrentPrepairAction[WHub]);
}
#pragma optimize("", off)
bool AManagerMakingActions::MoveToResource(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub)
		return false;

	if (!AboutAction.CurrentResource.IsValid())
	{
		ClearObj(WHub);
		return false;
	}

	FVector Size = AboutAction.CurrentResource->GetIActorPaperFlipbook()->GetRenderBounds().GetBox().GetSize();
	Size.Z = 0;

	int32 MaxSize = Size.GetMax();

	int32 Distance = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetDistanceToMakeAction(AboutAction.SubActionType) + MaxSize;

	if (Distance < 0)
	{
		ClearObj(WHub);
		return false;
	}

	auto test1 = WHub->GetActorLocation();
	auto test2 = AboutAction.CurrentResource->GetLocation();

	auto test = FVector::DistSquared2D(WHub->GetActorLocation(), AboutAction.CurrentResource->GetLocation());

	if (FVector::DistSquared2D(WHub->GetActorLocation(), AboutAction.CurrentResource->GetLocation()) <= (Distance * Distance))
	{
		bool ResultStart = false;
		switch (AboutAction.SubActionType)
		{
		case EActionType::TakeResource: 
			ResultStart = FinishTakeResource(WHub, AboutAction);
			break;
		}

		if (ResultStart)
		{
			//ClearObj(WHub);
			return true;
		}
	}
	else if (MovableToPointOnDistabce(WHub, AboutAction.CurrentResource->GetLocation(), Distance))
	{
		WHub->SetHubStatus(EWorkerHubStatus::PrepairToAction);
		return true;
	}

	ClearObj(WHub);
	return false;
}
#pragma optimize("", on)
bool AManagerMakingActions::MoveToBuilding(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub)
		return false;

	if (!AboutAction.CurrentBuilding.IsValid())
	{
		ClearObj(WHub);
		return false;
	}

	FVector Size = AboutAction.CurrentBuilding->GetBuildingPaperFlipbook()->GetRenderBounds().GetBox().GetSize();
	Size.Z = 0;

	int32 MaxSize = Size.GetMax();

	int32 Distance = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetDistanceToMakeAction(CurrentPrepairAction[WHub].Type) + MaxSize / 2;

	if (Distance < 0)
	{
		ClearObj(WHub);
		return false;
	}

	if (FVector::DistSquared2D(WHub->GetActorLocation(), AboutAction.CurrentBuilding->GetActorLocation()) <= (Distance * Distance))
	{
		bool ResultStart = false;
		switch (CurrentPrepairAction[WHub].Type)
		{
		case EActionType::MakeRecipe:
			ResultStart = StartMakeRecipeOnBuilding(WHub, AboutAction);
			break;
		}

		if (ResultStart)
		{
			WHub->SetHubStatus(EWorkerHubStatus::MakingAction);
			return true;
		}
	}
	else if (MovableToPointOnDistabce(WHub, AboutAction.CurrentBuilding->GetActorLocation(), Distance))
	{
		WHub->SetHubStatus(EWorkerHubStatus::PrepairToAction);
		return true;
	}

	ClearObj(WHub);
	return false;
}

bool AManagerMakingActions::MovableToPointOnDistabce(TObjectPtr<AWorkerHub>& WHub, FVector Point, int32 Dist) const
{
	return WHub->MoveToPointOnDistance(Point, Dist);
}

// !!!!!!!!!!!!!!!! START ACTION !!!!!!!!!!!!!!!!


bool AManagerMakingActions::StartResearch(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction) const
{
	if (!WHub || !AboutAction.CurrentInteractableActor.IsValid())
		return false;

	return !WHub->IsKnownObject(AboutAction.CurrentInteractableActor);
}

bool AManagerMakingActions::StartMinerResource(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction) const
{
	if (!WHub || !AboutAction.CurrentInteractableActor.IsValid())
		return false;

	const TArray<EResourceType>* AvailableResourceList = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetResourcesMinedAction(AboutAction.Type);
	const TMap<EResourceType, int32>* ExistedResourceList = AboutAction.CurrentInteractableActor->GetAvailableResourceList();

	if (!AvailableResourceList || !ExistedResourceList)
		return false;

	TArray<EResourceType> PossibleResource;

	for (const auto& Resource : *AvailableResourceList)
	{
		if (ExistedResourceList->Contains(Resource))
		{
			int32 CountResource = *(ExistedResourceList->Find(Resource));

			for (int32 i = 0; i < CountResource; ++i)
			{
				PossibleResource.Add(Resource);
			}
		}
	}

	if (PossibleResource.Num() == 0)
		return false;

	return AboutAction.CurrentInteractableActor->LockResource(PossibleResource[FMath::RandRange(0, (PossibleResource.Num() - 1))], WHub);
}

bool AManagerMakingActions::StartHarvestAllResources(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction) const
{
	if (!WHub || !AboutAction.CurrentInteractableActor.IsValid())
		return false;


	return AboutAction.CurrentInteractableActor->LockActor(WHub);
}

bool AManagerMakingActions::StartTakeResource(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction) const
{
	if (!WHub || !AboutAction.CurrentInteractableActor.IsValid())
		return false;


	return true;
}

bool AManagerMakingActions::StartMakeRecipe(TObjectPtr<AWorkerHub>& WHub) const
{
	if (!WHub)
		return false;

	return WHub->SpendResourcesForRecipe(CurrentPrepairAction[WHub].RecipeType);
}

bool AManagerMakingActions::StartMakeRecipeOnInteractableActor(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction) const
{
	if (!WHub || !AboutAction.CurrentInteractableActor.IsValid())
		return false;

	if (!AboutAction.CurrentInteractableActor->LockActor(WHub))
		return false;

	return WHub->SpendResourcesForRecipe(AboutAction.RecipeType);
}

bool AManagerMakingActions::StartMakeRecipeOnBuilding(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub || !AboutAction.CurrentBuilding.IsValid())
		return false;

	if (AboutAction.CurrentBuilding->IsBuildingBusy())
		return false;

	ENeedWorkerToMakeRecipe NeedWorker = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetNeededWorkerToMakeRecipe(AboutAction.RecipeType);
	
	if (NeedWorker == ENeedWorkerToMakeRecipe::Invalid)
		return false;

	if (!WHub->SpendResourcesForRecipe(AboutAction.RecipeType))
		return false;

	if (!AboutAction.CurrentBuilding->MakeRecipe(AboutAction.RecipeType))
		return false;

	if (NeedWorker == ENeedWorkerToMakeRecipe::DontNeedworker)
	{
		WHub->CompletedAction(AboutAction.Type, EInteractableActorType::Invalid);
		WHub->MakeRecipeCompleted(AboutAction.RecipeType);
		return false;
	}

	return true;
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!! FINISH ACTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#pragma optimize("", off)
void AManagerMakingActions::FinishAction(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub)
		return;

	bool ResultFinishAction = false;

	switch (AboutAction.Type)
	{
	case EActionType::Research: ResultFinishAction = FinishResearch(WHub, AboutAction); break;
	case EActionType::HarvestTree:
	case EActionType::HarvestBush:
	case EActionType::MiningStone:
	case EActionType::HarvestClay:
	case EActionType::MiningGypsum:
	case EActionType::MiningCoal:
	case EActionType::MiningIron:
		ResultFinishAction = FinishMinerResource(WHub, AboutAction); 
		break;
	case EActionType::HarvestGrass:
		ResultFinishAction = FinishHarvestAllResources(WHub, AboutAction);
		break;
	case EActionType::PullOutBush:
		ResultFinishAction = FinishPullOutBush(WHub, AboutAction);
		break;
	case EActionType::BackToCore: ResultFinishAction = FinishBackToCore(WHub, AboutAction); break;
	case EActionType::MakeRecipe: ResultFinishAction = FinishMakeRecipe(WHub, AboutAction); break;
	default: break;
	}

	if (ResultFinishAction)
	{
		if (!AboutAction.CurrentInteractableActor.IsValid())
		{
			WHub->CompletedAction(AboutAction.Type, EInteractableActorType::Invalid);
		}
		else
		{
			WHub->CompletedAction(AboutAction.Type, AboutAction.CurrentInteractableActor->GetActorType());
		}

		if (AboutAction.Type == EActionType::MakeRecipe)
		{
			WHub->MakeRecipeCompleted(AboutAction.RecipeType);
		}
	}

	WHub->SetHubRest();
	return;
}
#pragma optimize("", on)
#pragma optimize("", off)
bool AManagerMakingActions::FinishResearch(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub || !AboutAction.CurrentInteractableActor.IsValid())
		return false;

	UKnownObjectInfo* ObjectInfo = NewObject<UKnownObjectInfo>(this);

	FSupportKnownObjectInfo TempKnownObjectInfo;

	TempKnownObjectInfo.DataSize = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetSizeInfoAboutIActor(AboutAction.CurrentInteractableActor->GetActorType());
	TempKnownObjectInfo.ActionList = *(Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetAvailableActions(AboutAction.CurrentInteractableActor->GetActorType()));
	TempKnownObjectInfo.ActorType = AboutAction.CurrentInteractableActor->GetActorType();
	TempKnownObjectInfo.KnownObjectLocation = AboutAction.CurrentInteractableActor->GetLocation();

	ObjectInfo->SetInfo(MoveTemp(TempKnownObjectInfo));
	ObjectInfo->SetWeakObjectPtr(AboutAction.CurrentInteractableActor);

	return WHub->AddNewObjectInfo(ObjectInfo);
}
#pragma optimize("", on)
bool AManagerMakingActions::FinishMinerResource(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction) const
{
	if (!WHub || !AboutAction.CurrentInteractableActor.IsValid())
		return false;

	return AboutAction.CurrentInteractableActor->CreateNewResource(WHub);
}

bool AManagerMakingActions::FinishBackToCore(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub)
		return false;

	TWeakObjectPtr<AMainPlayer> TempCore = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetCore();

	if (!TempCore.IsValid())
		return false;

	return TempCore->AttachHub(WHub);
}

void AManagerMakingActions::ClearObj(TObjectPtr<AWorkerHub>& WHub)
{
	if (CurrentPrepairAction.Contains(WHub))
	{
		WHub->ChangePriorityAfterDoAction(CurrentPrepairAction[WHub].Type);
	}

	CurrentPrepairAction.Remove(WHub);
	WHub->SetHubRest();
}
#pragma optimize("", off)
bool AManagerMakingActions::FinishHarvestAllResources(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub || !AboutAction.CurrentInteractableActor.IsValid())
		return false;

	return AboutAction.CurrentInteractableActor->ExtractAllResources(WHub);
}
#pragma optimize("", on)
bool AManagerMakingActions::FinishPullOutBush(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub || !AboutAction.CurrentInteractableActor.IsValid())
		return false;

	FVector ActorPossition = AboutAction.CurrentInteractableActor->GetLocation();
	bool HarvestAllResouceResult = FinishHarvestAllResources(WHub, AboutAction);

	if (!HarvestAllResouceResult)
		return false;

	return Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->CreateHole(ActorPossition);
}

bool AManagerMakingActions::FinishTakeResource(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub || !AboutAction.CurrentResource.IsValid())
		return false;

	if (Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetResourceToWorker(WHub, AboutAction.CurrentResource))
	{
		AboutAction.CurrentResource = nullptr;
		AboutAction.SubActionType = EActionType::Invalid;
		return true;
	}

	return false;
}

bool AManagerMakingActions::FinishMakeRecipe(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub || AboutAction.RecipeType == ERecipeType::Invalid)
		return false;

	EPlaceWhereMakingRecipe PlaceForMaking = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetPlaceWhereMakingRecipe(AboutAction.RecipeType);


	if (PlaceForMaking == EPlaceWhereMakingRecipe::Invalid)
		return false;

	switch (PlaceForMaking)
	{
	case EPlaceWhereMakingRecipe::Empty: return MakeRecipeForEmptyPlace(WHub, AboutAction.RecipeType);
	case EPlaceWhereMakingRecipe::InteractableActor: return MakeRecipeOnInteractableActor(WHub, AboutAction);
	case EPlaceWhereMakingRecipe::Building: return MakeRecipeInBuilding(WHub, AboutAction);
	}

	return false;
}

bool AManagerMakingActions::MakeRecipeForEmptyPlace(TObjectPtr<AWorkerHub>& WHub, ERecipeType Type)
{
	EGlobalRecipeType GlobalType = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetGlobalRecipeType(Type);
	
	if (GlobalType == EGlobalRecipeType::Invalid)
		return false;

	switch (GlobalType)
	{
	case EGlobalRecipeType::InteractableActor:
	{
		EInteractableActorType IActorType = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetInteractableActorForCreating(Type);

		if (IActorType == EInteractableActorType::Invalid)
			return false;

		return Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->CreateInteractableActorFromRecipe(IActorType, WHub->GetActorLocation());
	}
	
	case EGlobalRecipeType::Resource:
	{
		EResourceType ResourceType = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetResourceWhichCreating(Type);

		if (ResourceType == EResourceType::Invalid)
			return false;

		return Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->CreateNewResource(ResourceType, WHub->GetActorLocation(), 5.f);
	}
	case EGlobalRecipeType::Building:
	{
		EBuildingType BuildingType = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetBuildingWhichCreating(Type);

		if (BuildingType == EBuildingType::Invalid)
			return false;

		return Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->CreateBuildingOnEmptyPlace(BuildingType, WHub->GetActorLocation());
	}
	}


	return false;
}

bool AManagerMakingActions::MakeRecipeOnInteractableActor(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	EGlobalRecipeType GlobalType = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetGlobalRecipeType(AboutAction.RecipeType);

	if (GlobalType == EGlobalRecipeType::Invalid)
		return false;

	switch (GlobalType)
	{
	case EGlobalRecipeType::Building:
		EBuildingType BuildingType = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetBuildingWhichCreating(AboutAction.RecipeType);

		if (BuildingType == EBuildingType::Invalid)
			return false;

		return Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->CreateBuildingOnInteractableActor(BuildingType, AboutAction.CurrentInteractableActor, WHub);
	}

	return false;
}

bool AManagerMakingActions::MakeRecipeInBuilding(TObjectPtr<AWorkerHub>& WHub, FInfoAboutAction& AboutAction)
{
	if (!WHub || !AboutAction.CurrentBuilding.IsValid())
		return false;

	return true;
}

int32 AManagerMakingActions::CountResourcesInList(const TArray<EResourceType> ResourceList, EResourceType Type) const
{
	int32 Count = 0;
	for (const auto& Resource : ResourceList)
	{
		if (Resource == Type)
		{
			Count += 1;
		}
	}
	return Count;
}

const TMap<TObjectPtr<AWorkerHub>, FInfoAboutAction>& AManagerMakingActions::GetCurrentPrepairAction() const
{
	return CurrentPrepairAction;
}

void AManagerMakingActions::LoadMakingActionList(TMap<TObjectPtr<AWorkerHub>, FInfoAboutAction>&& List)
{
	CurrentPrepairAction = List;
}