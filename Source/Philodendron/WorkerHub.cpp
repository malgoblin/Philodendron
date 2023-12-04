// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerHub.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "Components/BoxComponent.h"
#include "KnownObjectInfo.h"
#include "Resource.h"
#include "MainAIController.h"
#include "InteractableActor.h"
#include "KnownObjectInfo.h"
#include "WorkerPawn.h"
#include "MainGameStateBase.h"
#include "InteractableActor.h"
#include "WorkerMap.h"

#include "WorkerAddon.h"
#include "MainPlayer.h"

#include "EnumActionList.h"
#include "EnumRecipeType.h"
#include "EnumWorkerAddonType.h"
#include "EnumActionTreeElementName.h"
AWorkerHub::AWorkerHub()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	WorkerHubFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("WorkerFlipbookComponent"));

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("WorkerCollision"));

	WorkerHubFlipbookComponent->SetupAttachment(RootComponent);

	BoxCollisionComponent->SetupAttachment(RootComponent);

	this->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
}

void AWorkerHub::InitWorkerHub(const FString& HName)
{
	ConnectedPawn.Empty();
	SocketPossition.Empty();

	HubInfo.HubName = HName;
	HubLog.InitHubLog(&HubInfo.HubName);

	ConnectedPawn.Add(EWorkerType::Left, nullptr);
	ConnectedPawn.Add(EWorkerType::Right, nullptr);
	ConnectedPawn.Add(EWorkerType::Top, nullptr);

	SocketPossition.Add(EWorkerType::Left, "SocketLeft");
	SocketPossition.Add(EWorkerType::Right, "SocketRight");
	SocketPossition.Add(EWorkerType::Top, "SocketTop");


	WorkerHubFlipbook = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetWorkerHubFlipbook();

	WorkerHubFlipbookComponent->SetFlipbook(WorkerHubFlipbook);
	WorkerHubFlipbookComponent->CastShadow = true;

	HubInfo.WorkerRestCount = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetWorkerRestTime();
	HubInfo.DefaultWorkerSpeed = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetDefaultWorkerSpeed();
	HubInfo.DefaultWorkerSpottingRadius = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetDefaultWorkerSpottingRadius();
}

void AWorkerHub::BeginPlay()
{
	Super::BeginPlay();
	
}
#pragma optimize("", off)
void AWorkerHub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!HubInfo.bTurnOnWorker)
		return;

	if (HubInfo.HubStatus == EWorkerHubStatus::Rest)
	{
		HubInfo.CurrentWorkerRestCount -= DeltaTime;
		if (HubInfo.CurrentWorkerRestCount < 0)
		{
			HubInfo.CurrentWorkerRestCount = 0;
			HubInfo.HubStatus = EWorkerHubStatus::Free;
		}
		return;
	}

	if (HubInfo.HubStatus == EWorkerHubStatus::Free)
	{
		if (!Cast<AMainAIController>(this->GetController())->GenerateNewAction())
		{
			SetHubRest();
		}
		return;
	}
	
	if (HubInfo.HubStatus == EWorkerHubStatus::MakingAction)
		return;
	
	if (HubInfo.Velocity == FVector::ZeroVector && HubInfo.HubStatus == EWorkerHubStatus::PrepairToAction)
	{
		Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->ContinueToPrepair(this);
	}

	FVector CurLocation = GetActorLocation();

	const float Len = HubInfo.DefaultWorkerSpeed * DeltaTime;

	FVector NewLocation = CurLocation + HubInfo.Velocity * Len;

	if (FVector::DistSquared(CurLocation, HubInfo.NextPoint) < Len * Len)
	{
		NewLocation = HubInfo.NextPoint;
	}

	SetActorLocation(NewLocation);
	CurLocation = GetActorLocation();

	if (HubInfo.NextPoint.IsZero())
		return;

	if (HubInfo.CPoint == (HubInfo.PathToDestination.Num() - 1) && (FVector::DistSquared2D(HubInfo.NextPoint, CurLocation) < 1.f))
	{
		HubInfo.Velocity = FVector::ZeroVector;
		HubInfo.NextPoint = FVector::ZeroVector;
		HubInfo.CPoint = 0;
		HubInfo.DistanceToPoint = 0;
		Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->ContinueToPrepair(this);
		return;
	}

	if (FVector::DistSquared2D(HubInfo.NextPoint, CurLocation) < 1.f)
	{
		++HubInfo.CPoint;
		HubInfo.NextPoint = HubInfo.PathToDestination[HubInfo.CPoint];
		UpdateVelocity();
		return;
	}
}
#pragma optimize("", on)

