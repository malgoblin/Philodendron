// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerCreatorWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "MainPlayer.h"
#include "CoreHUD.h"
#include "WorkerPawn.h"
#include "WorkerHub.h"
#include "WorkerAddon.h"
#include "Components/EditableTextBox.h"
#include "EnumWorkerType.h"
#include "EnumWorkerAddonType.h"
#include "MainGameStateBase.h"
#include "SinglePartOfHubImageWidget.h"
#include "ReadOnlyWidget.h"


UWorkerCreatorWidget::UWorkerCreatorWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UWorkerCreatorWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UWorkerCreatorWidget::InitHUD(TObjectPtr<AMainPlayer> Core, TObjectPtr<ACoreHUD> CHUD)
{
	MainCore = Core;
	CoreHUD = CHUD;
	AddNewWorker->OnClicked.AddDynamic(this, &UWorkerCreatorWidget::ClickOnAddNewWorker);
	CreateWorker->OnClicked.AddDynamic(this, &UWorkerCreatorWidget::ClickOnCreateWorker);
	TakeApartWorker->OnClicked.AddDynamic(this, &UWorkerCreatorWidget::ClickOnTakeApartWorker);
	
	ButtonClose->OnClicked.AddDynamic(this, &UWorkerCreatorWidget::ClickCloseButton);
	TurnOnPawnList->OnClicked.AddDynamic(this, &UWorkerCreatorWidget::ClickTurnOnPawnList);
	TurnOnAddonList->OnClicked.AddDynamic(this, &UWorkerCreatorWidget::ClickTurnOnAddonList);

	HubImage->Init(this, ESinglePartOfHubType::Hub, ESinglePartOfHubPossitionType::Invalid, EWorkerType::Invalid, EAddonPossitionType::Invalid);
	HubImage->ShowImage();

	TopPawnImage->Init(this, ESinglePartOfHubType::Pawn, ESinglePartOfHubPossitionType::TopPawn, EWorkerType::Top, EAddonPossitionType::Invalid);
	LeftPawnImage->Init(this, ESinglePartOfHubType::Pawn, ESinglePartOfHubPossitionType::LeftPawn, EWorkerType::Left, EAddonPossitionType::Invalid);
	RightPawnImage->Init(this, ESinglePartOfHubType::Pawn, ESinglePartOfHubPossitionType::RightPawn, EWorkerType::Right, EAddonPossitionType::Invalid);

	PawnPossitionList.Add(EWorkerType::Top, TopPawnImage);
	PawnPossitionList.Add(EWorkerType::Left, LeftPawnImage);
	PawnPossitionList.Add(EWorkerType::Right, RightPawnImage);

	TopAddon0Image->Init(this, ESinglePartOfHubType::Addon, ESinglePartOfHubPossitionType::TopAddon0, EWorkerType::Top, EAddonPossitionType::Zero);
	TopAddon1Image->Init(this, ESinglePartOfHubType::Addon, ESinglePartOfHubPossitionType::TopAddon1, EWorkerType::Top, EAddonPossitionType::First);

	LeftAddon0Image->Init(this, ESinglePartOfHubType::Addon, ESinglePartOfHubPossitionType::LeftAddon0, EWorkerType::Left, EAddonPossitionType::Zero);
	LeftAddon1Image->Init(this, ESinglePartOfHubType::Addon, ESinglePartOfHubPossitionType::LeftAddon1, EWorkerType::Left, EAddonPossitionType::First);

	RightAddon0Image->Init(this, ESinglePartOfHubType::Addon, ESinglePartOfHubPossitionType::RightAddon0, EWorkerType::Right, EAddonPossitionType::Zero);
	RightAddon1Image->Init(this, ESinglePartOfHubType::Addon, ESinglePartOfHubPossitionType::RightAddon1, EWorkerType::Right, EAddonPossitionType::First);

	AddonPossitionList.Add(TopAddon0Image);
	AddonPossitionList.Add(TopAddon1Image);
	AddonPossitionList.Add(LeftAddon0Image);
	AddonPossitionList.Add(LeftAddon1Image);
	AddonPossitionList.Add(RightAddon0Image);
	AddonPossitionList.Add(RightAddon1Image);

	TopPawnReadOnly->Init();
	LeftPawnReadOnly->Init();
	RightPawnReadOnly->Init();

	ReadOnlyList.Add(EWorkerType::Top, TopPawnReadOnly);
	ReadOnlyList.Add(EWorkerType::Left, LeftPawnReadOnly);
	ReadOnlyList.Add(EWorkerType::Right, RightPawnReadOnly);

}

