// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerTreeElementWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "WorkerTreeElementListener.h"

UWorkerTreeElementWidget::UWorkerTreeElementWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UWorkerTreeElementWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UWorkerTreeElementWidget::InitTreeElementWidget(EActionTreeElementName Name, const FString& Text, IWorkerTreeElementListener* Listener)
{
	TreeElementName = Name;
	ButtonListener = Listener;
	VisibleName->SetText(FText::FromString(Text));

	Select->OnClicked.AddDynamic(this, &UWorkerTreeElementWidget::PressOnSelect);
	Diselect->OnClicked.AddDynamic(this, &UWorkerTreeElementWidget::PressOnDiselect);

}

void UWorkerTreeElementWidget::HideSelectButton()
{
	Select->SetVisibility(ESlateVisibility::Hidden);
}

void UWorkerTreeElementWidget::ShowSelectButton()
{
	Select->SetVisibility(ESlateVisibility::Visible);
}

void UWorkerTreeElementWidget::HideDiselectButton()
{
	Diselect->SetVisibility(ESlateVisibility::Hidden);
}

void UWorkerTreeElementWidget::ShowDiselectButton()
{
	Diselect->SetVisibility(ESlateVisibility::Visible);
}

void UWorkerTreeElementWidget::ClearMemorySize()
{
	MemorySize->SetText(FText::FromString(""));
}

void UWorkerTreeElementWidget::SetMemorySize(int32 Size)
{
	MemorySize->SetText(FText::FromString(FString::FromInt(Size)));
}

EActionTreeElementName UWorkerTreeElementWidget::GetTreeElementName() const
{
	return TreeElementName;
}

void UWorkerTreeElementWidget::PressOnSelect()
{
	ButtonListener->PressedWorkerTreeElementSelect(this, TreeElementName);
}


void UWorkerTreeElementWidget::PressOnDiselect()
{
	ButtonListener->PressedWorkerTreeElementDiselect(this, TreeElementName);
}

void UWorkerTreeElementWidget::MakeActive()
{
	VisibleName->SetColorAndOpacity(FLinearColor::Green);
}

void UWorkerTreeElementWidget::MakeUnActive()
{
	VisibleName->SetColorAndOpacity(FLinearColor::White);
}