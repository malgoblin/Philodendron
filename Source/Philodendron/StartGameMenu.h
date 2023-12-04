// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumPossibleLanguages.h"
#include "StartGameMenu.generated.h"

class UCameraComponent;
class ACoreHUD;
class ULevel;

UCLASS()
class PHILODENDRON_API AStartGameMenu : public AActor
{
	GENERATED_BODY()
	
public:	
	AStartGameMenu();

	bool LoadGame(EPossibleLanguages Language);
	bool LoadMainLevel(EPossibleLanguages Language);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<UCameraComponent> StartMenuCamera = nullptr;

	UPROPERTY()
	TObjectPtr<ACoreHUD> MainMenuHUD = nullptr;

	UPROPERTY(EditAnywhere)
	FName MainLevel;

	bool bFirstTick = false;
};