#pragma optimize("", off)
bool AWorkerHub::MoveToPointOnDistance(const FVector& Point, int32 Dist)
{
	FVector TempPoint = Cast<AMainAIController>(this->GetController())->GetRandomPointInRadiusAroundPoint(Point, Dist);

	if (TempPoint == FVector::ZeroVector)
		return false;

	HubInfo.PathToDestination = Cast<AMainAIController>(this->GetController())->GetPathFromPointToPoint(this->GetActorLocation(), Point);
	HubInfo.CPoint = 0;
	HubInfo.DistanceToPoint = Dist;
	HubInfo.NextPoint = HubInfo.PathToDestination[HubInfo.CPoint];
	UpdateVelocity();
	return true;
}
#pragma optimize("", on)
void AWorkerHub::UpdateVelocity()
{
	HubInfo.Velocity = (HubInfo.NextPoint - GetActorLocation());
	HubInfo.Velocity.Normalize();
}

void AWorkerHub::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AWorkerHub::AttachWorker(TObjectPtr<AWorkerPawn> WPawn, EWorkerType WType)
{
	if (!ConnectPawnToSocket(WPawn, WType))
	{
		return false;
	}

	WPawn->SetMasterHub(this);
	
	HubLog.AddNewWorkerPawn(WPawn->GetWorkerName());
		
	return true;
}

bool AWorkerHub::ConnectPawnToSocket(TObjectPtr<AWorkerPawn> WPawn, EWorkerType WType)
{
	if (!SocketPossition.Contains(WType) || !ConnectedPawn.Contains(WType))
	{
		return false;
	}

	auto& PawnInList = ConnectedPawn[WType];

	if (PawnInList != nullptr)
	{
		return false;
	}

	PawnInList = WPawn;
	WPawn->SetWorkerType(WType);
	WPawn->AttachToComponent(WorkerHubFlipbookComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(*SocketPossition.Find(WType)));
	++HubInfo.CountConnectedWorkers;
	
	if (WPawn->GetMemoryBroken())
		return true;

	if (!WPawn->GetReadOnly())
	{
		HubInfo.HubMemorySize += WPawn->GetPawnMemorySize();
		HubInfo.HubMaxMemorySize += WPawn->GetPawnMaxMemorySize();
	}

	for (const auto& TreeElement : WPawn->GetTreeElementList())
	{
		if (TreeElementListWithPawn.Contains(TreeElement.Key))
		{
			TreeElementListWithPawn[TreeElement.Key].PawnList.Add(WPawn);
			continue;
		}

		FPawnList PawnList;
		PawnList.PawnList.Add(WPawn);

		TreeElementListWithPawn.Add(TreeElement.Key, PawnList);

		const FElementReward* Reward = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetRewardForTreeElement(TreeElement.Key);
		Cast<AMainAIController>(this->GetController())->AddNewTreeElementToMemory(TreeElement.Key, Reward);

	}

	for (const auto& FullKnownObj : WPawn->GetObjectInfoList())
	{
		bool AlreadyExistObj = false;
		for (auto& Obj : KnownObjectInPawns)
		{
			if (Obj.Key->GetKnownObjectLocation() == FullKnownObj->GetKnownObjectLocation())
			{
				Obj.Value.PawnList.Add(WPawn);
				AlreadyExistObj = true;
				break;
			}
		}

		if (AlreadyExistObj)
			continue;

		FPawnList PawnList;
		PawnList.PawnList.Add(WPawn);
		KnownObjectInPawns.Add(FullKnownObj, PawnList);
	}

	for (const auto& PartKnownObj : WPawn->GetPartOfObjectInfo())
	{
		bool AlreadyExistObj = false;
		for (auto& Obj : KnownObjectInPawns)
		{
			if (Obj.Key->GetKnownObjectLocation() == PartKnownObj.Key->GetKnownObjectLocation())
			{
				Obj.Value.PawnList.Add(WPawn);
				AlreadyExistObj = true;
				break;
			}
		}

		if (AlreadyExistObj)
			continue;

		FPawnList PawnList;
		PawnList.PawnList.Add(WPawn);
		KnownObjectInPawns.Add(PartKnownObj.Key, PawnList);
	}

	UpdateHubActionAndRecipeList();

	return true;
}

