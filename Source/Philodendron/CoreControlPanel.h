// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserHUDInterface.h"
#include "SinglePartOfHubListener.h"
#include "SinglePartOfTreeElementWidget.h"
#include "SinglePartOfTreeElementListener.h"
#include "KnownObjectInfoWidget.h"
#include "KnownObjectInfoListener.h"
#include "SinglePartOfHubWidget.h"
#include "EnumSinglePartOfHubType.h"
#include "CoreControlPanel.generated.h"

class AWorkerPawn;
class AWorkerHub;
class AMainPlayer;
class ACoreHUD;
class UButton;
class UScrollBox;
class UCanvasPanel;
class UTextBlock;
class UKnownObjectInfo;
class UWidgetSwitcher;
class UImage;

UCLASS()
class PHILODENDRON_API UCoreControlPanel : public UUserWidget, public ISinglePartOfTreeElementListener, public IKnownObjectInfoListener, public ISinglePartOfHubListener
{
	GENERATED_BODY()
public:
	UCoreControlPanel(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void CloseHUD();

	void OpenMenu();
	void InitHUD(TObjectPtr<AMainPlayer> Core, TObjectPtr<ACoreHUD> CHUD);

	virtual void PressedSinglePartOfHub(TObjectPtr<USinglePartOfHubWidget> Widget, ESinglePartOfHubType Type) override;

	
	void AddNewAttachedHub(AWorkerHub* WHub);
	void UpdateInterface();

	virtual void PressedSinglePartOfTreeElement(EActionTreeElementName Name, TObjectPtr<USinglePartOfTreeElementWidget> Widget) override;
	virtual void PressKnownObject(TObjectPtr<UKnownObjectInfoWidget> ObjectWidget) override;


private:
	UFUNCTION()
	void ClickFreeSelectedHub();

	UFUNCTION()
	void ClickWorkerCreator();

	UFUNCTION()
	void ClickAddSelectedInfoToHub();

	UFUNCTION()
	void ClickRemoveSelectedInfoFromHub();


	UFUNCTION()
	void TurnOnActionListInHub();

	UFUNCTION()
	void TurnOnRecipeListInHub();

	UFUNCTION()
	void TurnOnKnownObjectListInHub();

	UFUNCTION()
	void TurnOnActionListInCore();

	UFUNCTION()
	void TurnOnRecipeListInCore();

	UFUNCTION()
	void TurnOnKnownObjectListInCore();

	UFUNCTION()
	void ClickExitButton();

	void ClearHud();

	void ClearHubHud();

	void ClearCoreHud();

	void UpdateSelectedInfo();

	void UpdateSelectedInfoInHub();

	void SelectHub();

	void DiselectHub();

	void UpdateHubInterface();

	void SelectTreeElementInCore(EActionTreeElementName Name);

	void SelectTreeElementInHub(EActionTreeElementName Name);

	void SelectKnownObjectInCore(TObjectPtr<UKnownObjectInfoWidget> Widget, TObjectPtr<UKnownObjectInfo> Object);
	void SelectKnownObjectInHub(TObjectPtr<UKnownObjectInfoWidget> Widget, TObjectPtr<UKnownObjectInfo> Object);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UCanvasPanel> MainCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UScrollBox> AttachedHubToCore;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> FreeSelectedHub;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> WorkerCreateor;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UScrollBox> ActionsInCore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UScrollBox> RecipesInCore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UScrollBox> KnownObjectsInCore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> AddSelectedInfoToHub;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UTextBlock> MaxMemorySizeForSelectedHub;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UTextBlock> CurrentMemorySizeForSelectedHub;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UTextBlock> CurrentMemorySizeSelectedInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UTextBlock> CurrentMemorySizeSelectedInfoInHub;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> RemoveSelectedInfoFromHub;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UScrollBox> ActionsInHub;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UScrollBox> RecipesInHub;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UScrollBox> KnownObjectsInHub;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> ButtonTurnOnActionListOnHub;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> ButtonTurnOnRecipeListOnHub;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> ButtonTurnOnKnownObjectListOnHub;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> ButtonTurnOnActionListOnCore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> ButtonTurnOnRecipeListOnCore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> ButtonTurnOnKnownObjectListOnCore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> ButtonExit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UWidgetSwitcher> HubInfoSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UWidgetSwitcher> CoreInfoSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UImage> HubImage;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USinglePartOfTreeElementWidget> SinglePartOfTreeElementWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USinglePartOfHubWidget> SinglePartOfHubWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UKnownObjectInfoWidget> KnownObjectInfoWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<AMainPlayer> MainCore = nullptr;

	UPROPERTY()
	TMap<TObjectPtr<USinglePartOfHubWidget>, TObjectPtr<AWorkerHub>> AttachedHubs;

	UPROPERTY()
	TObjectPtr<ACoreHUD> CoreHUD = nullptr;

	UPROPERTY()
	TObjectPtr<AWorkerHub> SelectedHub = nullptr;

	UPROPERTY()
	TObjectPtr<USinglePartOfHubWidget> SelectedHubButton = nullptr;

	UPROPERTY()
	TArray<EActionTreeElementName> SelectedTreeElements;
	
	UPROPERTY()
	TArray<EActionTreeElementName> SelectedTreeElementsInHub;
	
	UPROPERTY()
	TArray<TObjectPtr<USinglePartOfTreeElementWidget>> WidgetsForCore;
	
	UPROPERTY()
	TArray<TObjectPtr<USinglePartOfTreeElementWidget>> WidgetsForHub;

	UPROPERTY()
	TMap<TObjectPtr<UKnownObjectInfoWidget>, TObjectPtr<UKnownObjectInfo>> CoreKnownObjectAndWidget;

	UPROPERTY()
	TMap<TObjectPtr<UKnownObjectInfoWidget>, TObjectPtr<UKnownObjectInfo>> HubKnownObjectAndWidget;

	UPROPERTY()
	TArray<TObjectPtr<UKnownObjectInfo>> SelectedKnownObjectInHub;

	UPROPERTY()
	TArray<TObjectPtr<UKnownObjectInfo>> SelectedKnownObjectInCore;

	UPROPERTY()
	int32 CurrentMemorySizeHub = 0;
	
	UPROPERTY()
	int32 CurrentSizeSelectedInfo = 0;
	
	UPROPERTY()
	int32 CurrentSizeSelectedInfoInHub = 0;
};
