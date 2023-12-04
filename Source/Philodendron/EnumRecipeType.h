// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ERecipeType
{
	Invalid = 0,
	Hole,
	Cesspool,
	Compost,
	StoneDust,
	StoneBlock,
	Cement,
	Fertilizer,
	Foundry,
	IronBlock,
	IronTree,
};

UENUM()
enum class EGlobalRecipeType
{
	Invalid = 0,
	Resource,
	InteractableActor,
	Building,
};

UENUM()
enum class EPlaceWhereMakingRecipe
{
	Invalid = 0,
	Empty,
	Building,
	InteractableActor,
};

UENUM()
enum class ENeedWorkerToMakeRecipe
{
	Invalid = 0,
	NeedWorker,
	DontNeedworker,
};