void UWorkerCreatorWidget::OpenCreator()
{
	UpdateHUD();
	this->SetVisibility(ESlateVisibility::Visible);
	MainCore->PauseGame();
	CoreHUD->AddActivityHUD(this);
	MainCore->TurnOffControls();

	HubMainCanvas->SetVisibility(ESlateVisibility::Hidden);
}

void UWorkerCreatorWidget::CloseHUD()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	MainCore->ContinueGame();
	MainCore->TurnOnControls();

	ResetHUD();
}

void UWorkerCreatorWidget::UpdateHUD()
{
	UpdateAttachedPawnList();

	UpdateAttachedHubList();

	UpdateExistedAddonList();

}

#pragma optimize("", off)
void UWorkerCreatorWidget::PressedSinglePartOfHub(TObjectPtr<USinglePartOfHubWidget> Widget, ESinglePartOfHubType Type)
{
	if (Type == ESinglePartOfHubType::Hub)
	{
		SelectHub(Widget);
		return;
	}

	if (!SelectedHub)
		return;

	if (SelectedPartOfHubWidget)
	{
		SelectedPartOfHubWidget->MakeUnActive();
	}

	if (SelectedPartOfHubWidget == Widget)
	{
		SelectedPartOfHubWidget = nullptr;
		return;
	}

	SelectedPartOfHubWidget = Widget;
	SelectedPartOfHubWidget->MakeActive();
}

#pragma optimize("", on)
#pragma optimize("", off)
void UWorkerCreatorWidget::SelectHub(TObjectPtr<USinglePartOfHubWidget> Widget)
{
	if (Widget == SelectedHubWidget)
	{
		ResetHub();
		return;
	}

	ResetHub();
	HubMainCanvas->SetVisibility(ESlateVisibility::Visible);
	SelectedHubWidget = Widget;
	SelectedHubWidget->MakeActive();

	SelectedHub = AttachedHubs[Widget];

	for (const auto& WPawn : SelectedHub->GetConnectedWorkers())
	{
		if (!WPawn.Value)
			continue;

		SelectedPawns.Add(WPawn.Value, PawnPossitionList[WPawn.Key]);
		PawnPossitionList[WPawn.Key]->ShowImage();

		if (WPawn.Value->GetReadOnly())
		{
			ReadOnlyList[WPawn.Key]->MakeActive();
		}

		for (const auto& Addon : WPawn.Value->GetWorkerAddon())
		{
			if (!Addon.Value)
				continue;

			for (auto& AddonPoss : AddonPossitionList)
			{
				if (AddonPoss->GetWorkerType() == WPawn.Key && AddonPoss->GetAddonPossitionType() == Addon.Key)
				{
					SelectedAddons.Add(Addon.Value, AddonPoss);
					AddonPoss->ShowImage();
					break;
				}
			}
		}
	}

}
#pragma optimize("", on)
void UWorkerCreatorWidget::PressedSinglePartOfHubImage(TObjectPtr<USinglePartOfHubImageWidget> Widget, ESinglePartOfHubType Type, ESinglePartOfHubPossitionType PossitionType)
{
	if (!SelectedHub)
		return;

	if (SelectedPartOfHubWidget && SelectedPartOfHubWidget->GetPartOfHubType() != Widget->GetSinglePartOfHubType())
		return;

	if (Type == ESinglePartOfHubType::Pawn)
	{
		SelectPawnPossition(Widget);
	}

	if (Type == ESinglePartOfHubType::Addon)
	{
		SelectAddonPossition(Widget);
	}
	
}

void UWorkerCreatorWidget::SelectPawnPossition(TObjectPtr<USinglePartOfHubImageWidget> Widget)
{
	TObjectPtr<AWorkerPawn> TempPawn = nullptr;
	for (const auto& WPawn : SelectedPawns)
	{
		if (WPawn.Value == Widget)
		{
			TempPawn = WPawn.Key;
			break;
		}
	}

	if (SelectedHub->HasPawn(TempPawn))
		return;

	Widget->HideImage();
	SelectedPawns.Remove(TempPawn);

	if (!SelectedPartOfHubWidget)
	{
		UpdateAttachedPawnList();
		return;
	}

	Widget->ShowImage();
	SelectedPawns.Add(AttachedPawns[SelectedPartOfHubWidget], Widget);

	SelectedPartOfHubWidget->MakeUnActive();
	SelectedPartOfHubWidget = nullptr;

	UpdateAttachedPawnList();
}

