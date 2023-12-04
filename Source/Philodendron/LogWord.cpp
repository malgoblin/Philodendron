// Fill out your copyright notice in the Description page of Project Settings.


#include "LogWord.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "LogWordPressedListener.h"


ULogWord::ULogWord(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULogWord::NativeConstruct()
{
	Super::NativeConstruct();
	LogWordButton->OnClicked.AddDynamic(this, &ULogWord::PressOnWord);
	LogWordButton->OnHovered.AddDynamic(this, &ULogWord::HoveredOnWord);
	LogWordButton->OnUnhovered.AddDynamic(this, &ULogWord::UnhoveredOnWord);

}

void ULogWord::InitWord(const FString& Text, ILogWordPressedListener* NewListener)
{
	WordText = Text;
	LogWord->SetText(FText::FromString(WordText));
	LogListener = NewListener;
}

void ULogWord::PressOnWord()
{
	LogListener->LogWordPress(WordText);
}

void ULogWord::HoveredOnWord()
{
	LogWordButton->SetColorAndOpacity(FLinearColor::Yellow);
}

void ULogWord::UnhoveredOnWord()
{
	LogWordButton->SetColorAndOpacity(FLinearColor::Green);
}