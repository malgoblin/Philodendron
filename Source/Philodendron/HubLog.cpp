// Fill out your copyright notice in the Description page of Project Settings.


#include "HubLog.h"

void UHubLog::InitHubLog(const FString* HName)
{
	HubName = HName;
}

void UHubLog::AddNewWorkerPawn(const FString& WPawnName)
{
	LogArray.Add(*HubName + ": Add new Worker " + WPawnName);
}

void UHubLog::AddNewPointToDestination(const FVector& Point)
{
	LogArray.Add(*HubName + ": Add new Point To Destination X: " + FString::FromInt(Point.X) + " Y: " + FString::FromInt(Point.Y) + " Z: " + FString::FromInt(Point.Z));
}

void UHubLog::AddNewActionToMake(FString&& ActionName)
{
	LogArray.Add(*HubName + ": Next Action " + ActionName);

}

void UHubLog::AddNewRecipeToMake(FString&& RecipeName)
{
	LogArray.Add(*HubName + ": Recipe to Make " + RecipeName);
}

void UHubLog::AddNewInteractableActorToInteract(FString&& IActorType)
{
	LogArray.Add(*HubName + ": Interactable Actor To Interact " + IActorType);
}