void AWorkerHub::LoadWorkerPawn(TObjectPtr<AWorkerPawn> WPawn, EWorkerType WType)
{
	AttachWorker(WPawn, WType);
}

void AWorkerHub::FreeWorker(TObjectPtr<AWorkerPawn> WPawn)
{
	for (auto& Pawn : ConnectedPawn)
	{
		if (Pawn.Value == WPawn)
		{
			Pawn.Value = nullptr;
			--HubInfo.CountConnectedWorkers;
		}
	}
	WPawn->RemoveMasterHub();
}

const TMap<EWorkerType, TObjectPtr<AWorkerPawn>>& AWorkerHub::GetConnectedWorkers() const
{
	return ConnectedPawn;
}

void AWorkerHub::RemoveHub()
{
	this->SetActorHiddenInGame(true);
}

bool AWorkerHub::AddNewObjectInfo(const TObjectPtr<UKnownObjectInfo>& ObjInfo)
{
	int32 DataSize = ObjInfo->GetSizeData();

	if (HubInfo.HubMemorySize < DataSize)
		return false;

	bool bShareTreeElement = false;
	for (const auto& WPawn : ConnectedPawn)
	{
		if (!WPawn.Value || WPawn.Value->GetReadOnly() || WPawn.Value->GetMemoryBroken())
			continue;

		int32 PawnAvailableMemory = WPawn.Value->GetPawnMemorySize();

		if (PawnAvailableMemory == 0)
			continue;

		if (!bShareTreeElement && PawnAvailableMemory >= DataSize)
		{
			if (WPawn.Value->AddNewObjectInfo(ObjInfo))
			{
				FPawnList PList;
				PList.PawnList.Add(WPawn.Value);
				KnownObjectInPawns.Add(ObjInfo, PList);
				break;
			}
		}

		int32 PartOfTreeElement = DataSize;

		if (PawnAvailableMemory < DataSize)
		{
			PartOfTreeElement = PawnAvailableMemory;
		}

		if (WPawn.Value->AddPartOfKnownObject(ObjInfo, PartOfTreeElement))
		{
			bShareTreeElement = true;
			DataSize -= PartOfTreeElement;
			if (!KnownObjectInPawns.Contains(ObjInfo))
			{
				FPawnList PList;
				KnownObjectInPawns.Add(ObjInfo, PList);
			}
			KnownObjectInPawns[ObjInfo].PawnList.Add(WPawn.Value);
		}

		if (DataSize <= 0)
		{
			break;
		}

	}

	return true;
}

