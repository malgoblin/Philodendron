
#pragma once

#include "CoreMinimal.h"
#include "KnownObjectInfoListener.generated.h"

class UKnownObjectInfoWidget;

UINTERFACE()
class UKnownObjectInfoListener : public UInterface
{
	GENERATED_BODY()
};

class IKnownObjectInfoListener
{
	GENERATED_BODY()
public:
	virtual void PressKnownObject(TObjectPtr<UKnownObjectInfoWidget> ObjectWidget) = 0;
};
