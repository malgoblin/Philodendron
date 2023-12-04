// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseLog.h"

const TArray<FString>& UBaseLog::GetLogArray() const
{
	return LogArray;
}

void UBaseLog::ClearLog()
{
	LogArray.Empty();
}