// Fill out your copyright notice in the Description page of Project Settings.


#include "LogLine.h"
#include "Components/HorizontalBox.h"
#include "LogWordPressedListener.h"


ULogLine::ULogLine(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}
#pragma optimize("", off)
void ULogLine::InitLine(const FString& NewLine, ILogWordPressedListener* NewListener)
{
	TArray<FString> WordArray;
	SplitFStringToSpace(NewLine, WordArray);

	for (const auto& Word : WordArray)
	{
		ULogWord* NewLogWord = CreateWidget<ULogWord>(GetWorld(), LogWordClass);
		NewLogWord->InitWord(Word, NewListener);
		LogLine->AddChild(NewLogWord);
		AllWordInLine.Add(NewLogWord);
	}
}
#pragma optimize("", on)

void ULogLine::SplitFStringToSpace(const FString& String, TArray<FString>& Result)
{
	FString Word;
	for (const auto& Letter : String)
	{
		if (Letter != ' ')
		{
			Word += Letter;
		}
		else
		{
			Result.Add(Word);
			Word.Empty();
		}
	}
	Result.Add(Word);
}