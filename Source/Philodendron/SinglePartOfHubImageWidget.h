// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnumSinglePartOfHubType.h"
#include "EnumWorkerType.h"
#include "EnumWorkerAddonType.h"
#include "SinglePartOfHubImageWidget.generated.h"

class ISinglePartOfHubImageListener;
class UImage;
class UBorder;

UCLASS()
class PHILODENDRON_API USinglePartOfHubImageWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	USinglePartOfHubImageWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void Init(ISinglePartOfHubImageListener* Listener, ESinglePartOfHubType Type, ESinglePartOfHubPossitionType PossitionType, EWorkerType WType, EAddonPossitionType APType);

	void SetImage(const TObjectPtr<UTexture> Image);

	void ShowImage();

	void HideImage();

	ESinglePartOfHubType GetSinglePartOfHubType() const;
	ESinglePartOfHubPossitionType GetSinglePartOfHubPossitionType() const;
	EWorkerType GetWorkerType() const;
	EAddonPossitionType GetAddonPossitionType() const;

private:
	UFUNCTION()
	void ClickOnBorder();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UImage> ObjectImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UBorder> ObjectBorder;

private:
	ISinglePartOfHubImageListener* PressListener = nullptr;

	UPROPERTY()
	ESinglePartOfHubType PartType = ESinglePartOfHubType::Invalid;
	
	UPROPERTY()
	ESinglePartOfHubPossitionType PartPossitionType = ESinglePartOfHubPossitionType::Invalid;
	
	UPROPERTY()
	EWorkerType WorkerType = EWorkerType::Invalid;
	
	UPROPERTY()
	EAddonPossitionType AddonPossitionType = EAddonPossitionType::Invalid;
};
