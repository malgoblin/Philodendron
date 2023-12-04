// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectManager.h"
#include "PaperFlipbook.h"
#include "MainGameStateBase.h"
#include "Engine/TextureRenderTarget2D.h"


AObjectManager::AObjectManager()
{
	PrimaryActorTick.bCanEverTick = false;


}

void AObjectManager::BeginPlay()
{
	Super::BeginPlay();
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SetObjectManager(this);
}


void AObjectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const TObjectPtr<UPaperFlipbook>& AObjectManager::GetWorkerPawnFlipbook(EWorkerType WType) const
{
	return WorkerPawns[WType];
}


int32 AObjectManager::GetMinimalFoVCamera() const
{
	return MinimalFoVCamera;
}

int32 AObjectManager::GetMaxFoVCamera() const
{
	return MaxFoVCamera;
}

const TObjectPtr<UPaperFlipbook>& AObjectManager::GetCoreFlipbook() const
{
	return Core;
}

TObjectPtr<UTextureRenderTarget2D> AObjectManager::GetInterfaceCam() const
{
	return InterfaceCam;
}

TObjectPtr<UPaperFlipbook> AObjectManager::GetWorkerHubFlipbook()
{
	return WorkerHub;
}

int32 AObjectManager::GetWorkerPawnMemorySize() const
{
	return WorkerPawnMemorySize;
}

float AObjectManager::GetDefaultWorkerSpeed() const
{
	return DefaultWorkerSpeed;
}

float AObjectManager::GetDefaultWorkerSpottingRadius() const
{
	return DefaultWorkerSpottingRadius;
}

float AObjectManager::GetWorkerRestTime() const
{
	return WorkerRestTime;
}

const TMap<EPossibleLanguages, FString>* AObjectManager::GetAvailableLanguages() const
{
	return &AvailableLanguagesWithName;
}

const TObjectPtr<UTexture2D>& AObjectManager::GetWorkerPawnIcon() const
{
	return WorkerPawnIcon;
}

const TObjectPtr<UTexture2D>& AObjectManager::GetWorkerPawnImage() const
{
	return WorkerPawnImage;
}

const TObjectPtr<UTexture2D>& AObjectManager::GetWorkerHubIcon() const
{
	return WorkerHubIcon;
}

const TObjectPtr<UTexture2D>& AObjectManager::GetWorkerHubImage() const
{
	return WorkerHubImage;
}

float AObjectManager::GetSunWorkRadius() const
{
	return WorkRadius;
}

float AObjectManager::GetSunIntensity() const
{
	return Intensity;
}

const FVector& AObjectManager::GetSunPossitionRelativelyMapEdge() const
{
	return PossitionRelativelyMapEdge;
}

const bool AObjectManager::HasUseTemperature() const
{
	return bTemperature;
}

const float AObjectManager::GetTemperature() const
{
	return Temperature;
}

const float AObjectManager::GetDirectionalIntensity() const
{
	return DirectionalIntensity;
}

const float AObjectManager::GetSourceRadius() const
{
	return SourceRadius;
}