void AWorkerHub::StartWork()
{
	HubInfo.AddonList.Empty();

	for (const auto& WPawn : ConnectedPawn)
	{
		if (!WPawn.Value)
		{
			continue;
		}
		for (const auto& Addon : WPawn.Value->GetWorkerAddon())
		{
			if (!Addon.Value)
			{
				continue;
			}

			if (!HubInfo.AddonList.Contains(Addon.Value->GetAddonType()))
			{
				HubInfo.AddonList.Add(Addon.Value->GetAddonType(), 0);
			}

			HubInfo.AddonList[Addon.Value->GetAddonType()] += 1;
		}
	}

	UpdateAddonModificators();

	HubInfo.bTurnOnWorker = true;
	HubInfo.HubStatus = EWorkerHubStatus::Free;
}


bool AWorkerHub::PrepairToAction(EActionType Type, TWeakObjectPtr<AInteractableActor> IActor, ERecipeType RecipeType)
{
	if (!Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->PrepairToNewAction(this, Type, IActor, RecipeType))
		return false;

	return true;
}

bool AWorkerHub::CompletedAction(EActionType Type, EInteractableActorType ActorType)
{
	Cast<AMainAIController>(this->GetController())->ActionCompleted(Type, ActorType);

	return true;
}

void AWorkerHub::MakeRecipeCompleted(ERecipeType Recipe)
{
	Cast<AMainAIController>(this->GetController())->MakeRecipeCompleted(Recipe);

}

void AWorkerHub::ClearWorker()
{
	//ClearCurrentAction();
}


float AWorkerHub::GetWorkerSpottingRadius() const
{
	return HubInfo.DefaultWorkerSpottingRadius;
}

float AWorkerHub::GetWorkerSpottingRadiusSq() const
{
	return HubInfo.DefaultWorkerSpottingRadius * HubInfo.DefaultWorkerSpottingRadius;
}

const TArray<ERecipeType>& AWorkerHub::GetRecipeList() const
{
	return HubInfo.RecipeList;
}

const TArray<EActionType>& AWorkerHub::GetActionList() const
{
	return HubInfo.ActionList;
}

TArray<TObjectPtr<UKnownObjectInfo>> AWorkerHub::GetInfoList() const
{
	TArray<TObjectPtr<UKnownObjectInfo>> AllObjectInHub;

	for (const auto& WPawn : ConnectedPawn)
	{
		if (!WPawn.Value)
		{
			continue;
		}

		for (const auto& Obj : WPawn.Value->GetObjectInfoList())
		{
			AllObjectInHub.Add(Obj);
		}
	}
	return AllObjectInHub;
}

const TArray<FString>& AWorkerHub::GetHubLogs() const
{
	return HubLog.GetLogArray();
}

void AWorkerHub::SetHubHidden(bool bIsHidden)
{
	this->SetHidden(bIsHidden);

	for (auto& WPawn : ConnectedPawn)
	{
		if (!WPawn.Value)
		{
			continue;
		}
		WPawn.Value->SetWorkerHidden(bIsHidden);
	}
}

void AWorkerHub::GetAddonListFromPawn(TObjectPtr<AWorkerPawn> WPawn, TArray<TObjectPtr<AWorkerAddon>>& AList) const
{
	if (!WPawn)
	{
		return;
	}

	for (const auto Addon : WPawn->GetWorkerAddon())
	{
		if (!Addon.Value)
		{
			continue;
		}
		AList.Add(Addon.Value);
	}
}

const FString& AWorkerHub::GetHubName() const
{
	return HubInfo.HubName;
}

bool AWorkerHub::CanMakeAction(EActionType Type) const
{
	const TMap<EWorkerAddonType, int32>* TempConAddon = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetConditionAddons(Type);

	if (!TempConAddon)
	{
		return false;
	}

	for (const auto& Addon : *TempConAddon)
	{
		if (!HubInfo.AddonList.Contains(Addon.Key))
			return false;
		
		if (HubInfo.AddonList[Addon.Key] < Addon.Value)
			return false;
	}
	return true;
}

