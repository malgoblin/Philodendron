// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumActionList.h"

UENUM()
enum class EWorkerAddonType
{
	Invalid = 0,
	Arm,
	Laser,
	GravGrab,
};

UENUM()
enum class EAddonModifiers
{
	Invalid = 0,
	IcreaseWeightLimit,
};

UENUM()
enum class EAddonPossitionType
{
	Invalid = 0,
	Zero,
	First,
};
