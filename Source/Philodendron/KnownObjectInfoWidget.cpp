// Fill out your copyright notice in the Description page of Project Settings.


#include "KnownObjectInfoWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "KnownObjectInfoListener.h"

UKnownObjectInfoWidget::UKnownObjectInfoWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UKnownObjectInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UKnownObjectInfoWidget::Init(const FString& Name, const FVector& Location, IKnownObjectInfoListener* Listener)
{
	ButtonKnownObject->OnClicked.AddDynamic(this, &UKnownObjectInfoWidget::PressOnButton);


	PressListener = Listener;

	ObjectName->SetText(FText::FromString(Name));
	LocationXY->SetText(FText::FromString("X: " + FString::FromInt(Location.X) + " ; Y: " + FString::FromInt(Location.Y)));
	GreenLightImage->SetVisibility(ESlateVisibility::Hidden);
}

void UKnownObjectInfoWidget::PressOnButton()
{
	PressListener->PressKnownObject(this);
}

void UKnownObjectInfoWidget::MakeActive()
{
	GreenLightImage->SetVisibility(ESlateVisibility::Visible);
}

void UKnownObjectInfoWidget::MakeUnActive()
{
	GreenLightImage->SetVisibility(ESlateVisibility::Hidden);
}