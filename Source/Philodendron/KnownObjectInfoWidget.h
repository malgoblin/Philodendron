// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KnownObjectInfoWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;
class IKnownObjectInfoListener;


UCLASS()
class PHILODENDRON_API UKnownObjectInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UKnownObjectInfoWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Init(const FString& Name, const FVector& Location, IKnownObjectInfoListener* Listener);

	void MakeActive();
	void MakeUnActive();

private:
	UFUNCTION()
	void PressOnButton();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UTextBlock> ObjectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UTextBlock> LocationXY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UImage> GreenLightImage;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> ButtonKnownObject;

private:
	IKnownObjectInfoListener* PressListener = nullptr;
};
