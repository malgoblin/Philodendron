// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LogWord.generated.h"

class ILogWordPressedListener;

UCLASS()
class PHILODENDRON_API ULogWord : public UUserWidget
{
	GENERATED_BODY()
public:
	ULogWord(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	void InitWord(const FString& Text, ILogWordPressedListener* NewListener);

	UFUNCTION()
	void PressOnWord();

	UFUNCTION()
	void HoveredOnWord();

	UFUNCTION()
	void UnhoveredOnWord();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UTextBlock* LogWord;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UButton* LogWordButton;


private:
	ILogWordPressedListener* LogListener = nullptr;
	FString WordText;
};
