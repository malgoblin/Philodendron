// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionTreeElement.h"
#include "WorkerAchivementStruct.h"


void UActionTreeElement::SetParents(const TArray<TObjectPtr<UActionTreeElement>>& Parents)
{
	ParentList = Parents;
}

void UActionTreeElement::SetChildren(const TArray<TObjectPtr<UActionTreeElement>>& Children)
{
	ChildrenList = Children;
}

void UActionTreeElement::SetCost(const FElementCost* ECost)
{
	Cost = ECost;
}

void UActionTreeElement::SetAlreadyKnown()
{
	bIsAlreadyKnownElement = true;
}

void UActionTreeElement::RemoveAlreadyKnown()
{
	bIsAlreadyKnownElement = false;
}

const TArray<TObjectPtr<UActionTreeElement>>& UActionTreeElement::GetParentList() const
{
	return ParentList;
}

const TArray<TObjectPtr<UActionTreeElement>>& UActionTreeElement::GetChildrenList() const
{
	return ChildrenList;
}

const FElementCost* UActionTreeElement::GetCost() const
{
	return Cost;
}

bool UActionTreeElement::IsAlreadyKnownElement() const
{
	return bIsAlreadyKnownElement;
}

const FElementReward* UActionTreeElement::GetReward() const
{
	return Reward;
}

void UActionTreeElement::SetReward(const FElementReward* EReward)
{
	Reward = EReward;
}

bool UActionTreeElement::IsConditionMet(const FElementCost& WorkerAchievements) const
{
	if (!Cost)
	{
		return true;
	}

	for (const auto& TCost : Cost->TypeCost)
	{
		if (!WorkerAchievements.TypeCost.Find(TCost.Key) || *(WorkerAchievements.TypeCost.Find(TCost.Key)) < TCost.Value)
		{
			return false;
		}
	}

	for (const auto& ACost : Cost->ActionCost)
	{
		if (!WorkerAchievements.ActionCost.Find(ACost.Key) || *(WorkerAchievements.ActionCost.Find(ACost.Key)) < ACost.Value)
		{
			return false;
		}
	}

	for (const auto& RCost : Cost->RecipeCost)
	{
		if (!WorkerAchievements.RecipeCost.Find(RCost.Key) || *(WorkerAchievements.RecipeCost.Find(RCost.Key)) < RCost.Value)
		{
			return false;
		}
	}
	return true;
}

void UActionTreeElement::SetElementName(EActionTreeElementName EName)
{
	ElementName = EName;
}

EActionTreeElementName UActionTreeElement::GetElementName() const
{
	return ElementName;
}