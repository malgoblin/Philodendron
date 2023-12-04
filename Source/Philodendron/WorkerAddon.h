// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumWorkerAddonType.h"
#include "EnumAnimationType.h"
#include "EnumWorkerType.h"
#include "WorkerAddon.generated.h"

class UPaperFlipbookComponent;
class AWorkerPawn;

UCLASS()
class PHILODENDRON_API AWorkerAddon : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorkerAddon();
	virtual void Tick(float DeltaTime) override;
	void InitAddon(EWorkerAddonType Type);

	TObjectPtr<UPaperFlipbookComponent> GetAddonFlipbookComponent();
	const TObjectPtr<AWorkerPawn>& GetAddonOwner() const;
	EWorkerAddonType GetAddonType() const;
	FString GetAddonInfo() const;

	bool ConnectAddon(EWorkerType WType, AWorkerPawn* WPawn);

	const FString& GetAddonName() const;

protected:
	virtual void BeginPlay() override;

private:
	void InitAddonName();

private:
	UPROPERTY()
	EWorkerAddonType AddonType = EWorkerAddonType::Arm;

	UPROPERTY()
	EWorkerType AddonPosition = EWorkerType::Left;

	UPROPERTY()
	FString AddonName = "Addon";

	UPROPERTY()
	TObjectPtr<AWorkerPawn> AddonOwner = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbookComponent> AddonFlipbookComponent = nullptr;

	UPROPERTY(EditAnywhere)
	FDeviceAnimationStuct AddonFlipbooks;
};
