// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KnownObjectInfo.h"
#include "DataStorage.generated.h"

class IWorkerData;

UCLASS()
class PHILODENDRON_API UDataStorage : public UObject
{
	GENERATED_BODY()
public:
	void SetStorageSize(int32 Size);
	bool TryToAddNewData(int32 DataSize);
	void AddData(int32 DataSize);
	void RemoveData(int32 DataSize);
	void AddStorageSize(int32 Size);
	void ResetStorage();

private:
	UPROPERTY()
	int32 AvailableMemory = 0;

	UPROPERTY()
	int32 MaxSize = 0;
};
