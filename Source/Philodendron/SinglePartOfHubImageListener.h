// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SinglePartOfHubImageListener.generated.h"

class USinglePartOfHubImageWidget;
enum class ESinglePartOfHubType;
enum class ESinglePartOfHubPossitionType;

UINTERFACE()
class USinglePartOfHubImageListener : public UInterface
{
	GENERATED_BODY()
};

class ISinglePartOfHubImageListener
{
	GENERATED_BODY()
public:
	virtual void PressedSinglePartOfHubImage(TObjectPtr<USinglePartOfHubImageWidget> Widget, ESinglePartOfHubType Type, ESinglePartOfHubPossitionType PossitionType) = 0;

};