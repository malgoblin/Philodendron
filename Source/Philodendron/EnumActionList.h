// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EActionType
{
	Invalid = 0,
	Research,
	HarvestTree,
	HarvestBush,
	HarvestGrass,
	PullOutBush,
	MakeRecipe,
	BackToCore,
	TakeResource,
	MiningStone,
	HarvestClay,
	Heat,
	Sort,
	MiningGypsum,
	MiningCoal,
	MiningIron,
};