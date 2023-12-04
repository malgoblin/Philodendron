// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SupportStructsForSaveLoad.h"
#include "KnownObjectInfo.generated.h"

enum class EInteractableActorType;
enum class EActionType;
class AInteractableActor;

UCLASS()
class PHILODENDRON_API UKnownObjectInfo : public UObject
{
	GENERATED_BODY()
public:
	UKnownObjectInfo();
	void SetInfo(FSupportKnownObjectInfo&& Info);
	void SetWeakObjectPtr(TWeakObjectPtr<AInteractableActor> Actor);

	const TWeakObjectPtr<AInteractableActor> GetInteractableActor() const;

	bool HasAction(EActionType Type) const;

	const FVector& GetKnownObjectLocation() const;
	EInteractableActorType GetActorType() const;
	int32 GetSizeData() const;

	const FSupportKnownObjectInfo& GetKnownObjectInfo() const;

private:
	UPROPERTY()
	FSupportKnownObjectInfo KnownInfo;

	UPROPERTY()
	TWeakObjectPtr<AInteractableActor> IActor;
};
