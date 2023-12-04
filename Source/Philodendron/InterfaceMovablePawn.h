#pragma once
#include "CoreMinimal.h"
#include "InterfaceMovablePawn.generated.h"


UINTERFACE()
class UInterfaceMovablePawn : public UInterface
{
	GENERATED_BODY()
};

class IInterfaceMovablePawn
{
	GENERATED_BODY()
public:
	virtual void SetNewDestination(const TArray<FVector>& Path) = 0;
	virtual const FVector GetPawnLocation() const = 0;
	virtual const double GetPawnCloseToObjectDistanceSq() const = 0;
	virtual const float GetPawnSpottingRadius() const = 0;
};
