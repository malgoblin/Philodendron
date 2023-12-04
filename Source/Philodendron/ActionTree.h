// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumActionList.h"
#include "EnumRecipeType.h"
#include "EnumActionTreeElementName.h"
#include "ActionTree.generated.h"

class UActionTreeElement;
struct FElementReward;
struct FElementCost;

UCLASS()
class PHILODENDRON_API UActionTree : public UObject
{
	GENERATED_BODY()
public:

	void CreateActionTree();
	TArray<EActionTreeElementName> GetNewAvailableTreeElement(UActionTreeElement& CurrentElement, const FElementCost& WorkerAchievements);
	UActionTreeElement* GetTreeRoot() const;

	int32 GetSizeTreeToTreeElementWithoutTreeElementList(EActionTreeElementName Name, const TArray<EActionTreeElementName>& SuperfloursList) const;

	void SetAlreadyKnownTreeElement(EActionTreeElementName Name);
	void RemoveAlreadyKnownTreeElement(EActionTreeElementName Name);

	TArray<EActionTreeElementName> GetAllUnknownTreeElementsListToTreeElement(EActionTreeElementName Name) const;

	TArray<EActionTreeElementName> GetAllDeletableTreeElementListToTreeElement(EActionTreeElementName Name) const;

	TArray<EActionTreeElementName> GetAllKnownTreeElementList(const TObjectPtr<UActionTreeElement>& CurrentElement) const;

	const FElementReward* GetRewardForTreeElement(EActionTreeElementName TreeElement) const;

	void RemoveTreeElementsFromList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const;
	void RemoveTreeElementsFromRemovableList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const;
	
private:
	void CheckTreeElementsAndRemoveUnvailable(TArray<EActionTreeElementName>& List, TObjectPtr<UActionTreeElement> CurrentElement, EActionTreeElementName ElementForRemove) const;
	void CreateNewTreeElement(EActionTreeElementName EName, const FElementCost* ECost);
	void RemoveElements(TArray<EActionTreeElementName>& List, TObjectPtr<UActionTreeElement> CurrentElement) const;
	void RemoveParentsFromList(TArray<EActionTreeElementName>& List, TObjectPtr<UActionTreeElement> CurrentElement) const;

	int32 TreeElementSizeWithoutTreeElementList(TObjectPtr<UActionTreeElement> CurrentElement, const TArray<EActionTreeElementName>& SuperfloursList) const;
private:
	UPROPERTY()
	TObjectPtr<UActionTreeElement> TreeRoot = nullptr;

	UPROPERTY()
	TMap<EActionTreeElementName, TObjectPtr<UActionTreeElement>> AllTreeElements;
};
