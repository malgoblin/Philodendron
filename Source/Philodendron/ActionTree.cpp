// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionTree.h"
#include "EnumResourceType.h"
#include "MainGameStateBase.h"
#include "ActionTreeElement.h"
#include "EnumActionTreeElementName.h"

#pragma optimize("", off)
void UActionTree::CreateActionTree()
{
	TObjectPtr<AMainGameStateBase> MainGS = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState());

	if (!MainGS)
		return;

	for (const auto& TreeElement : MainGS->GetActionTreeElementCosts())
	{
		CreateNewTreeElement(TreeElement.Key, &(TreeElement.Value));
	}

	for (const auto& TreeElement : MainGS->GetActionTreeElementReward())
	{
		if (!AllTreeElements.Contains(TreeElement.Key))
			continue;

		AllTreeElements[TreeElement.Key]->SetReward(&(TreeElement.Value));
	}

	for (const auto& TreeElement : MainGS->GetActionTreeElementParents())
	{
		if (!AllTreeElements.Contains(TreeElement.Key))
			continue;

		TArray<TObjectPtr<UActionTreeElement>> ParentList;
		for (const auto& Parent : TreeElement.Value.LinkedElements)
		{
			for (const auto& Element : AllTreeElements)
			{
				if (Element.Key == Parent)
				{
					ParentList.Add(Element.Value);
					break;
				}
			}
		}
		AllTreeElements[TreeElement.Key]->SetParents(ParentList);
	}

	for (const auto& TreeElement : MainGS->GetActionTreeElementChildren())
	{
		if (!AllTreeElements.Contains(TreeElement.Key))
			continue;

		TArray<TObjectPtr<UActionTreeElement>> ChildrenList;
		for (const auto& Child : TreeElement.Value.LinkedElements)
		{
			for (const auto& Element : AllTreeElements)
			{
				if (Element.Key == Child)
				{
					ChildrenList.Add(Element.Value);
					break;
				}
			}
		}
		AllTreeElements[TreeElement.Key]->SetChildren(ChildrenList);
	}
}
#pragma optimize("", on)
void UActionTree::CreateNewTreeElement(EActionTreeElementName EName, const FElementCost* ECost)
{
	TObjectPtr<UActionTreeElement> NewElement = NewObject<UActionTreeElement>(this);
	NewElement->SetCost(ECost);
	NewElement->SetElementName(EName);
	AllTreeElements.Add(EName, NewElement);

	if (!TreeRoot && EName == EActionTreeElementName::Root)
	{
		TreeRoot = NewElement;
	}
}

TArray<EActionTreeElementName> UActionTree::GetNewAvailableTreeElement(UActionTreeElement& CurrentElement, const FElementCost& WorkerAchievements)
{
	TArray<EActionTreeElementName> ElementTreeList;

	if (!CurrentElement.IsConditionMet(WorkerAchievements) && !CurrentElement.IsAlreadyKnownElement())
		return ElementTreeList;

	if (!CurrentElement.IsAlreadyKnownElement())
	{
		if (!ElementTreeList.Contains(CurrentElement.GetElementName()))
		{
			ElementTreeList.Add(CurrentElement.GetElementName());
		}
		return ElementTreeList;
	}

	for (const auto& Child : CurrentElement.GetChildrenList())
	{
		for (const auto& Reward : GetNewAvailableTreeElement(*Child, WorkerAchievements))
		{
			if (!ElementTreeList.Contains(Reward))
			{
				ElementTreeList.Add(Reward);
			}
		}
	}

	return ElementTreeList;
}

UActionTreeElement* UActionTree::GetTreeRoot() const
{
	return TreeRoot;
}

int32 UActionTree::GetSizeTreeToTreeElementWithoutTreeElementList(EActionTreeElementName Name, const TArray<EActionTreeElementName>& SuperfloursList) const
{
	return TreeElementSizeWithoutTreeElementList(AllTreeElements[Name], SuperfloursList);
}

int32 UActionTree::TreeElementSizeWithoutTreeElementList(TObjectPtr<UActionTreeElement> CurrentElement, const TArray<EActionTreeElementName>& SuperfloursList) const
{
	if (SuperfloursList.Contains(CurrentElement->GetElementName()))
	{
		return 0;
	}

	int32 SizeCurrentElement = 0;

	for (const auto& Action : CurrentElement->GetReward()->ActionList)
	{
		SizeCurrentElement += Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetActionSize(Action);
	}

	for (const auto& Recipe : CurrentElement->GetReward()->RecipeList)
	{
		SizeCurrentElement += Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetRecipeSize(Recipe);
	}

	for (const auto& Parent : CurrentElement->GetParentList())
	{
		SizeCurrentElement += TreeElementSizeWithoutTreeElementList(Parent, SuperfloursList);
	}

	return SizeCurrentElement;
}

