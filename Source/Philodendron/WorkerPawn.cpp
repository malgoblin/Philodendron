#include "WorkerPawn.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "MainAIController.h"
#include "AIController.h"
#include "EnumWorkerType.h"
#include "InteractableActor.h"

#include "KnownObjectInfo.h"
#include "NavigationSystem.h"
#include "Engine/StaticMeshSocket.h"

#include "MainGameStateBase.h"
#include "MainPlayer.h"

#include "EnumWorkerAddonType.h"

#include "Resource.h"
#include "WorkerMap.h"
#include "UObject/Object.h"
#include "WorkerAddon.h"

#include "EnumActionList.h"

AWorkerPawn::AWorkerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	this->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	WorkerPawnFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("WorkerPawnFlipbookComponent"));

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("WorkerCollision"));

	WorkerPawnFlipbookComponent->SetupAttachment(RootComponent);

	BoxCollisionComponent->SetupAttachment(RootComponent);
}

void AWorkerPawn::SetWorkerType(EWorkerType WType)
{
	UPaperFlipbook* Flipbook = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetWorkerPawnFlipbook(WType);

	if (!Flipbook)
	{
		return;
	}

	WorkerPawnFlipbookComponent->SetFlipbook(Flipbook);
	WorkerPawnFlipbookComponent->CastShadow = true;
	PawnInfo.WorkerType = WType;
}


void AWorkerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorkerPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AWorkerPawn::InitWorker()
{
	PawnInfo.PawnMemorySize = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetWorkerPawnMemorySize();

	PawnInfo.PawnMaxMemorySize = PawnInfo.PawnMemorySize;

	ConnectedAddon.Add(EAddonPossitionType::Zero, nullptr);
	ConnectedAddon.Add(EAddonPossitionType::First, nullptr);

	AddonSocketName.Add(EAddonPossitionType::Zero, "Socket_0");
	AddonSocketName.Add(EAddonPossitionType::First, "Socket_1");
}

const FString& AWorkerPawn::GetWorkerName() const
{
	return PawnInfo.WorkerPawnName;
}

void AWorkerPawn::SetWorkerName(const FString& Name)
{
	PawnInfo.WorkerPawnName = Name;
}

bool AWorkerPawn::AddNewObjectInfo(const TObjectPtr<UKnownObjectInfo>& NewObj)
{
	int32 ObjectSize = NewObj->GetSizeData();

	if (ObjectSize > PawnInfo.PawnMemorySize)
		return false;

	if (ObjectList.Find(NewObj) != INDEX_NONE)
	{
		return false;
	}

	ObjectList.Add(NewObj);
	PawnInfo.PawnMemorySize -= ObjectSize;
	return true;
}




bool AWorkerPawn::AddWorkerMap(const TObjectPtr<UWorkerMap>& WMap)
{
	int32 MapSize = WMap->GetSizeData();

	if (MapSize > PawnInfo.PawnMemorySize)
		return false;

	if (WorkerMap)
	{
		return false;
	}

	WorkerMap = WMap;
	PawnInfo.PawnMemorySize -= MapSize;
	return true;
}

bool AWorkerPawn::AddPartOfMap(const TObjectPtr<UWorkerMap>& WMap, int32 Part)
{
	if (PartOfMap)
		return false;

	PartOfMap = WMap;
	PartOfMapSize = Part;

	PawnInfo.PawnMemorySize -= Part;
	return true;
}

bool AWorkerPawn::AddFullTreeElementContainsInPawn(EActionTreeElementName TreeElement, int32 FullSize)
{
	if (PawnInfo.FullTreeElementContainsInPawn.Contains(TreeElement))
		return false;

	PawnInfo.FullTreeElementContainsInPawn.Add(TreeElement, FullSize);
	PawnInfo.PawnMemorySize -= FullSize;
	return true;
}

bool AWorkerPawn::AddPartOfTreeElement(EActionTreeElementName Name, int32 Part)
{
	if (PawnInfo.PartOfTreeElement.Contains(Name))
		return false;

	PawnInfo.PartOfTreeElement.Add(Name, Part);
	PawnInfo.PawnMemorySize -= Part;
	return true;
}


bool AWorkerPawn::AddPartOfKnownObject(const TObjectPtr<UKnownObjectInfo>& NewObj, int32 Part)
{
	if (PartOfObjectInfo.Contains(NewObj))
		return false;

	PartOfObjectInfo.Add(NewObj, Part);
	PawnInfo.PawnMemorySize -= Part;
	return true;
}


