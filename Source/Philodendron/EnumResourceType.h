// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EResourceType
{
	Invalid = 0,
	RedWood,
	BunchOfLeaf,
	BunchOfGrass,
	Compost,
	Stone,
	Clay,
	Gypsum,
	StoneDust,
	StoneBlock,
	Coal,
	Iron,
	Cement,
	Fertilizer,
	IronBlock,
};

UENUM()
enum class ESubResourceType
{
	Invalid = 0,
	Wood,
	Red,
	Leaf,
	Grass,
	Bunch,
	Clay,
	Gypsum,
	Stone,
};