void UWorkerCreatorWidget::SelectAddonPossition(TObjectPtr<USinglePartOfHubImageWidget> Widget)
{
	Widget->HideImage();

	TObjectPtr<AWorkerAddon> TempAddon = nullptr;
	for (const auto& Addon : SelectedAddons)
	{
		if (Addon.Value == Widget)
		{
			TempAddon = Addon.Key;
			break;
		}
	}

	if (TempAddon)
	{
		if (SelectedHub->RemoveAddon(TempAddon))
		{
			MainCore->AddAddonsToCore(TempAddon);
		}
	}

	SelectedAddons.Remove(TempAddon);

	if (!SelectedPartOfHubWidget)
	{
		UpdateExistedAddonList();
		return;
	}


	Widget->SetImage(Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetAddonImage(ExistedAddons[SelectedPartOfHubWidget]->GetAddonType()));

	Widget->ShowImage();
	SelectedAddons.Add(ExistedAddons[SelectedPartOfHubWidget], Widget);

	SelectedPartOfHubWidget->MakeUnActive();
	SelectedPartOfHubWidget = nullptr;

	UpdateExistedAddonList();

}

void UWorkerCreatorWidget::ResetHUD()
{
	AttachedWorkerPawnToCore->ClearChildren();
	AttachedWorkerHubToCore->ClearChildren();
	ExistedAddonsInCore->ClearChildren();

	AttachedPawns.Empty();
	AttachedHubs.Empty();
	ExistedAddons.Empty();

	HubImage->SetVisibility(ESlateVisibility::Hidden);

}

void UWorkerCreatorWidget::ClickOnAddNewWorker()
{
	ResetHub();
	HubMainCanvas->SetVisibility(ESlateVisibility::Visible);

	SelectedHub = MainCore->CreateTempHub(InitDefaultHubName());

	UpdateAttachedPawnList();
	UpdateExistedAddonList();
}

void UWorkerCreatorWidget::ResetHub()
{
	for (const auto& WPawn : SelectedPawns)
	{
		WPawn.Value->HideImage();
	}

	for (const auto& Addon : SelectedAddons)
	{
		Addon.Value->HideImage();
	}

	SelectedPawns.Empty();
	SelectedAddons.Empty();

	SelectedHub = nullptr;
	if (SelectedHubWidget)
	{
		SelectedHubWidget->MakeUnActive();
		SelectedHubWidget = nullptr;
	}
	if (SelectedPartOfHubWidget)
	{
		SelectedPartOfHubWidget->MakeUnActive();
		SelectedPartOfHubWidget = nullptr;
	}

	for (auto& Temp : ReadOnlyList)
	{
		Temp.Value->MakeUnActive();
	}

	HubMainCanvas->SetVisibility(ESlateVisibility::Hidden);
}
#pragma optimize("", off)
void UWorkerCreatorWidget::ClickOnCreateWorker()
{
	if (!SelectedHub)
		return;

	TMap<EWorkerType, TObjectPtr<AWorkerPawn>> TempPawnMap;
	TMap<TObjectPtr<AWorkerPawn>, bool> TempPawnMapReadOnly;
	TMap<TObjectPtr<AWorkerPawn>, TMap<EAddonPossitionType, TObjectPtr<AWorkerAddon>>> TempAddonMap;

	for (const auto& WPawn : SelectedPawns)
	{
		TempPawnMap.Add(WPawn.Value->GetWorkerType(), WPawn.Key);
	}

	for (const auto& Addon : SelectedAddons)
	{
		if (!TempPawnMap.Contains(Addon.Value->GetWorkerType()))
			continue;
		
		TMap<EAddonPossitionType, TObjectPtr<AWorkerAddon>> TempInternalAddonMap;
		TempInternalAddonMap.Add(Addon.Value->GetAddonPossitionType(), Addon.Key);

		if (TempAddonMap.Contains(TempPawnMap[Addon.Value->GetWorkerType()]))
		{
			TempAddonMap[TempPawnMap[Addon.Value->GetWorkerType()]].Add(Addon.Value->GetAddonPossitionType(), Addon.Key);
		}
		else
		{
			TempAddonMap.Add(TempPawnMap[Addon.Value->GetWorkerType()], TempInternalAddonMap);
		}
	}

	for (const auto& WPawn : TempPawnMap)
	{
		TempPawnMapReadOnly.Add(WPawn.Value, ReadOnlyList[WPawn.Key]->IsActive());
	}

	bool Result;

	if (!SelectedHubWidget)
	{
		Result = MainCore->CreateNewHubFromTemp(TempPawnMap, TempPawnMapReadOnly, TempAddonMap);
	}
	else
	{
		Result = MainCore->UpdateExistedHub(SelectedHub, TempPawnMap, TempPawnMapReadOnly, TempAddonMap);
	}

	if (!Result)
		return;

	ResetHub();

	UpdateHUD();
}
#pragma optimize("", on)
void UWorkerCreatorWidget::ClickOnTakeApartWorker()
{
	if (!SelectedHub)
		return;

	if (!SelectedHubWidget)
	{
		ResetHub();
		UpdateHUD();
		return;
	}

	SelectedHub->GiveAllPawnAndAddonsToCore();
	MainCore->RemoveHub(SelectedHub);

	ResetHub();
	UpdateHUD();
}


