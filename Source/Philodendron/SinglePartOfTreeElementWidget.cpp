// Fill out your copyright notice in the Description page of Project Settings.


#include "SinglePartOfTreeElementWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "SinglePartOfTreeElementListener.h"

USinglePartOfTreeElementWidget::USinglePartOfTreeElementWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USinglePartOfTreeElementWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

bool USinglePartOfTreeElementWidget::Init(EActionTreeElementName Name, const FString& Text, ISinglePartOfTreeElementListener* Listener)
{
	if (Name == EActionTreeElementName::Invalid || !Listener)
	{
		return false;
	}

	TreeElementName = Name;

	TextSinglePart->SetText(FText::FromString(Text));

	PressedListener = Listener;
	GreenLightImage->SetVisibility(ESlateVisibility::Hidden);
	ButtonSinglePart->OnClicked.AddDynamic(this, &USinglePartOfTreeElementWidget::PressedToButton);

	return true;
}

void USinglePartOfTreeElementWidget::PressedToButton()
{
	PressedListener->PressedSinglePartOfTreeElement(TreeElementName, this);
}

EActionTreeElementName USinglePartOfTreeElementWidget::GetTreeElementName() const
{
	return TreeElementName;
}

void USinglePartOfTreeElementWidget::MakeActive()
{
	GreenLightImage->SetVisibility(ESlateVisibility::Visible);
}

void USinglePartOfTreeElementWidget::MakeUnActive()
{
	GreenLightImage->SetVisibility(ESlateVisibility::Hidden);
}