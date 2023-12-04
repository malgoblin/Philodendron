
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumBuildingType.h"
#include "EnumRecipeType.h"
#include "EnumAnimationType.h"
#include "ManagerBuilding.generated.h"

class UPaperFlipbook;
class ABuilding;

USTRUCT()
struct FBuildingInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FDeviceAnimationStuct BuildingFlipbooks;

	UPROPERTY(EditAnywhere)
	int32 DistanceForCreatResource;
};

UCLASS()
class PHILODENDRON_API AManagerBuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	AManagerBuilding();
	virtual void Tick(float DeltaTime) override;

	TObjectPtr<ABuilding> CreateNewBuilding(EBuildingType Type, const FVector& Location) const;

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnywhere)
	TMap<EBuildingType, FBuildingInfo> BuildingParametrs;

};
