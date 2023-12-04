// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PressedToCreatorButtonListener.generated.h"

class USelectedObjectInCreatorWidget;
enum class EButtonType;
UINTERFACE()
class UPressedToCreatorButtonListener : public UInterface
{
	GENERATED_BODY()
};

class IPressedToCreatorButtonListener
{
	GENERATED_BODY()
public:
	virtual void PressedCreatorButton(const TObjectPtr<USelectedObjectInCreatorWidget>& PressedButton, EButtonType BType) = 0;
};