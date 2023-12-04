// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadOnlyWidget.generated.h"

class UBorder;
class UWidgetSwitcher;

UCLASS()
class PHILODENDRON_API UReadOnlyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UReadOnlyWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Init();
	bool IsActive();

	void MakeActive();
	void MakeUnActive();

private:
	UFUNCTION()
	void ClickOnBorder();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UWidgetSwitcher> ObjectSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UBorder> ObjectBorder;

private:
	UPROPERTY()
	bool bActive = false;
};
