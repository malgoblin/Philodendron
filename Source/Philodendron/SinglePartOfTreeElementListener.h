// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SinglePartOfTreeElementListener.generated.h"

enum class EActionTreeElementName;
class USinglePartOfTreeElementWidget;

UINTERFACE()
class USinglePartOfTreeElementListener : public UInterface
{
	GENERATED_BODY()
};

class ISinglePartOfTreeElementListener
{
	GENERATED_BODY()
public:
	virtual void PressedSinglePartOfTreeElement(EActionTreeElementName Name, TObjectPtr<USinglePartOfTreeElementWidget> Widget) = 0;

};