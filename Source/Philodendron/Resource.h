// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Resource.generated.h"

enum class EResourceType;
enum class EActionType;
enum class EInteractableActorType;
class AWorkerHub;
struct FResourceParametrs;

class UPaperFlipbookComponent;
class UPaperFlipbook;

UCLASS()
class PHILODENDRON_API AResource : public AActor
{
	GENERATED_BODY()
	
public:	
	AResource();
	bool InitResource(EResourceType Type, const FResourceParametrs& ResourceParam);

	FVector GetLocation() const;
	const TObjectPtr<UPaperFlipbook> GetIActorPaperFlipbook() const;


	EResourceType GetResourceType() const;
	float GetResourceWeight() const;
	void RemoveResource();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	EResourceType ResourceType;
	
	UPROPERTY()
	float ResourceWeight = 0.0f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbookComponent> ResourceFlipbookComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> ResourceFlipbook;
};
