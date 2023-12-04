// Fill out your copyright notice in the Description page of Project Settings.


#include "ReadOnlyWidget.h"
#include "Components/Border.h"
#include "Components/WidgetSwitcher.h"


UReadOnlyWidget::UReadOnlyWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UReadOnlyWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UReadOnlyWidget::Init()
{
	ObjectBorder->OnMouseButtonUpEvent.BindUFunction(this, "ClickOnBorder");
}

bool UReadOnlyWidget::IsActive()
{
	return bActive;
}

void UReadOnlyWidget::ClickOnBorder()
{
	if (bActive)
	{
		MakeUnActive();
		return;
	}

	MakeActive();
}

void UReadOnlyWidget::MakeActive()
{
	ObjectSwitcher->SetActiveWidgetIndex(1);
	bActive = true;
}

void UReadOnlyWidget::MakeUnActive()
{
	ObjectSwitcher->SetActiveWidgetIndex(0);
	bActive = false;
}