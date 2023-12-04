// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerActionTree.h"
#include "MainGameStateBase.h"
#include "EnumActionTreeElementName.h"

AManagerActionTree::AManagerActionTree()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AManagerActionTree::BeginPlay()
{
	Super::BeginPlay();
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SetManagerActionTree(this);
}

void AManagerActionTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const TMap<EActionTreeElementName, FElementCost>& AManagerActionTree::GetActionTreeElementCosts() const
{
	return ElementCost;
}

const TMap<EActionTreeElementName, FElementReward>& AManagerActionTree::GetActionTreeElementReward() const
{
	return ElementReward;
}

const TMap<EActionTreeElementName, FLinkedElements>& AManagerActionTree::GetActionTreeElementParents() const
{
	return ParentsElement;
}

const TMap<EActionTreeElementName, FLinkedElements>& AManagerActionTree::GetActionTreeElementChildren() const
{
	return ChildrenElement;
}

EActionTreeElementName AManagerActionTree::GetActionTreeElementNameForActionName(EActionType AType) const
{
	for (const auto& EReward : ElementReward)
	{
		if (EReward.Value.ActionList.Contains(AType))
		{
			return EReward.Key;
		}
	}
	return EActionTreeElementName::Invalid;
}

EActionTreeElementName AManagerActionTree::GetActionTreeElementNameForRecipeName(ERecipeType RType) const
{
	for (const auto& EReward : ElementReward)
	{
		if (EReward.Value.RecipeList.Contains(RType))
		{
			return EReward.Key;
		}
	}
	return EActionTreeElementName::Invalid;
}

int32 AManagerActionTree::GetTreeElementSize(EActionTreeElementName TreeElementName) const
{
	int32 Size = 0;
	for (const auto& Action : ElementReward[TreeElementName].ActionList)
	{
		Size += Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetActionSize(Action);
	}

	for (const auto& Recipe : ElementReward[TreeElementName].RecipeList)
	{
		Size += Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetRecipeSize(Recipe);
	}
	return Size;
}

const FElementReward* AManagerActionTree::GetRewardForTreeElement(EActionTreeElementName TreeElement) const
{
	return ElementReward.Find(TreeElement);
}