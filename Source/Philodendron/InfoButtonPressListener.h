// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InfoButtonPressListener.generated.h"

class AWorkerPawn;

UINTERFACE()
class UInfoButtonPressListener : public UInterface
{
	GENERATED_BODY()
};

class IInfoButtonPressListener
{
	GENERATED_BODY()
public:
	virtual void PressedWorkerButton(AWorkerPawn* WPawn) = 0;
};