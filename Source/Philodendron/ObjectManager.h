// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumInteractableActorType.h"
#include "EnumResourceType.h"
#include "EnumWorkerType.h"
#include "EnumWorkerAddonType.h"
#include "PaperTileMap.h"
#include "Engine/Level.h"
#include "EnumAnimationType.h"
#include "EnumPossibleLanguages.h"
#include "ObjectManager.generated.h"

class UPaperFlipbook;
class UTextureRenderTarget2D;



UCLASS()
class PHILODENDRON_API AObjectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AObjectManager();
	const TObjectPtr<UPaperFlipbook>& GetWorkerPawnFlipbook(EWorkerType WType) const;

	const TObjectPtr<UPaperFlipbook>& GetCoreFlipbook() const;
	TObjectPtr<UTextureRenderTarget2D> GetInterfaceCam() const;

	TObjectPtr<UPaperFlipbook> GetWorkerHubFlipbook();

	int32 GetWorkerPawnMemorySize() const;

	float GetDefaultWorkerSpeed() const;
	float GetDefaultWorkerSpottingRadius() const;
	float GetWorkerRestTime() const;

	int32 GetMinimalFoVCamera() const;
	int32 GetMaxFoVCamera() const;

	const TMap<EPossibleLanguages, FString>* GetAvailableLanguages() const;

	float GetSunWorkRadius() const;
	float GetSunIntensity() const;
	const FVector& GetSunPossitionRelativelyMapEdge() const;

	const float GetSourceRadius() const;
	const bool HasUseTemperature() const;
	const float GetTemperature() const;
	const float GetDirectionalIntensity() const;

	const TObjectPtr<UTexture2D>& GetWorkerPawnIcon() const;
	const TObjectPtr<UTexture2D>& GetWorkerPawnImage() const;
	const TObjectPtr<UTexture2D>& GetWorkerHubIcon() const;
	const TObjectPtr<UTexture2D>& GetWorkerHubImage() const;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TMap<EPossibleLanguages, FString> AvailableLanguagesWithName;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> Core = nullptr;

	UPROPERTY(EditAnywhere)
	int32 MinimalFoVCamera = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxFoVCamera = 0;

	UPROPERTY(EditAnywhere, Category = "Sun")
	float Intensity = 0.f;

	UPROPERTY(EditAnywhere, Category = "Sun")
	float WorkRadius = 0.f;

	UPROPERTY(EditAnywhere, Category = "Sun")
	FVector PossitionRelativelyMapEdge;

	UPROPERTY(EditAnywhere, Category = "Sun")
	float SourceRadius = 350.f;

	UPROPERTY(EditAnywhere, Category = "Sun")
	bool bTemperature = true;

	UPROPERTY(EditAnywhere, Category = "Sun")
	float Temperature = 12000;

	UPROPERTY(EditAnywhere, Category = "Sun")
	float DirectionalIntensity = 0.5;

	UPROPERTY(EditAnywhere)
	TMap<EInteractableActorType, FDeviceAnimationStuct> Devices;

	UPROPERTY(EditAnywhere)
	TMap<EWorkerType, TObjectPtr<UPaperFlipbook>> WorkerPawns;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> WorkerPawnIcon = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> WorkerPawnImage = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> WorkerHub = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> WorkerHubIcon = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> WorkerHubImage = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTextureRenderTarget2D> InterfaceCam = nullptr;

	UPROPERTY(EditAnywhere)
	int32 WorkerPawnMemorySize = 0;

	UPROPERTY(EditAnywhere)
	float DefaultWorkerSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	float DefaultWorkerSpottingRadius = 0.f;

	UPROPERTY(EditAnywhere)
	float WorkerRestTime = 0.f;
};
