// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnumActionTreeElementName.h"
#include "SinglePartOfTreeElementWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;
class ISinglePartOfTreeElementListener;

UCLASS()
class PHILODENDRON_API USinglePartOfTreeElementWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USinglePartOfTreeElementWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	bool Init(EActionTreeElementName Name, const FString& Text, ISinglePartOfTreeElementListener* Listener);

	EActionTreeElementName GetTreeElementName() const;

	void MakeActive();
	void MakeUnActive();

private:
	UFUNCTION()
	void PressedToButton();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UTextBlock> TextSinglePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> ButtonSinglePart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UImage> GreenLightImage;

private:
	UPROPERTY()
	EActionTreeElementName TreeElementName = EActionTreeElementName::Invalid;

	ISinglePartOfTreeElementListener* PressedListener = nullptr;
};