FString UWorkerCreatorWidget::InitDefaultHubName()
{
	int32 HubNumber = MainCore->GetHubNumber();
	return "Hub_" + FString::FromInt(HubNumber);
}

void UWorkerCreatorWidget::UpdateAttachedPawnList()
{
	AttachedPawns.Empty();
	AttachedWorkerPawnToCore->ClearChildren();

	for (const auto& WPawn : MainCore->GetAttachedWorkers())
	{
		if (SelectedPawns.Contains(WPawn))
			continue;

		TObjectPtr<USinglePartOfHubWidget> NewButton = CreateWidget<USinglePartOfHubWidget>(this->GetWorld(), SinglePartOfHubWidgetClass);
		NewButton->Init(WPawn->GetWorkerName(), this, ESinglePartOfHubType::Pawn, Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetWorkerPawnIcon());
		AttachedWorkerPawnToCore->AddChild(NewButton);
		AttachedPawns.Add(NewButton, WPawn);
	}
}

void UWorkerCreatorWidget::UpdateExistedAddonList()
{
	ExistedAddons.Empty();
	ExistedAddonsInCore->ClearChildren();

	for (const auto& Addon : MainCore->GetExistedAddons())
	{
		if (SelectedAddons.Contains(Addon))
			continue;

		TObjectPtr<USinglePartOfHubWidget> NewButton = CreateWidget<USinglePartOfHubWidget>(this->GetWorld(), SinglePartOfHubWidgetClass);
		NewButton->Init(Addon->GetAddonName(), this, ESinglePartOfHubType::Addon, Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetAddonIcon(Addon->GetAddonType()));
		ExistedAddonsInCore->AddChild(NewButton);
		ExistedAddons.Add(NewButton, Addon);
	}
}

void UWorkerCreatorWidget::UpdateAttachedHubList()
{
	AttachedHubs.Empty();
	AttachedWorkerHubToCore->ClearChildren();

	for (const auto& WHub : MainCore->GetAttachedWorkerHubs())
	{
		TObjectPtr<USinglePartOfHubWidget> NewButton = CreateWidget<USinglePartOfHubWidget>(this->GetWorld(), SinglePartOfHubWidgetClass);
		NewButton->Init(WHub->GetHubName(), this, ESinglePartOfHubType::Hub, Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetWorkerHubIcon());
		AttachedWorkerHubToCore->AddChild(NewButton);
		AttachedHubs.Add(NewButton, WHub);
	}
}


void UWorkerCreatorWidget::ClickTurnOnPawnList()
{
	SinglePartSwitcher->SetActiveWidget(AttachedWorkerPawnToCore);
}

void UWorkerCreatorWidget::ClickTurnOnAddonList()
{
	SinglePartSwitcher->SetActiveWidget(ExistedAddonsInCore);
}

void UWorkerCreatorWidget::ClickCloseButton()
{
	CoreHUD->OpenCloseMenu();
}