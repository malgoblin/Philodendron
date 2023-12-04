// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapHelperToCreate.generated.h"

class UPaperTileMapComponent;
class UPaperTileMap;

UCLASS()
class PHILODENDRON_API AMapHelperToCreate : public AActor
{
	GENERATED_BODY()
	
public:	

	AMapHelperToCreate();
	virtual void Tick(float DeltaTime) override;
	void InitTile(const FVector& Possition, TObjectPtr<UPaperTileMap> TileMap, int32 NTMap);

	FVector GetTilePossition() const;
	int32 GetTileNumber() const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperTileMapComponent> TileMapComponent;

	int32 NumberTileMap = 0;
	
};
