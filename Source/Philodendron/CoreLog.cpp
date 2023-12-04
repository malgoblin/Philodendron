// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreLog.h"

void UCoreLog::AddManyLogLines(const TArray<FString>& NewFStringArray)
{
	for (const auto& NewLine : NewFStringArray)
	{
		LogArray.Add(NewLine);
	}
}

const TArray<FString>* UCoreLog::GetCoreLog() const
{
	return &LogArray;
}
