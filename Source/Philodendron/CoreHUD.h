// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CoreControlPanel.h"
#include "StartMenuWidget.h"
#include "MainMenuWidget.h"
#include "WorkerCreatorWidget.h"
#include "EnumPossibleLanguages.h"
#include "CoreHUD.generated.h"

class UKnownObjectInfo;
class AMainPlayer;
class AWorkerPawn;
class AWorkerHub;
class AStartGameMenu;
class IUserHUDInterface;

UCLASS()
class PHILODENDRON_API ACoreHUD : public AHUD
{
	GENERATED_BODY()
public:
	ACoreHUD();
	virtual void DrawHUD() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void OpenCloseMenu();
	void RegisterCore(const TObjectPtr<AMainPlayer>& Core);
	void SetStartGameMenu(const TObjectPtr<AStartGameMenu>& SGM, const TMap<EPossibleLanguages, FString>* AvailableLanguages);

	void AddActivityHUD(IUserHUDInterface* AHUD);


	void OpenWorkerCreator();

	void OpenCloseMainMenu();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> CoreControlPanelClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> StartMenuWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WorkerCreatorWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	void AddNewAttachedHub(const TObjectPtr<AWorkerHub>& WHub);

private:
	UPROPERTY()
	TObjectPtr<UCoreControlPanel> ControlPanel = nullptr;

	UPROPERTY()
	TObjectPtr<UStartMenuWidget> StartMenu = nullptr;

	UPROPERTY()
	TObjectPtr<UWorkerCreatorWidget> WorkerCreator = nullptr;

	UPROPERTY()
	TObjectPtr<UMainMenuWidget> MainMenu = nullptr;

	IUserHUDInterface* ActivityHUD = nullptr;
};
