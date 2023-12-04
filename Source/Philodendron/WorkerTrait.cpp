// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerTrait.h"

float FWorkerTrait::GetDeltaPriorityOnActionDone(EActionType Type)
{
	if (!PriorityAfterDoThisAction.Contains(Type))
		return 0.f;

	return PriorityAfterDoThisAction[Type];
}

float FWorkerTrait::GetDeltaPriorityOnOtherActionDone(EActionType Type)
{
	if (!PriorityAfterDoAnotherAction.Contains(Type))
		return 0.f;

	return PriorityAfterDoAnotherAction[Type];
}


void FWorkerTrait::ChangePriorityAfterDoAction(EActionType Type, float Delta)
{
	if (!PriorityAfterDoThisAction.Contains(Type))
	{
		PriorityAfterDoThisAction.Add(Type, 0);
	}
	PriorityAfterDoThisAction[Type] += Delta;
}

void FWorkerTrait::ChangePriorityAfterDoAnotherAction(EActionType Type, float Delta)
{
	if (!PriorityAfterDoAnotherAction.Contains(Type))
	{
		PriorityAfterDoAnotherAction.Add(Type, 0);
	}
	PriorityAfterDoAnotherAction[Type] += Delta;
}