const TArray<TObjectPtr<UKnownObjectInfo>>& AWorkerPawn::GetObjectInfoList() const
{
	return ObjectList;
}

const TObjectPtr<UWorkerMap>& AWorkerPawn::GetWorkerMap() const
{
	return WorkerMap;
}

void AWorkerPawn::RemoveObjectInfo(const TObjectPtr<UKnownObjectInfo>& NewObj)
{
	if (!ObjectList.Contains(NewObj))
	{
		return;
	}

	ObjectList.Remove(NewObj);
	PawnInfo.PawnMemorySize += NewObj->GetSizeData();

}

void AWorkerPawn::RemovePartOfObjectInto(const TObjectPtr<UKnownObjectInfo>& NewObj)
{
	if (!PartOfObjectInfo.Contains(NewObj))
		return;

	PawnInfo.PawnMemorySize += PartOfObjectInfo[NewObj];
	PartOfObjectInfo.Remove(NewObj);
}

void AWorkerPawn::RemoveTreeElement(EActionTreeElementName TreeElement)
{
	if (!PawnInfo.FullTreeElementContainsInPawn.Contains(TreeElement))
		return;

	PawnInfo.PawnMemorySize += PawnInfo.FullTreeElementContainsInPawn[TreeElement];
	PawnInfo.FullTreeElementContainsInPawn.Remove(TreeElement);
}

void AWorkerPawn::RemovePartOfTreeElement(EActionTreeElementName TreeElement)
{
	if (!PawnInfo.PartOfTreeElement.Contains(TreeElement))
		return;

	PawnInfo.PawnMemorySize += PawnInfo.PartOfTreeElement[TreeElement];
	PawnInfo.PartOfTreeElement.Remove(TreeElement);
}

void AWorkerPawn::RemoveTreeElementFromMemory(EActionTreeElementName TreeElement)
{
	RemoveTreeElement(TreeElement);
	RemovePartOfTreeElement(TreeElement);
}

void AWorkerPawn::RemovePartOfMap()
{
	if (!PartOfMap)
		return;

	PawnInfo.PawnMemorySize += PartOfMapSize;
	PartOfMap = nullptr;
	PartOfMapSize = 0;

}

void AWorkerPawn::RemoveWorkerMap()
{
	if (WorkerMap)
	{
		PawnInfo.PawnMemorySize += WorkerMap->GetSizeData();
	}
	WorkerMap = nullptr;
}

bool AWorkerPawn::IsWorkerKnownAboutInteractableActor(const TObjectPtr<UKnownObjectInfo>& Target) const
{
	for (const auto& Info : ObjectList)
	{
		if (Info->GetKnownObjectLocation() == Target->GetKnownObjectLocation())
		{
			return true;
		}
	}

	if (!WorkerMap)
	{
		return false;
	}

	for (const auto& Info : WorkerMap->GetMapInfo())
	{
		if (Info->GetKnownObjectLocation() == Target->GetKnownObjectLocation())
		{
			return true;
		}
	}

	return false;
}

void AWorkerPawn::SetMasterHub(const TObjectPtr<AWorkerHub>& WHub)
{
	MasterHub = WHub;
}

AWorkerHub* AWorkerPawn::GetWorkerMasterHub() const
{
	return MasterHub;
}

void AWorkerPawn::SetReadOnly(bool IsReadOnly)
{
	PawnInfo.bIsReadOnly = IsReadOnly;
}

bool AWorkerPawn::GetReadOnly() const
{
	return PawnInfo.bIsReadOnly;
}

void AWorkerPawn::ClearWorker()
{
	ObjectList.Empty();
	WorkerMap = nullptr;
	PawnInfo.FullTreeElementContainsInPawn.Empty();

	PawnInfo.bMemoryBroken = false;
}

void AWorkerPawn::RemoveMasterHub()
{
	MasterHub = nullptr;
}

void AWorkerPawn::SetWorkerHidden(bool bIsHidden)
{
	this->SetHidden(bIsHidden);
	for (auto& Addon : ConnectedAddon)
	{
		if (!Addon.Value)
		{
			continue;
		}
		Addon.Value->SetHidden(bIsHidden);
	}
}

