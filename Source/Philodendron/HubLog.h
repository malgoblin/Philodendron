// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseLog.h"


class PHILODENDRON_API UHubLog : public UBaseLog
{
public:
	void InitHubLog(const FString* HName);
	void AddNewWorkerPawn(const FString& WPawnName);
	void AddNewPointToDestination(const FVector& Point);
	void AddNewActionToMake(FString&& ActionName);
	void AddNewInteractableActorToInteract(FString&& IActorType);
	void AddNewRecipeToMake(FString&& RecipeName);

private:
	const FString* HubName = nullptr;
};
