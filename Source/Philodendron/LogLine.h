// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LogWord.h"
#include "LogLine.generated.h"

class ILogWordPressedListener;

UCLASS()
class PHILODENDRON_API ULogLine : public UUserWidget
{
	GENERATED_BODY()
public:
	ULogLine(const FObjectInitializer& ObjectInitializer);
	void InitLine(const FString& NewLine, ILogWordPressedListener* NewListener);
	static void SplitFStringToSpace(const FString& String, TArray<FString>& Result);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	class UHorizontalBox* LogLine;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULogWord> LogWordClass;
	
private:
	TArray<ULogWord*> AllWordInLine;
};
