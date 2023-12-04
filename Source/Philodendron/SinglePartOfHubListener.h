
#pragma once

#include "CoreMinimal.h"
#include "SinglePartOfHubListener.generated.h"

class USinglePartOfHubWidget;
enum class ESinglePartOfHubType;

UINTERFACE()
class USinglePartOfHubListener : public UInterface
{
	GENERATED_BODY()
};

class ISinglePartOfHubListener
{
	GENERATED_BODY()
public:
	virtual void PressedSinglePartOfHub(TObjectPtr<USinglePartOfHubWidget> Widget, ESinglePartOfHubType Type) = 0;

};