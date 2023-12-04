// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "MainPlayer.h"
#include "CoreHUD.h"
#include "MainGameStateBase.h"

UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainMenuWidget::InitHUD(const TObjectPtr<AMainPlayer>& Core, const TObjectPtr<ACoreHUD>& CHUD)
{
	MainPlayer = Core;
	CoreHUD = CHUD;

	SaveButton->OnClicked.AddDynamic(this, &UMainMenuWidget::PressSaveButton);
}

void UMainMenuWidget::OpenMenu()
{
	this->SetVisibility(ESlateVisibility::Visible);
	MainPlayer->PauseGame();
	MainPlayer->TurnOffControls();
	MainPlayer->SetMainMenuStatus(true);
}

void UMainMenuWidget::CloseMenu()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	MainPlayer->ContinueGame();
	MainPlayer->TurnOnControls();
	MainPlayer->SetMainMenuStatus(false);

}

void UMainMenuWidget::PressSaveButton()
{
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SaveGame();
}