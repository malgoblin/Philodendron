// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "InteractableActorChanged.generated.h"

class UPaperFlipbook;
enum class EInteractableActorCreatingType;

struct FPlantAnimationStruct;
struct FGrowthStages;

UCLASS()
class PHILODENDRON_API AInteractableActorChanged : public AInteractableActor
{
	GENERATED_BODY()
public:
	AInteractableActorChanged();
	bool InitInteractableActor(EInteractableActorType Type, ETypeOfActionAndActor GlobalType, bool IsRemoveAfterEjectResources, const FPlantAnimationStruct* FlipbookStruct, EInteractableActorCreatingType CreatingType, int32 FlipbookNumber);
	virtual void Tick(float DeltaTime) override;

	const FSupportInteractableActorChangedInfo& GetInteractableActorChangedInfo() const;

	void LoadInteractableActor(FSupportInteractableActorInfo&& Info, FSupportInteractableActorChangedInfo&& ChangedInfo);

private:
	void ChangeGrowthStage();
	void AddResourcesFromStage();

private:

	UPROPERTY()
	FSupportInteractableActorChangedInfo IAChangedInfo;
};