bool AWorkerHub::RemoveAddon(TObjectPtr<AWorkerAddon> Addon)
{
	for (auto& WPawn : ConnectedPawn)
	{
		if (!WPawn.Value)
			continue;

		if (WPawn.Value->RemoveAddon(Addon))
		{
			if (HubInfo.AddonList[Addon->GetAddonType()] == 1)
			{
				HubInfo.AddonList.Remove(Addon->GetAddonType());
				return true;
			}
			HubInfo.AddonList[Addon->GetAddonType()] -= 1;
			return true;
		}
	}
	return false;
}

int32 AWorkerHub::GetSizeTreeToTreeElementWithoutTreeElementList(EActionTreeElementName Name, const TArray<EActionTreeElementName>& SuperfloursList) const
{
	return Cast<AMainAIController>(this->GetController())->GetSizeTreeToTreeElementWithoutTreeElementList(Name, SuperfloursList);
}

bool AWorkerHub::AddNewTreeElementListToMemory(const TArray<EActionTreeElementName>& TreeElementList)
{
	int32 ListSize = 0;
	TArray<EActionTreeElementName> PossibleNewTreeElements;
	UpdateAvailableHubMemory();

	for (const auto& TreeElement : TreeElementList)
	{
		if (TreeElementListWithPawn.Contains(TreeElement))
			continue;

		int32 TreeElementSize = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetTreeElementSize(TreeElement);

		ListSize += TreeElementSize;

		if (HubInfo.HubMemorySize >= TreeElementSize)
		{
			PossibleNewTreeElements.Add(TreeElement);
		}

	}

	if (ListSize <= HubInfo.HubMemorySize)
	{
		for (const auto& TreeElement : TreeElementList)
		{
			if (TreeElementListWithPawn.Contains(TreeElement))
				continue;

			AddTreeElement(TreeElement);
		}
		return true;
	}

	if (PossibleNewTreeElements.Num() == 0)
		return false;

	AddTreeElement(PossibleNewTreeElements[FMath::RandRange(0, PossibleNewTreeElements.Num() - 1)]);
	return true;
}

void AWorkerHub::AddTreeElement(EActionTreeElementName Name)
{
	const FElementReward* Reward = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetRewardForTreeElement(Name);

	if (!Reward)
	{
		return;
	}

	int32 TreeElementSize = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetTreeElementSize(Name);

	bool bShareTreeElement = false;

	for (const auto& WPawn : ConnectedPawn)
	{
		if (!WPawn.Value || WPawn.Value->GetReadOnly() || WPawn.Value->GetMemoryBroken())
			continue;

		int32 PawnAvailableMemory = WPawn.Value->GetPawnMemorySize();

		if (PawnAvailableMemory == 0)
			continue;

		if (!bShareTreeElement && PawnAvailableMemory >= TreeElementSize)
		{
			if (WPawn.Value->AddFullTreeElementContainsInPawn(Name, TreeElementSize))
			{
				FPawnList PList;
				PList.PawnList.Add(WPawn.Value);
				TreeElementListWithPawn.Add(Name, PList);
				break;
			}
		}
		
		int32 PartOfTreeElement = TreeElementSize;

		if (PawnAvailableMemory < TreeElementSize)
		{
			PartOfTreeElement = PawnAvailableMemory;
		}

		if (WPawn.Value->AddPartOfTreeElement(Name, PartOfTreeElement))
		{
			bShareTreeElement = true;
			TreeElementSize -= PartOfTreeElement;
			if (!TreeElementListWithPawn.Contains(Name))
			{
				FPawnList PList;
				TreeElementListWithPawn.Add(Name, PList);
			}
			TreeElementListWithPawn[Name].PawnList.Add(WPawn.Value);
		}

		if (TreeElementSize <= 0)
		{
			break;
		}

	}

	UpdateAvailableHubMemory();

	Cast<AMainAIController>(this->GetController())->AddNewTreeElementToMemory(Name, Reward);

	for (const auto& Action : Reward->ActionList)
	{
		if (HubInfo.ActionList.Contains(Action))
			continue;
		HubInfo.ActionList.Add(Action);
	}

	for (const auto& Recipe : Reward->RecipeList)
	{
		if (HubInfo.RecipeList.Contains(Recipe))
			continue;
		HubInfo.RecipeList.Add(Recipe);
	}
}

