// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnumInteractableActorType.h"
#include "WorkerMap.generated.h"

class UKnownObjectInfo;
class AInteractableActor;

UCLASS()
class PHILODENDRON_API UWorkerMap : public UObject
{
	GENERATED_BODY()
public:
	int32 GetSizeData() const;
	const TArray<TObjectPtr<UKnownObjectInfo>>& GetMapInfo() const;
	void AddInfoListToMap(const TArray<TObjectPtr<UKnownObjectInfo>>& List);

	bool HasObject(const TWeakObjectPtr<AInteractableActor> Object)  const;

	TArray<TWeakObjectPtr<AInteractableActor>> GetKnownInteractableActor(EInteractableActorType Type) const;

	int32 GetMapID() const;
	void SetMapID(int32 ID);
private:

	UPROPERTY()
	TArray<TObjectPtr<UKnownObjectInfo>> InfoList;

	UPROPERTY()
	int32 Size = 10;

	UPROPERTY()
	int32 MapID = -1;
};
