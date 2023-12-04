
#include "InteractableActor.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "WorkerHub.h"
#include "EnumInteractableActorType.h"
#include "EnumActionList.h"
#include "ObjectManager.h"
#include "NavAreas/NavArea_Null.h"

#include "MainGameStateBase.h"
#include "ManagerInteractableActors.h"

AInteractableActor::AInteractableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	this->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	InteractableActorFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("InteractableActorFlipbookComponent"));

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractableActorCollision"));

	InteractableActorFlipbookComponent->SetupAttachment(RootComponent);

	BoxCollisionComponent->SetupAttachment(RootComponent);


	BoxCollisionComponent->bDynamicObstacle = true;
	BoxCollisionComponent->AreaClass = UNavArea_Null::StaticClass();

}

bool AInteractableActor::InitInteractableActor(EInteractableActorType Type, ETypeOfActionAndActor GlobalType, bool IsRemoveAfterEjectResources, const FPlantAnimationStruct* FlipbookStruct, int32 FlipbookNumber)
{
	if (!FlipbookStruct)
	{
		return false;
	}

	IAInfo.ActorType = Type;
	IAInfo.GlobalActorType = GlobalType;
	IAInfo.FlipbookNumber = FlipbookNumber;
	IAInfo.bIsRemoveAfterEjectAllResources = IsRemoveAfterEjectResources;
	FlipbooksAndResourcesLists = FlipbookStruct;
	InteractableActorFlipbookComponent->CastShadow = true;

	IAInfo.CurrentState = EAnimationType::Final;

	InteractableActorFlipbookComponent->SetFlipbook(FlipbooksAndResourcesLists->Final);
	AddResources();
	SetActionList();

	/*
	FVector BoxExtent = { InteractableActorFlipbookComponent->GetFlipbook()->GetSpriteAtTime(0)->GetSourceSize().X
		, InteractableActorFlipbookComponent->GetFlipbook()->GetSpriteAtTime(0)->GetSourceSize().X / 3
		, InteractableActorFlipbookComponent->GetFlipbook()->GetSpriteAtTime(0)->GetSourceSize().Y };
	*/
	FVector BoxExtent = InteractableActorFlipbookComponent->GetFlipbook()->GetSpriteAtTime(0)->GetRenderBounds().GetBox().GetSize();
	BoxExtent.Y = BoxExtent.X / 3;

	BoxCollisionComponent->InitBoxExtent(BoxExtent);

	PrimaryActorTick.bCanEverTick = false;

	return true;
}

void AInteractableActor::LoadInteractableActor(FSupportInteractableActorInfo&& Info)
{
	IAInfo = Info;
	FlipbooksAndResourcesLists = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetAnimationForIActor(IAInfo.ActorType, IAInfo.FlipbookNumber);
	InteractableActorFlipbookComponent->CastShadow = true;

	InteractableActorFlipbookComponent->SetFlipbook(FlipbooksAndResourcesLists->Final);

	FVector BoxExtent = InteractableActorFlipbookComponent->GetFlipbook()->GetSpriteAtTime(0)->GetRenderBounds().GetBox().GetSize();
	BoxExtent.Y = BoxExtent.X / 3;

	BoxCollisionComponent->InitBoxExtent(BoxExtent);

	PrimaryActorTick.bCanEverTick = false;
}

void AInteractableActor::AddResources()
{
	for (const auto& NewResource : FlipbooksAndResourcesLists->FinalStageResources)
	{
		IAInfo.Resources.Add(NewResource.Key, FMath::RandRange(NewResource.Value.Min, NewResource.Value.Max));
	}
}

void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractableActor::SetActionList()
{
	const TArray<EActionType>* TempActionList = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetAvailableActions(IAInfo.ActorType);

	if (TempActionList)
	{
		IAInfo.ActionList = *TempActionList;
	}
}

void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const TArray<EActionType>& AInteractableActor::GetActionList() const
{
	return IAInfo.ActionList;
}


FVector AInteractableActor::GetLocation() const
{
	return this->GetActorLocation();
}

EInteractableActorType AInteractableActor::GetActorType() const
{
	return IAInfo.ActorType;
}
const ETypeOfActionAndActor AInteractableActor::GetActorEvolution() const
{
	return IAInfo.GlobalActorType;
}

