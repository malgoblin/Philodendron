// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class AMainPlayer;
class ACoreHUD;

UCLASS()
class PHILODENDRON_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UMainMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void InitHUD(const TObjectPtr<AMainPlayer>& Core, const TObjectPtr<ACoreHUD>& CHUD);

	void OpenMenu();

	void CloseMenu();

private:
	UFUNCTION()
	void PressSaveButton();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> SaveButton;

private:
	UPROPERTY()
	TObjectPtr<AMainPlayer> MainPlayer = nullptr;

	UPROPERTY()
	TObjectPtr<ACoreHUD> CoreHUD = nullptr;
};