void AWorkerHub::UpdateAvailableHubMemory()
{
	HubInfo.HubMemorySize = 0;
	for (const auto& WPawn : ConnectedPawn)
	{
		if (!WPawn.Value || WPawn.Value->GetReadOnly() || WPawn.Value->GetMemoryBroken())
			continue;
		HubInfo.HubMemorySize += WPawn.Value->GetPawnMemorySize();
	}
}

bool AWorkerHub::HasTreeElement(EActionTreeElementName Name) const
{
	return TreeElementListWithPawn.Contains(Name);
}

int32 AWorkerHub::GetHubMemotySize() const
{
	return HubInfo.HubMemorySize;
}

int32 AWorkerHub::GetMaxHubMemotySize() const
{
	return HubInfo.HubMaxMemorySize;
}

TArray<EActionTreeElementName> AWorkerHub::GetAllUnknownTreeElementsListToTreeElement(EActionTreeElementName Name) const
{
	return Cast<AMainAIController>(this->GetController())->GetAllUnknownTreeElementsListToTreeElement(Name);
}

void AWorkerHub::ChangePriorityAfterDoAction(EActionType Type)
{
	return Cast<AMainAIController>(this->GetController())->ChangePriorityAfterDoAction(Type);
}

TArray<EActionTreeElementName> AWorkerHub::GetAllDeletableTreeElementListToTreeElement(EActionTreeElementName Name) const
{
	return Cast<AMainAIController>(this->GetController())->GetAllDeletableTreeElementListToTreeElement(Name);
}

void AWorkerHub::RemoveTreeElementsFromList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const
{
	Cast<AMainAIController>(this->GetController())->RemoveTreeElementsFromList(List, ElementForRemove);
}

void AWorkerHub::RemoveTreeElementsFromRemovableList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const
{
	Cast<AMainAIController>(this->GetController())->RemoveTreeElementsFromRemovableList(List, ElementForRemove);
}

void AWorkerHub::RemoveTreeElementListFromMemory(const TArray<EActionTreeElementName>& TreeElementList)
{
	for (const auto& TreeElement : TreeElementList)
	{
		for (const auto& WPawn : TreeElementListWithPawn[TreeElement].PawnList)
		{
			if (WPawn->GetReadOnly())
				continue;

			WPawn->RemoveTreeElementFromMemory(TreeElement);
		}
		TreeElementListWithPawn.Remove(TreeElement);
		Cast<AMainAIController>(this->GetController())->RemoveTreeElement(TreeElement);
	}

	UpdateAvailableHubMemory();
	UpdateHubActionAndRecipeList();
}

void AWorkerHub::RemoveKnownObjectInfo(const TObjectPtr<UKnownObjectInfo> ObjInfo)
{
	for (const auto& WPawn : KnownObjectInPawns[ObjInfo].PawnList)
	{
		if (WPawn->GetReadOnly())
			continue;

		WPawn->RemoveObjectInfo(ObjInfo);
		WPawn->RemovePartOfObjectInto(ObjInfo);
	}
	KnownObjectInPawns.Remove(ObjInfo);
	
	UpdateAvailableHubMemory();
}

void AWorkerHub::UpdateHubActionAndRecipeList()
{
	HubInfo.ActionList.Empty();
	HubInfo.RecipeList.Empty();

	for (const auto& TreeElement : TreeElementListWithPawn)
	{
		const FElementReward* Reward = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetRewardForTreeElement(TreeElement.Key);

		for (const auto& Action : Reward->ActionList)
		{
			HubInfo.ActionList.Add(Action);
		}

		for (const auto& Recipe : Reward->RecipeList)
		{
			HubInfo.RecipeList.Add(Recipe);
		}
	}
}

