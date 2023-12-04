// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumTypeOfActionAndActor.h"
#include "EnumActionList.h"
#include "WorkerTrait.generated.h"

USTRUCT()
struct PHILODENDRON_API FWorkerTrait
{
	GENERATED_BODY()
public:
	void ChangePriorityAfterDoAction(EActionType Type, float Delta);
	void ChangePriorityAfterDoAnotherAction(EActionType Type, float Delta);

	float GetDeltaPriorityOnActionDone(EActionType Type);
	float GetDeltaPriorityOnOtherActionDone(EActionType Type);



private:
	UPROPERTY(EditAnywhere)
	TMap<EActionType, float> PriorityAfterDoThisAction;

	UPROPERTY(EditAnywhere)
	TMap<EActionType, float> PriorityAfterDoAnotherAction;
};