void UActionTree::SetAlreadyKnownTreeElement(EActionTreeElementName Name)
{
	AllTreeElements[Name]->SetAlreadyKnown();
}

TArray<EActionTreeElementName> UActionTree::GetAllUnknownTreeElementsListToTreeElement(EActionTreeElementName Name) const
{
	TArray<EActionTreeElementName> TreeElementList;

	if (AllTreeElements[Name]->IsAlreadyKnownElement())
	{
		return TreeElementList;
	}

	TreeElementList.Add(Name);

	for (const auto& Parent : AllTreeElements[Name]->GetParentList())
	{
		for (const auto& TreeElement : GetAllUnknownTreeElementsListToTreeElement(Parent->GetElementName()))
		{
			TreeElementList.Add(TreeElement);
		}
	}
	return TreeElementList;

}

TArray<EActionTreeElementName> UActionTree::GetAllDeletableTreeElementListToTreeElement(EActionTreeElementName Name) const
{
	TArray<EActionTreeElementName> TreeElementList;

	if (!AllTreeElements[Name]->IsAlreadyKnownElement())
	{
		return TreeElementList;
	}

	TreeElementList.Add(Name);

	for (const auto& Child : AllTreeElements[Name]->GetChildrenList())
	{
		for (const auto& TreeElement : GetAllDeletableTreeElementListToTreeElement(Child->GetElementName()))
		{
			TreeElementList.Add(TreeElement);
		}
	}
	return TreeElementList;

}

TArray<EActionTreeElementName> UActionTree::GetAllKnownTreeElementList(const TObjectPtr<UActionTreeElement>& CurrentElement) const
{
	TArray<EActionTreeElementName> TreeElementList;

	if (!CurrentElement->IsAlreadyKnownElement())
	{
		return TreeElementList;
	}

	TreeElementList.Add(CurrentElement->GetElementName());

	for (const auto& Child : CurrentElement->GetChildrenList())
	{
		for (const auto& TreeElement : GetAllKnownTreeElementList(Child))
		{
			TreeElementList.Add(TreeElement);
		}
	}
	return TreeElementList;
}

const FElementReward* UActionTree::GetRewardForTreeElement(EActionTreeElementName TreeElement) const
{
	return AllTreeElements[TreeElement]->GetReward();
}

void UActionTree::RemoveAlreadyKnownTreeElement(EActionTreeElementName Name)
{
	AllTreeElements[Name]->RemoveAlreadyKnown();
}

void UActionTree::RemoveTreeElementsFromList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const
{
	CheckTreeElementsAndRemoveUnvailable(List, TreeRoot, ElementForRemove);
}

void UActionTree::RemoveTreeElementsFromRemovableList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const
{
	RemoveParentsFromList(List, AllTreeElements[ElementForRemove]);
}

void UActionTree::CheckTreeElementsAndRemoveUnvailable(TArray<EActionTreeElementName>& List, TObjectPtr<UActionTreeElement> CurrentElement, EActionTreeElementName ElementForRemove) const
{
	if (CurrentElement->GetElementName() == ElementForRemove)
	{
		RemoveElements(List, CurrentElement);
		return;
	}

	for (const auto& Element : CurrentElement->GetChildrenList())
	{
		CheckTreeElementsAndRemoveUnvailable(List, Element, ElementForRemove);
	}
}

void UActionTree::RemoveElements(TArray<EActionTreeElementName>& List, TObjectPtr<UActionTreeElement> CurrentElement) const
{
	List.Remove(CurrentElement->GetElementName());

	for (const auto& Element : CurrentElement->GetChildrenList())
	{
		RemoveElements(List, Element);
	}
}

void UActionTree::RemoveParentsFromList(TArray<EActionTreeElementName>& List, TObjectPtr<UActionTreeElement> CurrentElement) const
{
	List.Remove(CurrentElement->GetElementName());

	for (const auto& TreeElement : CurrentElement->GetParentList())
	{
		RemoveParentsFromList(List, TreeElement);
	}
}