void AWorkerHub::ClearWorkerAchivements()
{
	Cast<AMainAIController>(this->GetController())->ClearWorkerAchivements();
}

bool AWorkerHub::IsKnownObject(TWeakObjectPtr<AInteractableActor> IActor) const
{
	for (const auto& Obj : KnownObjectInPawns)
	{
		if (Obj.Key->GetInteractableActor() == IActor)
			return true;
	}

	return false;
}

void AWorkerHub::TurnOnHub()
{
	HubInfo.bTurnOnWorker = true;
}

void AWorkerHub::TurnOffHub()
{
	HubInfo.bTurnOnWorker = false;
}

void AWorkerHub::SetHubStatus(EWorkerHubStatus HStatus)
{
	HubInfo.HubStatus = HStatus;
}

void AWorkerHub::SetHubRest()
{
	HubInfo.CurrentWorkerRestCount = HubInfo.WorkerRestCount;
	HubInfo.HubStatus = EWorkerHubStatus::Rest;
}

EWorkerHubStatus AWorkerHub::GetHubStatus() const
{
	return HubInfo.HubStatus;
}

bool AWorkerHub::HasPawn(TObjectPtr<AWorkerPawn> WPawn) const
{
	if (!WPawn)
		return false;

	for (const auto& Pawn : ConnectedPawn)
	{
		if (Pawn.Value == WPawn)
			return true;
	}
	return false;
}

void AWorkerHub::GiveAllPawnAndAddonsToCore()
{
	int32 CountPawn = 0;

	for (auto& Pawn : ConnectedPawn)
	{
		if (!Pawn.Value)
			continue;

		Pawn.Value->GiveAllAddonsToCore();
		CountPawn += 1;
	}

	for (auto& Pawn : ConnectedPawn)
	{
		if (!Pawn.Value)
			continue;

		if (CountPawn > 1 && !Pawn.Value->GetReadOnly())
		{
			Pawn.Value->SetMemoryBroken();
		}

		Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->AttachWorkerToCore(Pawn.Value);
		Pawn.Value = nullptr;
	}
}

bool AWorkerHub::CanTreeElementRemoved(EActionTreeElementName Name) const
{
	for (const auto& TreeElement : TreeElementListWithPawn)
	{
		if (TreeElement.Key != Name)
			continue;

		for (const auto& WPawn : TreeElement.Value.PawnList)
		{
			if (WPawn->GetReadOnly())
				return false;
		}
	}
	return true;
}

int32 AWorkerHub::GetCountPossessedResource(EResourceType Type) const
{
	int32 Count = 0;
	for (const auto& Resource : PossessedResourceList)
	{
		if (Resource->GetResourceType() == Type)
		{
			Count += 1;
		}
	}
	return Count;
}

TArray<TObjectPtr<UKnownObjectInfo>> AWorkerHub::GetAllKnownInteractableActor() const
{
	TArray<TObjectPtr<UKnownObjectInfo>> KnownList;

	for (const auto& Actor : KnownObjectInPawns)
	{
		KnownList.Add(Actor.Key);
	}
	return KnownList;
}

TArray<TObjectPtr<UKnownObjectInfo>> AWorkerHub::GetKnownObject() const
{
	TArray<TObjectPtr<UKnownObjectInfo>> KnownList;
	for (const auto& Actor : KnownObjectInPawns)
	{
		KnownList.Add(Actor.Key);
	}
	return KnownList;
}

bool AWorkerHub::TakeResource(TObjectPtr<AResource> Resource)
{
	if (!Resource)
		return false;

	if (Resource->GetResourceWeight() > HubInfo.WeightLimit)
		return false;

	PossessedResourceList.Add(Resource);
	
	//ADD TAKE RESOURCE MODEL TO WORKER

	Resource->SetHidden(false);

	return true;
}

