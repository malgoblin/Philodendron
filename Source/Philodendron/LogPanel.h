// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LogLine.h"
#include "LogWordPressedListener.h"
#include "LogPanel.generated.h"


UCLASS()
class PHILODENDRON_API ULogPanel : public UUserWidget, public ILogWordPressedListener
{
	GENERATED_BODY()
public:
	ULogPanel(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	virtual void LogWordPress(const FString& Word) override;

	void InitLogPanel(const TArray<FString>* ArrayLog);
	void UpdateLogPanel();
	void AddWordToSearch(const FString& NewWord);
	void ClearWordsToSearch();

	UFUNCTION()
	void PressOnRefreshButton();

	UFUNCTION()
	void SearchTextBoxChanged(const FText& SearchText);

private:
	void CreateNewLine(const FString& Line);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UCanvasPanel* LogCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UScrollBox* LogScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UEditableTextBox* LogSearchTextBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UButton* LogRefreshButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULogLine> LogLineClass;


private:
	TArray<ULogLine*> AllLogLine;

	const TArray<FString>* CoreLog = nullptr;

	TArray<FString> WordToSearch;
	
};
