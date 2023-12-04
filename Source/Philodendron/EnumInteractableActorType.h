#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EInteractableActorType
{
	Invalid = 0,
	Tree,
	Grass,
	Core,
	Bush,
	Resource,
	Hole,
	Empty,
	Stone,
	ClayDeposit,
	GypsumDeposit,
	CoalDeposit,
	IronDeposit,
};

UENUM()
enum class EInteractableActorCreatingType
{
	TakeFromIAManager = 0,
	FirstStage,
	FinalStage,
	RandomStage,
};
