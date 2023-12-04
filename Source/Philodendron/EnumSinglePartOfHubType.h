// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ESinglePartOfHubType
{
	Invalid = 0,
	Addon,
	Pawn,
	Hub,
};

UENUM()
enum class ESinglePartOfHubPossitionType
{
	Invalid = 0,
	TopPawn,
	LeftPawn,
	RightPawn,
	TopAddon0,
	TopAddon1,
	LeftAddon0,
	LeftAddon1,
	RightAddon0,
	RightAddon1,
};