// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumWorkerAddonType.h"
#include "EnumAnimationType.h"
#include "EnumWorkerType.h"
#include "EnumPossibleLanguages.h"
#include "ManagerAddons.generated.h"

USTRUCT()
struct FAddonInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<EPossibleLanguages, FString> AddonName;

	UPROPERTY(EditAnywhere)
	TMap<EWorkerType, FDeviceAnimationStuct> AddonFlipbooks;

	UPROPERTY(EditAnywhere)
	TMap<EAddonModifiers, float> AddonModifiers;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture> AddonIcon;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture> AddonImage;
};

UCLASS()
class PHILODENDRON_API AManagerAddons : public AActor
{
	GENERATED_BODY()
	
public:	
	AManagerAddons();
	virtual void Tick(float DeltaTime) override;
	const FDeviceAnimationStuct* GetAddonFlipbooks(EWorkerAddonType AddonType, EWorkerType WType) const;

	const TMap<EAddonModifiers, float>* GetAddonModifiers(EWorkerAddonType Type) const;

	const FString* GetAddonName(EWorkerAddonType Type, EPossibleLanguages Language) const;

	const TObjectPtr<UTexture> GetAddonIcon(EWorkerAddonType Type) const;
	const TObjectPtr<UTexture> GetAddonImage(EWorkerAddonType Type) const;

protected:
	virtual void BeginPlay() override;


public:
	UPROPERTY(EditAnywhere)
	TMap<EWorkerAddonType, FAddonInfo> AddonList;
};
