// Fill out your copyright notice in the Description page of Project Settings.


#include "LogPanel.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"


ULogPanel::ULogPanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ULogPanel::NativeConstruct()
{
	Super::NativeConstruct();
	LogRefreshButton->OnClicked.AddDynamic(this, &ULogPanel::PressOnRefreshButton);
	LogSearchTextBox->OnTextChanged.AddDynamic(this, &ULogPanel::SearchTextBoxChanged);
}

void ULogPanel::InitLogPanel(const TArray<FString>* ArrayLog)
{
	CoreLog = ArrayLog;
}

void ULogPanel::UpdateLogPanel()
{
	LogScrollBox->ClearChildren();

	if (CoreLog == nullptr)
	{
		return;
	}

	if (WordToSearch.Num() == 0)
	{
		for (const auto& Line : *CoreLog)
		{
			CreateNewLine(Line);
		}
	}
	else
	{
		for (const auto& Line : *CoreLog)
		{
			bool CorrectLine = true;

			for (const auto& Word : WordToSearch)
			{
				if (!Line.Contains(Word))
				{
					CorrectLine = false;
					break;
				}
			}
			if (CorrectLine)
			{
				CreateNewLine(Line);
			}
		}
	}

}
#pragma optimize("", off)
void ULogPanel::CreateNewLine(const FString& Line)
{
	ULogLine* NewLogLine = CreateWidget<ULogLine>(GetWorld(), LogLineClass);
	NewLogLine->InitLine(Line, this);
	LogScrollBox->AddChild(NewLogLine);
	AllLogLine.Add(NewLogLine);
}
#pragma optimize("", on)
void ULogPanel::AddWordToSearch(const FString& NewWord)
{
	WordToSearch.Add(NewWord);
	UpdateLogPanel();
}

void ULogPanel::ClearWordsToSearch()
{
	WordToSearch.Empty();
	LogSearchTextBox->SetText(FText::FromString(""));
	UpdateLogPanel();
}

void ULogPanel::LogWordPress(const FString& Word)
{
	AddWordToSearch(Word);
	FString SearchBox = LogSearchTextBox->GetText().ToString();
	SearchBox = SearchBox + " " + Word;
	LogSearchTextBox->SetText(FText::FromString(SearchBox));
}

void ULogPanel::PressOnRefreshButton()
{
	ClearWordsToSearch();
}
#pragma optimize("", off)
void ULogPanel::SearchTextBoxChanged(const FText& SearchText)
{
	WordToSearch.Empty();
	TArray<FString> WordArray;
	ULogLine::SplitFStringToSpace(SearchText.ToString(), WordArray);
	for (const auto& Word : WordArray)
	{
		if (Word.Len() != 0)
		{
			WordToSearch.Add(Word);
		}
	}
	UpdateLogPanel();
}
#pragma optimize("", on)