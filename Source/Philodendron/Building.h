// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumBuildingType.h"
#include "EnumAnimationType.h"
#include "EnumRecipeType.h"
#include "EnumResourceType.h"
#include "SupportStructsForSaveLoad.h"
#include "Building.generated.h"

class UPaperFlipbook;
class UPaperFlipbookComponent;
class UBoxComponent;

UCLASS()
class PHILODENDRON_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	ABuilding();
	virtual void Tick(float DeltaTime) override;
	void InitBuilding(EBuildingType Type, const FDeviceAnimationStuct* Flipbook, int32 DistanceForCreate);
	EBuildingType GetBuildingType() const;
	TObjectPtr<UPaperFlipbook> GetBuildingPaperFlipbook() const;

	bool IsBuildingBusy() const;

	bool MakeRecipe(ERecipeType Type);

	const FSupportBuildingInfo& GetBuildingInfo() const;

	void LoadBuildingInfo(FSupportBuildingInfo&& Info);

protected:
	virtual void BeginPlay() override;

private:
	void FinishWork();

private:
	UPROPERTY()
	FSupportBuildingInfo BuildingInfo;

	UPROPERTY()
	TObjectPtr<UPaperFlipbookComponent> BuildingFlipbookComponent;

	const FDeviceAnimationStuct* BuildingFlipbooks;

	UPROPERTY()
	TObjectPtr<UBoxComponent> BoxCollisionComponent;

};
