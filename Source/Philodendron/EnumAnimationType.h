// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbook.h"
#include "EnumAnimationType.generated.h"

enum class EResourceType;


UENUM()
enum class EAnimationType
{
	Invalid = 0,
	Growth,
	Final,
	Death,
};

USTRUCT()
struct FRandomRange
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 Min = 0;

	UPROPERTY(EditAnywhere)
	int32 Max = 0;
};


USTRUCT()
struct FGrowthStages
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<EResourceType, FRandomRange> NewResources;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> MainStageFlipbook = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> ChangeToNextStage = nullptr;

	UPROPERTY(EditAnywhere)
	FRandomRange TimeToSitOnStage;
};

USTRUCT()
struct FPlantAnimationStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> Final = nullptr;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> Death = nullptr;
	
	UPROPERTY(EditAnywhere)
	TMap<EResourceType, FRandomRange> FinalStageResources;

	UPROPERTY(EditAnywhere)
	TArray<FGrowthStages> Growth;
};



USTRUCT()
struct FDeviceAnimationStuct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> Idle = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> Work = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> Stop = nullptr;

};