bool AWorkerPawn::AddNewAddon(TObjectPtr<AWorkerAddon> NewAddon, EAddonPossitionType AType)
{
	if (!AddonSocketName.Contains(AType) || !ConnectedAddon.Contains(AType))
	{
		return false;
	}

	auto& AddonInList = ConnectedAddon[AType];

	if (AddonInList != nullptr)
	{
		return false;
	}

	AddonInList = NewAddon;
	NewAddon->ConnectAddon(PawnInfo.WorkerType, this);
	NewAddon->AttachToComponent(WorkerPawnFlipbookComponent, FAttachmentTransformRules::SnapToTargetIncludingScale, FName(*AddonSocketName.Find(AType)));
	return true;
}

bool AWorkerPawn::RemoveAddon(TObjectPtr<AWorkerAddon> NewAddon)
{
	for (auto& Addon : ConnectedAddon)
	{
		if (Addon.Value == NewAddon)
		{
			Addon.Value = nullptr;
			return true;
		}
	}
	return false;
}

const TMap<EAddonPossitionType, TObjectPtr<AWorkerAddon>>& AWorkerPawn::GetWorkerAddon() const
{
	return ConnectedAddon;
}

bool AWorkerPawn::HasAddon(EWorkerAddonType AddonType) const
{
	for (const auto& Addon : ConnectedAddon)
	{
		if (!Addon.Value)
		{
			continue;
		}
		if (Addon.Value->GetAddonType() == AddonType)
		{
			return true;
		}
	}
	return false;
}

int32 AWorkerPawn::GetPawnMemorySize() const
{
	return PawnInfo.PawnMemorySize;
}

bool AWorkerPawn::GetMemoryBroken() const
{
	return PawnInfo.bMemoryBroken;
}

void AWorkerPawn::SetMemoryBroken()
{
	PawnInfo.bMemoryBroken = true;
}

int32 AWorkerPawn::GetPawnMaxMemorySize() const
{
	return PawnInfo.PawnMaxMemorySize;
}

const TMap<EActionTreeElementName, int32>& AWorkerPawn::GetTreeElementList() const
{
	return PawnInfo.FullTreeElementContainsInPawn;
}

const TMap<EActionTreeElementName, int32>& AWorkerPawn::GetPartOfTreeElement() const
{
	return PawnInfo.PartOfTreeElement;
}

void AWorkerPawn::GiveAllAddonsToCore()
{
	for (auto& Addon : ConnectedAddon)
	{
		if (!Addon.Value)
			continue;

		if (Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->AddAddonsToCore(Addon.Value))
		{
			Addon.Value = nullptr;
		}
	}
}

const FSupportPawnInfo& AWorkerPawn::GetPawnInfo() const
{
	return PawnInfo;
}

int32 AWorkerPawn::GetWorkerMapID() const
{
	if (!WorkerMap)
		return -1;

	return WorkerMap->GetMapID();
}

int32 AWorkerPawn::GetPartOfWorkerMapID() const
{
	if (!PartOfMap)
		return -1;

	return PartOfMap->GetMapID();
}

int32 AWorkerPawn::GetPartOfMapSize() const
{
	return PartOfMapSize;
}

const TArray<TObjectPtr<UKnownObjectInfo>>& AWorkerPawn::GetObjectList() const
{
	return ObjectList;
}

const TMap<TObjectPtr<UKnownObjectInfo>, int32>& AWorkerPawn::GetPartOfObjectInfo() const
{
	return PartOfObjectInfo;
}

int32 AWorkerPawn::GetPawnID() const
{
	return PawnInfo.PawnID;
}

void AWorkerPawn::SetPawnID(int32 ID)
{
	PawnInfo.PawnID = ID;
}

void AWorkerPawn::LoadPawnInfo(FSupportPawnInfo&& Info)
{
	PawnInfo = Info;
}

void AWorkerPawn::LoadWorkerMap(TObjectPtr<UWorkerMap> FullWorkerMap, TObjectPtr<UWorkerMap> PartOfWorkerMap, int32 PartSize)
{
	WorkerMap = FullWorkerMap;
	PartOfMap = PartOfWorkerMap;
	PartOfMapSize = PartSize;
}

void AWorkerPawn::LoadKnownObjects(TArray<TObjectPtr<UKnownObjectInfo>>&& FullKnownObject, TMap<TObjectPtr<UKnownObjectInfo>, int32>&& PartOfObject)
{
	ObjectList = FullKnownObject;
	PartOfObjectInfo = PartOfObject;
}

const TObjectPtr<AWorkerHub> AWorkerPawn::GetMasterHub() const
{
	return MasterHub;
}