// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameMenu.h"
#include "Camera/CameraComponent.h"
#include "CoreHUD.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "MainGameStateBase.h"

// Sets default values
AStartGameMenu::AStartGameMenu()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	StartMenuCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	StartMenuCamera->SetupAttachment(RootComponent);
	StartMenuCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StartMenuCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

}

void AStartGameMenu::BeginPlay()
{
	Super::BeginPlay();
	MainMenuHUD = Cast<ACoreHUD>(this->GetWorld()->GetFirstPlayerController()->GetHUD());

}


void AStartGameMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bFirstTick)
	{
		MainMenuHUD->SetStartGameMenu(this, Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetAvailableLanguages());
		bFirstTick = true;
	}
}

bool AStartGameMenu::LoadGame(EPossibleLanguages Language)
{
	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this->GetWorld(), MainLevel, true, true, LatentInfo);

	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SetGameLanguages(Language);
	return Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->LoadGame();
}

bool AStartGameMenu::LoadMainLevel(EPossibleLanguages Language)
{

	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, MainLevel, true, true, LatentInfo);

	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SetGameLanguages(Language);
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GenerateMainLevel();

	return true;
}
