// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorkerAchivementStruct.h"
#include "ManagerActionTree.generated.h"

enum class EActionTreeElementName;

USTRUCT()
struct FLinkedElements
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<EActionTreeElementName> LinkedElements;
};

UCLASS()
class PHILODENDRON_API AManagerActionTree : public AActor
{
	GENERATED_BODY()
	
public:	
	AManagerActionTree();

	const TMap<EActionTreeElementName, FElementCost>& GetActionTreeElementCosts() const;
	const TMap<EActionTreeElementName, FElementReward>& GetActionTreeElementReward() const;
	const TMap<EActionTreeElementName, FLinkedElements>& GetActionTreeElementParents() const;
	const TMap<EActionTreeElementName, FLinkedElements>& GetActionTreeElementChildren() const;

	EActionTreeElementName GetActionTreeElementNameForActionName(EActionType AType) const;
	EActionTreeElementName GetActionTreeElementNameForRecipeName(ERecipeType RType) const;

	int32 GetTreeElementSize(EActionTreeElementName TreeElementName) const;

	const FElementReward* GetRewardForTreeElement(EActionTreeElementName TreeElement) const;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	TMap<EActionTreeElementName, FElementCost> ElementCost; // cost -> desiredCounters

	UPROPERTY(EditAnywhere)
	TMap<EActionTreeElementName, FElementReward> ElementReward;

	UPROPERTY(EditAnywhere)
	TMap<EActionTreeElementName, FLinkedElements> ParentsElement;

	UPROPERTY(EditAnywhere)
	TMap<EActionTreeElementName, FLinkedElements> ChildrenElement;

};
