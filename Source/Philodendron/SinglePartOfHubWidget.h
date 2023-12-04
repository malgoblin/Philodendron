// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnumSinglePartOfHubType.h"
#include "SinglePartOfHubWidget.generated.h"


class UTextBlock;
class UBorder;
class UButton;
class ISinglePartOfHubListener;
class UImage;

UCLASS()
class PHILODENDRON_API USinglePartOfHubWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USinglePartOfHubWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;
	
	void Init(const FString& Name, ISinglePartOfHubListener* Listener, ESinglePartOfHubType Type, const TObjectPtr<UTexture> Image);
	
	void MakeActive();

	void MakeUnActive();

	ESinglePartOfHubType GetPartOfHubType() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UTextBlock> ObjectName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> ButtonObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UImage> ObjectGreenLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UImage> MainImage;

private:
	UFUNCTION()
	void PressedBorder();

private:
	ISinglePartOfHubListener* PressedListener = nullptr;

	UPROPERTY()
	ESinglePartOfHubType PartType = ESinglePartOfHubType::Invalid;
};
