#include "CoreControlPanel.h"
#include "MainGameStateBase.h"
#include "MainPlayer.h"
#include "WorkerPawn.h"
#include "WorkerHub.h"
#include "KnownObjectInfo.h"
#include "WorkerAddon.h"

#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/Checkbox.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/WidgetSwitcher.h"
#include "LogPanel.h"
#include "CoreHUD.h"

UCoreControlPanel::UCoreControlPanel(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCoreControlPanel::NativeConstruct()
{
	Super::NativeConstruct();

}

void UCoreControlPanel::UpdateInterface()
{
	ClearHud();

	for (const auto& WHub : MainCore->GetAttachedWorkerHubs())
	{
		TObjectPtr<USinglePartOfHubWidget> NewButton = CreateWidget<USinglePartOfHubWidget>(this->GetWorld(), SinglePartOfHubWidgetClass);
		NewButton->Init(WHub->GetHubName(), this, ESinglePartOfHubType::Hub, Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetWorkerHubIcon());
		AttachedHubToCore->AddChild(NewButton);
		AttachedHubs.Add(NewButton, WHub);
	}

	for (const auto& Action : MainCore->GetKnownActionList())
	{
		EActionTreeElementName TreeElementName = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetActionTreeElementNameForActionName(Action);

		if (TreeElementName == EActionTreeElementName::Invalid)
		{
			continue;
		}

		TObjectPtr<USinglePartOfTreeElementWidget> NewButton = CreateWidget<USinglePartOfTreeElementWidget>(this->GetWorld(), SinglePartOfTreeElementWidgetClass);

		const FString* ActionName = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetActionName(Action);

		if (!ActionName)
		{
			NewButton->Init(TreeElementName, "!!!MISSING NAME!!!", this);
		}
		else
		{
			NewButton->Init(TreeElementName, *ActionName, this);
		}

		ActionsInCore->AddChild(NewButton);
		WidgetsForCore.Add(NewButton);
	}

	for (const auto& Recipe : MainCore->GetKnownRecipeList())
	{
		EActionTreeElementName TreeElementName = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetActionTreeElementNameForRecipeName(Recipe);

		if (TreeElementName == EActionTreeElementName::Invalid)
		{
			continue;
		}

		TObjectPtr<USinglePartOfTreeElementWidget> NewButton = CreateWidget<USinglePartOfTreeElementWidget>(this->GetWorld(), SinglePartOfTreeElementWidgetClass);

		const FString* RecipeName = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetRecipeName(Recipe);
		if (!RecipeName)
		{
			NewButton->Init(TreeElementName, "!!!MISSING NAME!!!", this);

		}
		else
		{
			NewButton->Init(TreeElementName, *RecipeName, this);

		}
		RecipesInCore->AddChild(NewButton);
		WidgetsForCore.Add(NewButton);
	}

	for (const auto& KnownObject : MainCore->GetKnownObject())
	{
		TObjectPtr<UKnownObjectInfoWidget> NewButton = CreateWidget<UKnownObjectInfoWidget>(this->GetWorld(), KnownObjectInfoWidgetClass);

		const FString* ObjectName = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetInteractableActorName(KnownObject->GetActorType());

		if (!ObjectName)
		{
			NewButton->Init("!!!MISSING NAME!!!", KnownObject->GetKnownObjectLocation(), this);

		}
		else
		{
			NewButton->Init(*ObjectName, KnownObject->GetKnownObjectLocation(), this);

		}
		KnownObjectsInCore->AddChild(NewButton);
		CoreKnownObjectAndWidget.Add(NewButton, KnownObject);
	}
}


void UCoreControlPanel::OpenMenu()
{
	UpdateInterface();
	MainCore->PauseGame();
	this->SetVisibility(ESlateVisibility::Visible);
	MainCore->TurnOffControls();
}

void UCoreControlPanel::CloseHUD()
{
	ClearHud();
	this->SetVisibility(ESlateVisibility::Hidden);
	MainCore->ContinueGame();
	MainCore->TurnOnControls();

}

void UCoreControlPanel::ClickExitButton()
{
	CoreHUD->OpenCloseMenu();
}

void UCoreControlPanel::InitHUD(TObjectPtr<AMainPlayer> Core, TObjectPtr<ACoreHUD> CHUD)
{
	MainCore = Core;
	CoreHUD = CHUD;

	FreeSelectedHub->OnClicked.AddDynamic(this, &UCoreControlPanel::ClickFreeSelectedHub);
	WorkerCreateor->OnClicked.AddDynamic(this, &UCoreControlPanel::ClickWorkerCreator);
	AddSelectedInfoToHub->OnClicked.AddDynamic(this, &UCoreControlPanel::ClickAddSelectedInfoToHub);
	RemoveSelectedInfoFromHub->OnClicked.AddDynamic(this, &UCoreControlPanel::ClickRemoveSelectedInfoFromHub);

	ButtonTurnOnActionListOnHub->OnClicked.AddDynamic(this, &UCoreControlPanel::TurnOnActionListInHub);
	ButtonTurnOnRecipeListOnHub->OnClicked.AddDynamic(this, &UCoreControlPanel::TurnOnRecipeListInHub);
	ButtonTurnOnKnownObjectListOnHub->OnClicked.AddDynamic(this, &UCoreControlPanel::TurnOnKnownObjectListInHub);

	ButtonTurnOnActionListOnCore->OnClicked.AddDynamic(this, &UCoreControlPanel::TurnOnActionListInCore);
	ButtonTurnOnRecipeListOnCore->OnClicked.AddDynamic(this, &UCoreControlPanel::TurnOnRecipeListInCore);
	ButtonTurnOnKnownObjectListOnCore->OnClicked.AddDynamic(this, &UCoreControlPanel::TurnOnKnownObjectListInCore);

	ButtonExit->OnClicked.AddDynamic(this, &UCoreControlPanel::ClickExitButton);


}

void UCoreControlPanel::ClickWorkerCreator()
{
	CloseHUD();
	CoreHUD->OpenWorkerCreator();
}

void UCoreControlPanel::PressedSinglePartOfHub(TObjectPtr<USinglePartOfHubWidget> Widget, ESinglePartOfHubType Type)
{
	if (SelectedHubButton)
	{
		SelectedHubButton->MakeUnActive();
		DiselectHub();
		HubImage->SetVisibility(ESlateVisibility::Hidden);
	}

	if (SelectedHubButton == Widget)
	{
		SelectedHubButton->MakeUnActive();
		SelectedHubButton = nullptr;
		SelectedHub = nullptr;
		return;
	}
	HubImage->SetVisibility(ESlateVisibility::Visible);
	SelectedHubButton = Widget;
	SelectedHub = AttachedHubs[SelectedHubButton];
	SelectedHubButton->MakeActive();
	SelectHub();
	UpdateHubInterface();
}

void UCoreControlPanel::ClickFreeSelectedHub()
{
	if (!SelectedHub)
		return;

	MainCore->FreeWorkerHub(SelectedHub);

	SelectedHubButton->MakeUnActive();
	SelectedHubButton = nullptr;
	SelectedHub = nullptr;
	HubImage->SetVisibility(ESlateVisibility::Hidden);
	UpdateInterface();
}

void UCoreControlPanel::AddNewAttachedHub(AWorkerHub* WHub)
{
	for (const auto& Button : AttachedHubs)
	{
		if (Button.Value == WHub)
			return;
	}

	TObjectPtr<USinglePartOfHubWidget> NewButton = CreateWidget<USinglePartOfHubWidget>(this->GetWorld(), SinglePartOfHubWidgetClass);
	NewButton->Init(WHub->GetHubName(), this, ESinglePartOfHubType::Hub, Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetWorkerHubIcon());
	AttachedHubToCore->AddChild(NewButton);
	AttachedHubs.Add(NewButton, WHub);
}

void UCoreControlPanel::ClearHud()
{
	AttachedHubs.Empty();
	AttachedHubToCore->ClearChildren();

	ClearCoreHud();

	SelectedHub = nullptr;
	SelectedHubButton = nullptr;
	ClearHubHud();
}

void UCoreControlPanel::ClearHubHud()
{
	WidgetsForHub.Empty();
	HubKnownObjectAndWidget.Empty();
	ActionsInHub->ClearChildren();
	RecipesInHub->ClearChildren();
	KnownObjectsInHub->ClearChildren();
	DiselectHub();
}

void UCoreControlPanel::ClearCoreHud()
{
	ActionsInCore->ClearChildren();
	RecipesInCore->ClearChildren();
	KnownObjectsInCore->ClearChildren();
	WidgetsForCore.Empty();
	CoreKnownObjectAndWidget.Empty();

}

void UCoreControlPanel::PressedSinglePartOfTreeElement(EActionTreeElementName Name, TObjectPtr<USinglePartOfTreeElementWidget> Widget)
{
	if (WidgetsForCore.Contains(Widget))
	{
		SelectTreeElementInCore(Name);
		return;
	}

	SelectTreeElementInHub(Name);
}

void UCoreControlPanel::SelectTreeElementInCore(EActionTreeElementName Name)
{
	if (!SelectedHub || !SelectedHubButton)
		return;

	if (SelectedHub->HasTreeElement(Name))
		return;

	if (!SelectedTreeElements.Contains(Name))
	{
		TArray<EActionTreeElementName> UnknownTreeElements = SelectedHub->GetAllUnknownTreeElementsListToTreeElement(Name);
		for (const auto& TreeElement : UnknownTreeElements)
		{
			if (!SelectedTreeElements.Contains(TreeElement))
			{
				SelectedTreeElements.Add(TreeElement);
			}
		}
	}
	else
	{
		SelectedHub->RemoveTreeElementsFromList(SelectedTreeElements, Name);
	}

	UpdateSelectedInfo();
}

void UCoreControlPanel::SelectTreeElementInHub(EActionTreeElementName Name)
{
	if (!SelectedHub || !SelectedHubButton)
		return;

	if (!SelectedTreeElementsInHub.Contains(Name))
	{
		TArray<EActionTreeElementName> AllDeletableTreeElements = SelectedHub->GetAllDeletableTreeElementListToTreeElement(Name);

		for (const auto& TreeElement : AllDeletableTreeElements)
		{
			if (!SelectedTreeElementsInHub.Contains(TreeElement))
			{
				SelectedTreeElementsInHub.Add(TreeElement);
			}
		}

	}
	else
	{
		SelectedHub->RemoveTreeElementsFromRemovableList(SelectedTreeElementsInHub, Name);
	}
	
	UpdateSelectedInfoInHub();
}

void UCoreControlPanel::UpdateSelectedInfo()
{
	for (auto& Action : ActionsInCore->GetAllChildren())
	{
		if (SelectedTreeElements.Contains(Cast<USinglePartOfTreeElementWidget>(Action)->GetTreeElementName()))
		{
			Cast<USinglePartOfTreeElementWidget>(Action)->MakeActive();
		}
		else
		{
			Cast<USinglePartOfTreeElementWidget>(Action)->MakeUnActive();
		}
	}

	for (auto& Recipe : RecipesInCore->GetAllChildren())
	{
		if (SelectedTreeElements.Contains(Cast<USinglePartOfTreeElementWidget>(Recipe)->GetTreeElementName()))
		{
			Cast<USinglePartOfTreeElementWidget>(Recipe)->MakeActive();
		}
		else
		{
			Cast<USinglePartOfTreeElementWidget>(Recipe)->MakeUnActive();
		}
	}

	if (SelectedTreeElements.Num() == 0 && SelectedKnownObjectInHub.Num() == 0)
	{
		CurrentMemorySizeSelectedInfo->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CurrentMemorySizeSelectedInfo->SetVisibility(ESlateVisibility::Visible);

		int32 Size = 0;
		for (const auto& TreeElement : SelectedTreeElements)
		{
			Size += Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetTreeElementSize(TreeElement);
		}

		for (const auto& KnownObj : SelectedKnownObjectInHub)
		{
			Size += KnownObj->GetSizeData();
		}

		CurrentMemorySizeSelectedInfo->SetText(FText::FromString("Selected size : " + FString::FromInt(Size) + " Mb"));
		CurrentSizeSelectedInfo = Size;
	}
}

void UCoreControlPanel::UpdateSelectedInfoInHub()
{
	for (auto& Action : ActionsInHub->GetAllChildren())
	{
		if (SelectedTreeElementsInHub.Contains(Cast<USinglePartOfTreeElementWidget>(Action)->GetTreeElementName()))
		{
			Cast<USinglePartOfTreeElementWidget>(Action)->MakeActive();
		}
		else
		{
			Cast<USinglePartOfTreeElementWidget>(Action)->MakeUnActive();
		}
	}

	for (auto& Recipe : RecipesInHub->GetAllChildren())
	{
		if (SelectedTreeElementsInHub.Contains(Cast<USinglePartOfTreeElementWidget>(Recipe)->GetTreeElementName()))
		{
			Cast<USinglePartOfTreeElementWidget>(Recipe)->MakeActive();
		}
		else
		{
			Cast<USinglePartOfTreeElementWidget>(Recipe)->MakeUnActive();
		}
	}


	if (SelectedTreeElementsInHub.Num() == 0 && SelectedKnownObjectInHub.Num() == 0)
	{
		CurrentMemorySizeSelectedInfoInHub->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		CurrentMemorySizeSelectedInfoInHub->SetVisibility(ESlateVisibility::Visible);

		int32 Size = 0;
		for (const auto& TreeElement : SelectedTreeElementsInHub)
		{
			Size += Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetTreeElementSize(TreeElement);
		}

		for (const auto& KnownObj : SelectedKnownObjectInHub)
		{
			Size += KnownObj->GetSizeData();
		}

		CurrentMemorySizeSelectedInfoInHub->SetText(FText::FromString("Selected size : " + FString::FromInt(Size) + " Mb"));
		CurrentSizeSelectedInfoInHub = Size;
	}
}

void UCoreControlPanel::SelectHub()
{
	if (!SelectedHub || !SelectedHubButton)
		return;

	MaxMemorySizeForSelectedHub->SetVisibility(ESlateVisibility::Visible);
	CurrentMemorySizeForSelectedHub->SetVisibility(ESlateVisibility::Visible);
	
	CurrentMemorySizeHub = SelectedHub->GetHubMemotySize();
	MaxMemorySizeForSelectedHub->SetText(FText::FromString(FString::FromInt(SelectedHub->GetMaxHubMemotySize())));
	CurrentMemorySizeForSelectedHub->SetText(FText::FromString(FString::FromInt(CurrentMemorySizeHub) + "/"));
}

void UCoreControlPanel::DiselectHub()
{
	MaxMemorySizeForSelectedHub->SetVisibility(ESlateVisibility::Hidden);
	CurrentMemorySizeForSelectedHub->SetVisibility(ESlateVisibility::Hidden);

	SelectedTreeElements.Empty();
	UpdateSelectedInfo();

	SelectedTreeElementsInHub.Empty();
	UpdateSelectedInfoInHub();
}

void UCoreControlPanel::UpdateHubInterface()
{
	if (!SelectedHub || !SelectedHubButton)
		return;

	ClearHubHud();

	for (const auto& Action : SelectedHub->GetActionList())
	{
		EActionTreeElementName TreeElementName = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetActionTreeElementNameForActionName(Action);

		if (TreeElementName == EActionTreeElementName::Invalid)
		{
			continue;
		}

		TObjectPtr<USinglePartOfTreeElementWidget> NewButton = CreateWidget<USinglePartOfTreeElementWidget>(this->GetWorld(), SinglePartOfTreeElementWidgetClass);

		const FString* ActionName = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetActionName(Action);

		if (!ActionName)
		{
			NewButton->Init(TreeElementName, "!!!MISSING NAME!!!", this);
		}
		else
		{
			NewButton->Init(TreeElementName, *ActionName, this);
		}

		ActionsInHub->AddChild(NewButton);
		WidgetsForHub.Add(NewButton);
	}

	for (const auto& Recipe : SelectedHub->GetRecipeList())
	{
		EActionTreeElementName TreeElementName = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetActionTreeElementNameForRecipeName(Recipe);

		if (TreeElementName == EActionTreeElementName::Invalid)
		{
			continue;
		}

		TObjectPtr<USinglePartOfTreeElementWidget> NewButton = CreateWidget<USinglePartOfTreeElementWidget>(this->GetWorld(), SinglePartOfTreeElementWidgetClass);

		const FString* RecipeName = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetRecipeName(Recipe);
		if (!RecipeName)
		{
			NewButton->Init(TreeElementName, "!!!MISSING NAME!!!", this);

		}
		else
		{
			NewButton->Init(TreeElementName, *RecipeName, this);

		}
		RecipesInHub->AddChild(NewButton);
		WidgetsForHub.Add(NewButton);
	}

	for (const auto& KnownObject : SelectedHub->GetKnownObject())
	{
		TObjectPtr<UKnownObjectInfoWidget> NewButton = CreateWidget<UKnownObjectInfoWidget>(this->GetWorld(), KnownObjectInfoWidgetClass);

		const FString* ObjectName = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetInteractableActorName(KnownObject->GetActorType());

		if (!ObjectName)
		{
			NewButton->Init("!!!MISSING NAME!!!", KnownObject->GetKnownObjectLocation(), this);

		}
		else
		{
			NewButton->Init(*ObjectName, KnownObject->GetKnownObjectLocation(), this);

		}
		KnownObjectsInHub->AddChild(NewButton);
		HubKnownObjectAndWidget.Add(NewButton, KnownObject);
	}
	SelectHub();
}

void UCoreControlPanel::ClickAddSelectedInfoToHub()
{
	if (!SelectedHub || !SelectedHubButton)
		return;

	if (CurrentMemorySizeHub < CurrentSizeSelectedInfo)
		return;

	if (!SelectedHub->AddNewTreeElementListToMemory(SelectedTreeElements))
		return;

	for (const auto& Obj : SelectedKnownObjectInCore)
	{
		SelectedHub->AddNewObjectInfo(Obj);
		for (auto& Widget : CoreKnownObjectAndWidget)
		{
			if (Widget.Value == Obj)
			{
				Widget.Key->MakeUnActive();
				break;
			}
		}
	}
	SelectedKnownObjectInCore.Empty();

	ClearHubHud();
	UpdateHubInterface();

	SelectedTreeElements.Empty();
	SelectedKnownObjectInCore.Empty();
	UpdateSelectedInfo();
}

void UCoreControlPanel::ClickRemoveSelectedInfoFromHub()
{
	if (!SelectedHub || !SelectedHubButton)
		return;

	SelectedHub->RemoveTreeElementListFromMemory(SelectedTreeElementsInHub);

	for (const auto& Obj : SelectedKnownObjectInHub)
	{
		SelectedHub->RemoveKnownObjectInfo(Obj);
	}

	SelectedKnownObjectInHub.Empty();

	ClearHubHud();
	UpdateHubInterface();
	SelectedTreeElementsInHub.Empty();
	SelectedKnownObjectInHub.Empty();
	UpdateSelectedInfoInHub();
}

void UCoreControlPanel::PressKnownObject(TObjectPtr<UKnownObjectInfoWidget> ObjectWidget)
{
	if (CoreKnownObjectAndWidget.Contains(ObjectWidget))
	{
		SelectKnownObjectInCore(ObjectWidget, CoreKnownObjectAndWidget[ObjectWidget]);
		return;
	}

	if (HubKnownObjectAndWidget.Contains(ObjectWidget))
	{
		SelectKnownObjectInHub(ObjectWidget, HubKnownObjectAndWidget[ObjectWidget]);
	}
}

void UCoreControlPanel::SelectKnownObjectInCore(TObjectPtr<UKnownObjectInfoWidget> Widget, TObjectPtr<UKnownObjectInfo> Object)
{
	if (!SelectedHub || !SelectedHubButton)
		return;

	if (SelectedHub->IsKnownObject(Object->GetInteractableActor()))
		return;

	if (SelectedKnownObjectInCore.Contains(Object))
	{
		SelectedKnownObjectInCore.Remove(Object);
		Widget->MakeUnActive();
		CurrentSizeSelectedInfo -= Object->GetSizeData();
	}
	else
	{
		SelectedKnownObjectInCore.Add(Object);
		Widget->MakeActive();
		CurrentSizeSelectedInfo += Object->GetSizeData();
	}
	UpdateSelectedInfo();

}

void UCoreControlPanel::SelectKnownObjectInHub(TObjectPtr<UKnownObjectInfoWidget> Widget, TObjectPtr<UKnownObjectInfo> Object)
{
	if (SelectedKnownObjectInHub.Contains(Object))
	{
		SelectedKnownObjectInHub.Remove(Object);
		Widget->MakeUnActive();
		CurrentSizeSelectedInfoInHub -= Object->GetSizeData();

	}
	else
	{
		SelectedKnownObjectInHub.Add(Object);
		Widget->MakeActive();
		CurrentSizeSelectedInfoInHub += Object->GetSizeData();
	}
	UpdateSelectedInfoInHub();
}

void UCoreControlPanel::TurnOnActionListInHub()
{
	HubInfoSwitcher->SetActiveWidget(ActionsInHub);
}

void UCoreControlPanel::TurnOnRecipeListInHub()
{
	HubInfoSwitcher->SetActiveWidget(RecipesInHub);
}

void UCoreControlPanel::TurnOnKnownObjectListInHub()
{
	HubInfoSwitcher->SetActiveWidget(KnownObjectsInHub);
}

void UCoreControlPanel::TurnOnActionListInCore()
{
	CoreInfoSwitcher->SetActiveWidget(ActionsInCore);
}

void UCoreControlPanel::TurnOnRecipeListInCore()
{
	CoreInfoSwitcher->SetActiveWidget(RecipesInCore);
}

void UCoreControlPanel::TurnOnKnownObjectListInCore()
{
	CoreInfoSwitcher->SetActiveWidget(KnownObjectsInCore);
}