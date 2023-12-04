// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UserHUDInterface.generated.h"


UINTERFACE()
class UUserHUDInterface : public UInterface
{
	GENERATED_BODY()
};

class IUserHUDInterface
{
	GENERATED_BODY()
public:
	virtual void CloseHUD() = 0;
};