bool AInteractableActor::RemoveInteractableActor()
{
	this->SetActorHiddenInGame(true);
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->RemoveExistedActor(this);
	return true;
}

bool AInteractableActor::CreateNewResource(TObjectPtr<AWorkerHub> WHub)
{
	if (!LockedResources.Contains(WHub))
	{
		return false;
	}

	EResourceType CreatedResource = *LockedResources.Find(WHub);

	if (Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->CreateNewResource(CreatedResource, GetLocation(), 
		(Cast<AMainGameStateBase>(this->GetWorld()->GetGameState()))->GetDistanceForCreateResources(IAInfo.ActorType)))
	{
		LockedResources.Remove(WHub);

		if (IAInfo.Resources.Num() == 0 && LockedResources.Num() == 0 && IAInfo.bIsRemoveAfterEjectAllResources)
		{
			RemoveInteractableActor();
		}

		return true;
	}

	FreeResourceLockedHub(WHub);

	return false;
}

bool AInteractableActor::ExtractAllResources(const TObjectPtr<AWorkerHub>& WHub)
{
	if (!IAInfo.bIsObjectBusy)
		return false;

	if (WorkerHubLockActor != WHub)
		return false;

	WorkerHubLockActor = nullptr;

	for (const auto& Resource : IAInfo.Resources)
	{
		int32 ResourceCount = Resource.Value;
		for (int32 i = 0; i < ResourceCount; ++i)
		{
			Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->CreateNewResource(Resource.Key, GetLocation(), 
				(Cast<AMainGameStateBase>(this->GetWorld()->GetGameState()))->GetDistanceForCreateResources(IAInfo.ActorType));
		}
	}

	if (IAInfo.bIsRemoveAfterEjectAllResources)
	{
		RemoveInteractableActor();
	}
	return true;

}

bool AInteractableActor::LockActor(const TObjectPtr<AWorkerHub>& WHub)
{
	if (IAInfo.bIsObjectBusy)
		return false;

	IAInfo.bIsObjectBusy = true;

	WorkerHubLockActor = WHub;

	return true;
}

bool AInteractableActor::IsWorkerHubLocked(const TObjectPtr<AWorkerHub>& WHub) const
{
	return WorkerHubLockActor == WHub;
}

bool AInteractableActor::LockResource(EResourceType ResourceForLock, TObjectPtr<AWorkerHub> WHub)
{
	if (IAInfo.bIsObjectBusy)
	{
		return false;
	}

	if (!IAInfo.Resources.Contains(ResourceForLock))
	{
		return false;
	}

	int32* NeededResource = IAInfo.Resources.Find(ResourceForLock);

	LockedResources.Add(WHub, ResourceForLock);

	*NeededResource -= 1;
	if (*NeededResource == 0)
	{
		IAInfo.Resources.Remove(ResourceForLock);
	}
	IAInfo.bIsObjectBusy = true;
	return true;
}

void AInteractableActor::FreeResourceLockedHub(TObjectPtr<AWorkerHub> WHub)
{
	if (LockedResources.Contains(WHub))
	{
		return;
	}

	EResourceType LockedResource = *(LockedResources.Find(WHub));

	int32* ExistedResource = IAInfo.Resources.Find(LockedResource);
	if (ExistedResource)
	{
		*ExistedResource += 1;
	}
	else
	{
		IAInfo.Resources.Add(LockedResource, 1);
	}
	LockedResources.Remove(WHub);

	if (LockedResources.Num() == 0)
	{
		IAInfo.bIsObjectBusy = false;
	}
}

TObjectPtr<UPaperFlipbook> AInteractableActor::GetIActorPaperFlipbook() const
{
	return InteractableActorFlipbookComponent->GetFlipbook();
}

bool AInteractableActor::HasAction(EActionType Type) const
{
	return IAInfo.ActionList.Contains(Type);
}

const TMap<EResourceType, int32>* AInteractableActor::GetAvailableResourceList() const
{
	return &IAInfo.Resources;
}

const FSupportInteractableActorInfo& AInteractableActor::GetInteractableActorInfo() const
{
	return IAInfo;
}

const TMap<TWeakObjectPtr<AWorkerHub>, EResourceType>& AInteractableActor::GetLockedResources() const
{
	return LockedResources;
}

const TWeakObjectPtr<AWorkerHub> AInteractableActor::GetWorkerHubLockActor() const
{
	return WorkerHubLockActor;
}