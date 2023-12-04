// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "StartGameMenu.h"

UStartMenuWidget::UStartMenuWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UStartMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoadGame->OnClicked.AddDynamic(this, &UStartMenuWidget::PressLoadGame);
	StartMainLevel->OnClicked.AddDynamic(this, &UStartMenuWidget::LaunchMainLevel);
}

void UStartMenuWidget::PressLoadGame()
{
	for (const auto& Language : *Languages)
	{
		if (Language.Value == LanguagesList->GetSelectedOption())
		{

			if (!StartGameMenu->LoadGame(Language.Key))
				return;

			this->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
	}
}

void UStartMenuWidget::LaunchMainLevel()
{
	for (const auto& Language : *Languages)
	{
		if (Language.Value == LanguagesList->GetSelectedOption())
		{

			if (!StartGameMenu->LoadMainLevel(Language.Key))
				return;

			this->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
	}

}

void UStartMenuWidget::SetStartGameMenu(AStartGameMenu* SGM, const TMap<EPossibleLanguages, FString>* AvailableLanguages)
{
	StartGameMenu = SGM;

	if (!AvailableLanguages)
	{
		return;
	}
	Languages = AvailableLanguages;

	for (const auto& Language : *Languages)
	{
		LanguagesList->AddOption(Language.Value);
	}
	LanguagesList->SetSelectedIndex(0);
}