// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ManagerResources.generated.h"

enum class EResourceType;
enum class ESubResourceType;
class AResource;
class UPaperFlipbook;

USTRUCT()
struct FResourceParametrs
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> ResourceFlipbook;
	
	UPROPERTY(EditAnywhere)
	float ResourceWeight;

	UPROPERTY(EditAnywhere)
	TArray<ESubResourceType> SubType;

};

UCLASS()
class PHILODENDRON_API AManagerResources : public AActor
{
	GENERATED_BODY()
	
public:	
	AManagerResources();
	virtual void Tick(float DeltaTime) override;

	TObjectPtr<AResource> CreateNewResource(EResourceType ResourceType, const FVector& Location, const FRotator& Rotation);

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
	TMap<EResourceType, FResourceParametrs> ResourcesParametrs;

};
