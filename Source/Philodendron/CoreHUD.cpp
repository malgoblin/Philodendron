// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreHUD.h"
#include "KnownObjectInfo.h"
#include "StartGameMenu.h"

ACoreHUD::ACoreHUD()
{
}

void ACoreHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ACoreHUD::BeginPlay()
{
	Super::BeginPlay();

	if (CoreControlPanelClass)
	{
		ControlPanel = CreateWidget<UCoreControlPanel>(GetWorld(), CoreControlPanelClass);
		if (ControlPanel)
		{
			ControlPanel->AddToViewport();
			ControlPanel->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (StartMenuWidgetClass)
	{
		StartMenu = CreateWidget<UStartMenuWidget>(GetWorld(), StartMenuWidgetClass);
		if (StartMenu)
		{
			StartMenu->AddToViewport();
		}
	}

	if (WorkerCreatorWidgetClass)
	{
		WorkerCreator = CreateWidget<UWorkerCreatorWidget>(GetWorld(), WorkerCreatorWidgetClass);
		if (WorkerCreator)
		{
			WorkerCreator->AddToViewport();
			WorkerCreator->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (MainMenuWidgetClass)
	{
		MainMenu = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenu)
		{
			MainMenu->AddToViewport();
			MainMenu->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
}

void ACoreHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACoreHUD::OpenCloseMenu()
{
	if (!ActivityHUD && ControlPanel->GetVisibility() == ESlateVisibility::Hidden)
	{
		ControlPanel->OpenMenu();
		return;
	}

	if (!ActivityHUD && ControlPanel->GetVisibility() == ESlateVisibility::Visible)
	{
		ControlPanel->CloseHUD();
		return;
	}

	ActivityHUD->CloseHUD();
	ControlPanel->OpenMenu();

	ActivityHUD = nullptr;
}

void ACoreHUD::RegisterCore(const TObjectPtr<AMainPlayer>& Core)
{
	if (ControlPanel)
		ControlPanel->InitHUD(Core, this);

	if (WorkerCreator)
		WorkerCreator->InitHUD(Core, this);

	if (MainMenu)
		MainMenu->InitHUD(Core, this);
}
void ACoreHUD::SetStartGameMenu(const TObjectPtr<AStartGameMenu>& SGM, const TMap<EPossibleLanguages, FString>* AvailableLanguages)
{
	if (StartMenu)
		StartMenu->SetStartGameMenu(SGM, AvailableLanguages);
}

void ACoreHUD::AddActivityHUD(IUserHUDInterface* AHUD)
{
	ActivityHUD = AHUD;
}

void ACoreHUD::OpenWorkerCreator()
{
	WorkerCreator->OpenCreator();
}

void ACoreHUD::AddNewAttachedHub(const TObjectPtr<AWorkerHub>& WHub)
{
	if (ControlPanel->GetVisibility() == ESlateVisibility::Visible)
	{
		ControlPanel->AddNewAttachedHub(WHub);
	}
}

void ACoreHUD::OpenCloseMainMenu()
{
	if (MainMenu->GetVisibility() == ESlateVisibility::Hidden)
	{
		MainMenu->OpenMenu();
		return;
	}
	MainMenu->CloseMenu();
}