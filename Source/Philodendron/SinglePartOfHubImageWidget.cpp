// Fill out your copyright notice in the Description page of Project Settings.


#include "SinglePartOfHubImageWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "SinglePartOfHubImageListener.h"

USinglePartOfHubImageWidget::USinglePartOfHubImageWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USinglePartOfHubImageWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USinglePartOfHubImageWidget::Init(ISinglePartOfHubImageListener* Listener, ESinglePartOfHubType Type, ESinglePartOfHubPossitionType PossitionType, EWorkerType WType, EAddonPossitionType APType)
{
	PressListener = Listener;

	PartType = Type;

	HideImage();

	PartPossitionType = PossitionType;

	ObjectBorder->OnMouseButtonUpEvent.BindUFunction(this, "ClickOnBorder");
	WorkerType = WType;
	AddonPossitionType = APType;

}

void USinglePartOfHubImageWidget::ShowImage()
{
	ObjectImage->SetVisibility(ESlateVisibility::Visible);
}

void USinglePartOfHubImageWidget::HideImage()
{
	ObjectImage->SetVisibility(ESlateVisibility::Hidden);
}

void USinglePartOfHubImageWidget::SetImage(const TObjectPtr<UTexture> Image)
{
	ObjectImage->Brush.SetResourceObject(Image);
}

ESinglePartOfHubType USinglePartOfHubImageWidget::GetSinglePartOfHubType() const
{
	return PartType;
}

ESinglePartOfHubPossitionType USinglePartOfHubImageWidget::GetSinglePartOfHubPossitionType() const
{
	return PartPossitionType;
}

void USinglePartOfHubImageWidget::ClickOnBorder()
{
	PressListener->PressedSinglePartOfHubImage(this, PartType, PartPossitionType);
}

EWorkerType USinglePartOfHubImageWidget::GetWorkerType() const
{
	return WorkerType;
}

EAddonPossitionType USinglePartOfHubImageWidget::GetAddonPossitionType() const
{
	return AddonPossitionType;
}