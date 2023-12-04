// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LogWordPressedListener.generated.h"

UINTERFACE()
class ULogWordPressedListener : public UInterface
{
	GENERATED_BODY()
};

class ILogWordPressedListener
{
	GENERATED_BODY()
public:
	virtual void LogWordPress(const FString& Word) = 0;
};
