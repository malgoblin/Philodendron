#include "ManagerInteractableActors.h"
#include "MainGameStateBase.h"
#include "InteractableActor.h"
#include "InteractableActorChanged.h"
#include "EnumAnimationType.h"

AManagerInteractableActors::AManagerInteractableActors()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AManagerInteractableActors::BeginPlay()
{
	Super::BeginPlay();
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SetManagerInteractableActor(this);
}

void AManagerInteractableActors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const TArray<EActionType>* AManagerInteractableActors::GetAvailableActions(EInteractableActorType Type) const
{
	if (!InteractableActorParametrs.Contains(Type))
	{
		return nullptr;
	}

	return &((*InteractableActorParametrs.Find(Type)).AvailableActions);
}

int32 AManagerInteractableActors::GetSizeInfoAboutIActor(EInteractableActorType Type) const
{
	if (!InteractableActorParametrs.Contains(Type))
	{
		return -1;
	}
	return (*InteractableActorParametrs.Find(Type)).SizeInfoAboutActor;
}
#pragma optimize("", off)
const TArray<EActionType>& AManagerInteractableActors::GetKnownActionInCore() const
{
	return DefaultsKnownActionInCore;
}
#pragma optimize("", on)
float AManagerInteractableActors::GetDistanceForCreateResources(EInteractableActorType Type) const
{
	if (!InteractableActorParametrs.Contains(Type))
	{
		return -1;
	}

	return (*InteractableActorParametrs.Find(Type)).DistanceForCreateResources;
}

const FPlantAnimationStruct* AManagerInteractableActors::GetAnimationForIActor(EInteractableActorType Type, int32 ObjectNumber) const
{
	if (!InteractableActorParametrs.Contains(Type))
	{
		return nullptr;
	}

	return &((*InteractableActorParametrs.Find(Type)).InteractableActorFlipbooks[ObjectNumber]);
}

FVector3d AManagerInteractableActors::GetBoxSizeFinalFlipbook(EInteractableActorType Type, int32 ObjectNumber) const
{
	if (!InteractableActorParametrs.Contains(Type))
	{
		return FVector3d::ZeroVector;
	}

	return (*InteractableActorParametrs.Find(Type)).InteractableActorFlipbooks[ObjectNumber].Final->GetRenderBounds().GetBox().GetSize();
}

int32 AManagerInteractableActors::GenerateRandomObject(EInteractableActorType Type) const
{
	if (!InteractableActorParametrs.Contains(Type))
	{
		return -1;
	}

	return FMath::RandRange(0, (InteractableActorParametrs[Type].InteractableActorFlipbooks.Num() - 1));
}

TObjectPtr<AInteractableActor> AManagerInteractableActors::CreateInteractableActorWithoutGrowth(EInteractableActorType Type, FVector Location, int32 ObjectNumber) const
{
	TObjectPtr<AInteractableActor> NewActor = GenerateIActorWithoutGrowthObject(Type, Location);

	if (!NewActor)
	{
		return nullptr;
	}

	const FPlantAnimationStruct* FlipbookStruct = &((*InteractableActorParametrs.Find(Type)).InteractableActorFlipbooks[ObjectNumber]);

	if (!FlipbookStruct)
	{
		return nullptr;
	}

	NewActor->InitInteractableActor(Type, InteractableActorParametrs[Type].GlobalType, InteractableActorParametrs[Type].IsActorRemoveAfterEjectAllResources, FlipbookStruct, ObjectNumber);
	return NewActor;
}

TObjectPtr<AInteractableActor> AManagerInteractableActors::CreateInteractableActorWithGrowth(EInteractableActorType Type, FVector Location, int32 ObjectNumber, EInteractableActorCreatingType CreatingType) const
{
	TObjectPtr<AInteractableActorChanged> NewActor = Cast<AInteractableActorChanged>(GenerateIActorWithGrowthObject(Type, Location));
	
	if (!NewActor)
	{
		return nullptr;
	}

	const FPlantAnimationStruct* FlipbookStruct = &((*InteractableActorParametrs.Find(Type)).InteractableActorFlipbooks[ObjectNumber]);

	if (!FlipbookStruct)
	{
		return nullptr;
	}

	NewActor->InitInteractableActor(Type, InteractableActorParametrs[Type].GlobalType, InteractableActorParametrs[Type].IsActorRemoveAfterEjectAllResources, FlipbookStruct, CreatingType, ObjectNumber);
	return NewActor;
}

TObjectPtr<AInteractableActor> AManagerInteractableActors::GenerateIActorWithGrowthObject(EInteractableActorType Type, FVector Location) const
{
	FRotator Rotation = { 0, 0, 0 };
	Location.Z = 1;

	if (InteractableActorParametrs[Type].IsItHorizontalObject)
	{
		Rotation = { 0, 0, 90 };
	}
	TObjectPtr<AInteractableActorChanged> NewActor = this->GetWorld()->SpawnActor<AInteractableActorChanged>(AInteractableActorChanged::StaticClass(), Location, Rotation);

	if (!NewActor)
	{
		return nullptr;
	}

	return NewActor;
}

TObjectPtr<AInteractableActor> AManagerInteractableActors::GenerateIActorWithoutGrowthObject(EInteractableActorType Type, FVector Location) const
{
	FRotator Rotation = { 0, 0, 0 };
	Location.Z = 1;

	if (InteractableActorParametrs[Type].IsItHorizontalObject)
	{
		Rotation = { 0, 0, 90 };
	}
	TObjectPtr<AInteractableActor> NewActor = this->GetWorld()->SpawnActor<AInteractableActor>(AInteractableActor::StaticClass(), Location, Rotation);

	if (!NewActor)
	{
		return nullptr;
	}
	return NewActor;
}

TObjectPtr<AInteractableActor> AManagerInteractableActors::CreateNewInteractableActor(EInteractableActorType Type, FVector Location, int32 ObjectNumber, EInteractableActorCreatingType CreatingType) const
{
	if (!InteractableActorParametrs.Contains(Type))
	{
		return nullptr;
	}

	if ((*InteractableActorParametrs.Find(Type)).InteractableActorFlipbooks.Num() <= ObjectNumber)
	{
		return nullptr;
	}

	EInteractableActorCreatingType IACreatingType = CreatingType;
	
	if (IACreatingType == EInteractableActorCreatingType::TakeFromIAManager)
	{
		IACreatingType = InteractableActorParametrs[Type].CreatingType;
	}

	if (IACreatingType == EInteractableActorCreatingType::FinalStage)
	{
		return CreateInteractableActorWithoutGrowth(Type, Location, ObjectNumber);
	}

	return CreateInteractableActorWithGrowth(Type, Location, ObjectNumber, IACreatingType);
}

TObjectPtr<AInteractableActor> AManagerInteractableActors::GenerateIActorObject(EInteractableActorType Type, FVector Location, EInteractableActorCreatingType CreatingType) const
{
	if (CreatingType == EInteractableActorCreatingType::FinalStage)
	{
		return GenerateIActorWithoutGrowthObject(Type, Location);
	}
	return GenerateIActorWithGrowthObject(Type, Location);
}

const FString* AManagerInteractableActors::GetInteractableActorName(EInteractableActorType Type, EPossibleLanguages Language) const
{
	if (!InteractableActorParametrs.Contains(Type))
		return nullptr;

	return InteractableActorParametrs[Type].IActorName.Find(Language);
}