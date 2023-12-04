// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActorChanged.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "EnumInteractableActorType.h"
#include "EnumTypeOfActionAndActor.h"
#include "EnumResourceType.h"
#include "ManagerInteractableActors.h"
#include "MainGameStateBase.h"

AInteractableActorChanged::AInteractableActorChanged()
{
}

bool AInteractableActorChanged::InitInteractableActor(EInteractableActorType Type, ETypeOfActionAndActor GlobalType, bool IsRemoveAfterEjectResources, const FPlantAnimationStruct* FlipbookStruct, EInteractableActorCreatingType CreatingType, int32 FlipbookNumber)
{
	if (!FlipbookStruct)
	{
		return false;
	}

	IAInfo.ActorType = Type;
	IAInfo.GlobalActorType = GlobalType;
	IAInfo.bIsRemoveAfterEjectAllResources = IsRemoveAfterEjectResources;
	IAInfo.FlipbookNumber = FlipbookNumber;
	FlipbooksAndResourcesLists = FlipbookStruct;
	InteractableActorFlipbookComponent->CastShadow = true;

	if (CreatingType == EInteractableActorCreatingType::FirstStage)
	{
		IAChangedInfo.CurrentFlipbookFromList = 0;
	}
	else
	{
		IAChangedInfo.CurrentFlipbookFromList = FMath::RandRange(0, FlipbooksAndResourcesLists->Growth.Num());
	}

	SetActionList();
	ChangeGrowthStage();

	return true;
}

void AInteractableActorChanged::LoadInteractableActor(FSupportInteractableActorInfo&& Info, FSupportInteractableActorChangedInfo&& ChangedInfo)
{
	IAInfo = Info;
	FlipbooksAndResourcesLists = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetAnimationForIActor(IAInfo.ActorType, IAInfo.FlipbookNumber);
	InteractableActorFlipbookComponent->CastShadow = true;
	IAChangedInfo = ChangedInfo;

	if (IAChangedInfo.CurrentFlipbookFromList == FlipbooksAndResourcesLists->Growth.Num())
	{
		InteractableActorFlipbookComponent->SetFlipbook(FlipbooksAndResourcesLists->Final);
	}
	else
	{
		InteractableActorFlipbookComponent->SetFlipbook(FlipbooksAndResourcesLists->Growth[IAChangedInfo.CurrentFlipbookFromList].MainStageFlipbook);
	}
	InteractableActorFlipbookComponent->PlayFromStart();

	FVector BoxExtent = InteractableActorFlipbookComponent->GetFlipbook()->GetSpriteAtTime(0)->GetRenderBounds().GetBox().GetSize();
	BoxExtent.Y = BoxExtent.X / 3;

	BoxCollisionComponent->InitBoxExtent(BoxExtent);

	PrimaryActorTick.bCanEverTick = false;
}

void AInteractableActorChanged::ChangeGrowthStage()
{
	if (IAChangedInfo.CurrentFlipbookFromList == FlipbooksAndResourcesLists->Growth.Num())
	{
		InteractableActorFlipbookComponent->SetFlipbook(FlipbooksAndResourcesLists->Final);
		IAChangedInfo.bIsGrowthCompleted = true;
		IAInfo.CurrentState = EAnimationType::Final;
		AddResourcesFromStage();
		InteractableActorFlipbookComponent->PlayFromStart();

		FVector BoxExtent = InteractableActorFlipbookComponent->GetFlipbook()->GetSpriteAtTime(0)->GetRenderBounds().GetBox().GetSize();
		BoxExtent.Y = BoxExtent.X / 3;

		BoxCollisionComponent->InitBoxExtent(BoxExtent);


		return;
	}

	IAChangedInfo.bIsGrowthCompleted = false;
	IAInfo.CurrentState = EAnimationType::Growth;
	InteractableActorFlipbookComponent->SetFlipbook(FlipbooksAndResourcesLists->Growth[IAChangedInfo.CurrentFlipbookFromList].MainStageFlipbook);

	IAChangedInfo.TimeForState = FMath::RandRange(FlipbooksAndResourcesLists->Growth[IAChangedInfo.CurrentFlipbookFromList].TimeToSitOnStage.Min, FlipbooksAndResourcesLists->Growth[IAChangedInfo.CurrentFlipbookFromList].TimeToSitOnStage.Max);

	AddResourcesFromStage();
	InteractableActorFlipbookComponent->PlayFromStart();

	FVector BoxExtent = InteractableActorFlipbookComponent->GetFlipbook()->GetSpriteAtTime(0)->GetRenderBounds().GetBox().GetSize();
	BoxExtent.Y = BoxExtent.X / 3;

	BoxCollisionComponent->InitBoxExtent(BoxExtent);

}

void AInteractableActorChanged::AddResourcesFromStage()
{
	IAInfo.Resources.Empty();

	TMap<EResourceType, FRandomRange> NewResourceList;

	if (IAChangedInfo.CurrentFlipbookFromList == FlipbooksAndResourcesLists->Growth.Num())
	{
		NewResourceList = FlipbooksAndResourcesLists->FinalStageResources;
	}
	else
	{
		NewResourceList = FlipbooksAndResourcesLists->Growth[IAChangedInfo.CurrentFlipbookFromList].NewResources;
	}

	for (const auto& NewResource : NewResourceList)
	{
		IAInfo.Resources.Add(NewResource.Key, FMath::RandRange(NewResource.Value.Min, NewResource.Value.Max));
	}
}

void AInteractableActorChanged::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IAChangedInfo.bIsGrowthCompleted && !IAInfo.bIsObjectBusy)
	{
		IAChangedInfo.TimerForChange += DeltaTime;
	}

	if (!IAChangedInfo.bIsGrowthCompleted && !IAChangedInfo.bIsTransitionProcess && IAChangedInfo.TimerForChange >= IAChangedInfo.TimeForState)
	{
		IAChangedInfo.TimerForChange = 0;

		InteractableActorFlipbookComponent->SetFlipbook(FlipbooksAndResourcesLists->Growth[IAChangedInfo.CurrentFlipbookFromList].ChangeToNextStage);
		IAChangedInfo.TimeForTransitionProcess = InteractableActorFlipbookComponent->GetFlipbookLength();
		InteractableActorFlipbookComponent->SetLooping(false);
		IAChangedInfo.bIsTransitionProcess = true;
		return;
	}
	if (!IAChangedInfo.bIsGrowthCompleted && IAChangedInfo.bIsTransitionProcess && IAChangedInfo.TimerForChange >= IAChangedInfo.TimeForTransitionProcess)
	{
		InteractableActorFlipbookComponent->SetLooping(true);
		IAChangedInfo.TimeForTransitionProcess = 0.f;
		IAChangedInfo.bIsTransitionProcess = false;
		IAChangedInfo.TimerForChange = 0;
		IAChangedInfo.CurrentFlipbookFromList += 1;

		ChangeGrowthStage();
	}
}

const FSupportInteractableActorChangedInfo& AInteractableActorChanged::GetInteractableActorChangedInfo() const
{
	return IAChangedInfo;
}