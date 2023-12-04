#pragma once

#include "CoreMinimal.h"
#include "ActionTreeListener.generated.h"

enum class EActionTreeElementName;

UINTERFACE()
class UActionTreeListener : public UInterface
{
	GENERATED_BODY()
};

class IActionTreeListener
{
	GENERATED_BODY()
public:
	virtual bool AddNewTreeElementListToMemory(const TArray<EActionTreeElementName>& TreeElementList) = 0;
};