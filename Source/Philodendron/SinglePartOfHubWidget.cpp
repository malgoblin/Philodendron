// Fill out your copyright notice in the Description page of Project Settings.


#include "SinglePartOfHubWidget.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "SinglePartOfHubListener.h"

USinglePartOfHubWidget::USinglePartOfHubWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USinglePartOfHubWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USinglePartOfHubWidget::Init(const FString& Name, ISinglePartOfHubListener* Listener, ESinglePartOfHubType Type, const TObjectPtr<UTexture> Image)
{
	PartType = Type;

	PressedListener = Listener;

	ObjectName->SetText(FText::FromString(Name));

	MainImage->Brush.SetResourceObject(Image);
	ObjectGreenLight->SetVisibility(ESlateVisibility::Hidden);

	ButtonObject->OnClicked.AddDynamic(this, &USinglePartOfHubWidget::PressedBorder);
}

void USinglePartOfHubWidget::PressedBorder()
{
	PressedListener->PressedSinglePartOfHub(this, PartType);
}

void USinglePartOfHubWidget::MakeActive()
{
	ObjectGreenLight->SetVisibility(ESlateVisibility::Visible);
}

void USinglePartOfHubWidget::MakeUnActive()
{
	ObjectGreenLight->SetVisibility(ESlateVisibility::Hidden);
}

ESinglePartOfHubType USinglePartOfHubWidget::GetPartOfHubType() const
{
	return PartType;
}