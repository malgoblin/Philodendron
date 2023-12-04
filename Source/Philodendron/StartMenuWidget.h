// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnumPossibleLanguages.h"
#include "StartMenuWidget.generated.h"

class AStartGameMenu;
class UButton;
class UComboBoxString;

UCLASS()
class PHILODENDRON_API UStartMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UStartMenuWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void SetStartGameMenu(AStartGameMenu* SGM, const TMap<EPossibleLanguages, FString>* AvailableLanguages);
private:
	UFUNCTION()
	void PressLoadGame();
	UFUNCTION()
	void LaunchMainLevel();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> LoadGame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> StartMainLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UComboBoxString> LanguagesList;

private:
	UPROPERTY()
	TObjectPtr<AStartGameMenu> StartGameMenu = nullptr;

	const TMap<EPossibleLanguages, FString>* Languages = nullptr;
};
