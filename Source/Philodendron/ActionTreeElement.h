// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorkerAchivementStruct.h"
#include "ActionTreeElement.generated.h"


enum class EActionTreeElementName;

UCLASS()
class PHILODENDRON_API UActionTreeElement : public UObject
{
	GENERATED_BODY()
public:
	void SetParents(const TArray<TObjectPtr<UActionTreeElement>>& Parents);
	void SetChildren(const TArray<TObjectPtr<UActionTreeElement>>& Children);

	void SetCost(const FElementCost* ECost);
	void SetAlreadyKnown();
	void RemoveAlreadyKnown();
	void SetReward(const FElementReward* EReward);

	const FElementReward* GetReward() const;
	const FElementCost* GetCost() const;
	const TArray<TObjectPtr<UActionTreeElement>>& GetParentList() const;
	const TArray<TObjectPtr<UActionTreeElement>>& GetChildrenList() const;

	bool IsAlreadyKnownElement() const;
	bool IsConditionMet(const FElementCost& WorkerAchievements) const;

	void SetElementName(EActionTreeElementName EName);
	EActionTreeElementName GetElementName() const;

private:
	const FElementReward* Reward;
	
	const FElementCost* Cost;

	UPROPERTY()
	TArray<TObjectPtr<UActionTreeElement>> ParentList;
	
	UPROPERTY()
	TArray<TObjectPtr<UActionTreeElement>> ChildrenList;

	UPROPERTY()
	bool bIsAlreadyKnownElement = false;

	UPROPERTY()
	EActionTreeElementName ElementName;
};