void AWorkerHub::UpdateAddonModificators()
{
	HubInfo.WeightLimit = 0;
	
	for (const auto& Addon : HubInfo.AddonList)
	{
		const TMap<EAddonModifiers, float>* AddonModifiers = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetAddonModifiers(Addon.Key);
		
		if (!AddonModifiers)
			continue;

		for (const auto& Modifier : *AddonModifiers)
		{
			switch (Modifier.Key)
			{
			case EAddonModifiers::IcreaseWeightLimit :
				HubInfo.WeightLimit += Modifier.Value * Addon.Value;
				break;
			}
		}
	}


}

FVector AWorkerHub::GetRandomPointInRadiusAroundPoint(const FVector& Destination, int32 Radius) const
{
	return Cast<AMainAIController>(this->GetController())->GetRandomPointInRadiusAroundPoint(Destination, Radius);
}

bool AWorkerHub::SpendResourcesForRecipe(ERecipeType Type)
{
	const TMap<EResourceType, int32>* NeededResource = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetNeededResourcesForRecipe(Type);

	if (!NeededResource)
		return false;

	TMap<EResourceType, int32> TempNeededResource = *NeededResource;

	TArray<TObjectPtr<AResource>> ResourcesForSpend;

	for (auto& Resource : TempNeededResource)
	{
		for (const auto& ExRes : PossessedResourceList)
		{
			if (ExRes->GetResourceType() == Resource.Key)
			{
				ResourcesForSpend.Add(ExRes);
				Resource.Value -= 1;
			}
			if (Resource.Value == 0)
				break;
		}
	}

	for (const auto& Res : TempNeededResource)
	{
		if (Res.Value != 0)
			return false;
	}

	for (const auto& Res : ResourcesForSpend)
	{
		PossessedResourceList.Remove(Res);
	}

	ResourcesForSpend.Empty();
	return true;
}

TArray<TWeakObjectPtr<AInteractableActor>> AWorkerHub::GetKnownInteractableActorByType(EInteractableActorType Type) const
{
	TArray<TWeakObjectPtr<AInteractableActor>> ActorList;

	for (const auto& Info : KnownObjectInPawns)
	{
		if (Info.Key->GetActorType() == Type && !ActorList.Contains(Info.Key->GetInteractableActor()))
		{
			ActorList.Add(Info.Key->GetInteractableActor());
		}
	}
	return ActorList;
}

TWeakObjectPtr<AInteractableActor> AWorkerHub::FindInteractableActorForCreateRecipe(EInteractableActorType Type) const
{
	return Cast<AMainAIController>(this->GetController())->FindInteractableActorForCreateRecipe(Type);
}

TWeakObjectPtr<ABuilding> AWorkerHub::FindBuildingForCreateRecipe(EBuildingType Type) const
{
	return Cast<AMainAIController>(this->GetController())->FindBuildingForCreateRecipe(Type);
}

int32 AWorkerHub::GetHubID() const
{
	return HubInfo.HubID;
}

void AWorkerHub::SetHubID(int32 NewID)
{
	HubInfo.HubID = NewID;
}

const FSupportHubInfo& AWorkerHub::GetHubInfo() const
{
	return HubInfo;
}

const TArray<TObjectPtr<AResource>>& AWorkerHub::GetPossessedResourceList() const
{
	return PossessedResourceList;
}

const TMap<EWorkerType, TObjectPtr<AWorkerPawn>>& AWorkerHub::GetConnectedPawn() const
{
	return ConnectedPawn;
}

void AWorkerHub::SetHubInfo(FSupportHubInfo&& Info)
{
	HubInfo = Info;
}

const FSupportAIInfo& AWorkerHub::GetAIInfo() const
{
	return Cast<AMainAIController>(this->GetController())->GetAIInfo();
}

void AWorkerHub::LoadAIInfo(FSupportAIInfo&& Info)
{
	Cast<AMainAIController>(this->GetController())->LoadAIInfo(MoveTemp(Info));
}