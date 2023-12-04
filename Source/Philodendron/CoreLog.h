#pragma once

#include "CoreMinimal.h"
#include "BaseLog.h"

class PHILODENDRON_API UCoreLog : public UBaseLog
{
public:
	void AddManyLogLines(const TArray<FString>& NewFStringArray);
	const TArray<FString>* GetCoreLog() const;
};
