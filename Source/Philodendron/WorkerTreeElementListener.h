// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorkerTreeElementListener.generated.h"

enum class EActionTreeElementName;
class UWorkerTreeElementWidget;

UINTERFACE()
class UWorkerTreeElementListener : public UInterface
{
	GENERATED_BODY()
};

class IWorkerTreeElementListener
{
	GENERATED_BODY()
public:
	virtual void PressedWorkerTreeElementSelect(UWorkerTreeElementWidget* Widget, EActionTreeElementName Name) = 0;
	virtual void PressedWorkerTreeElementDiselect(UWorkerTreeElementWidget* Widget, EActionTreeElementName Name) = 0;

};
