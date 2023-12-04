// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class PHILODENDRON_API UBaseLog
{
public:
	const TArray<FString>& GetLogArray() const;

	void ClearLog();

protected:
	TArray<FString> LogArray;
};
