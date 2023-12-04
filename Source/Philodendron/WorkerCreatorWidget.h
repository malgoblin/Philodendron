// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserHUDInterface.h"
#include "SinglePartOfHubWidget.h"
#include "SinglePartOfHubListener.h"
#include "SinglePartOfHubImageListener.h"
#include "EnumSinglePartOfHubType.h"
#include "WorkerCreatorWidget.generated.h"

class UScrollBox;
class UButton;
class AMainPlayer;
class UImage;
class UCanvasPanel;
class ACoreHUD;
class AWorkerPawn;
class AWorkerHub;
class AWorkerAddon;
class UWidgetSwitcher;

class USinglePartOfHubImageWidget;
class UReadOnlyWidget;

enum class EButtonType;
enum class EAddonPossitionType;
enum class EWorkerType;

UCLASS()
class PHILODENDRON_API UWorkerCreatorWidget : public UUserWidget, public IUserHUDInterface, public ISinglePartOfHubListener, public ISinglePartOfHubImageListener
{
	GENERATED_BODY()
public:
	UWorkerCreatorWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void InitHUD(TObjectPtr<AMainPlayer> Core, TObjectPtr<ACoreHUD> CHUD);
	void OpenCreator();
	virtual void CloseHUD() override;

	virtual void PressedSinglePartOfHub(TObjectPtr<USinglePartOfHubWidget> Widget, ESinglePartOfHubType Type) override;
	virtual void PressedSinglePartOfHubImage(TObjectPtr<USinglePartOfHubImageWidget> Widget, ESinglePartOfHubType Type, ESinglePartOfHubPossitionType PossitionType) override;

private:
	void UpdateHUD();
	void ResetHUD();

	void ResetHub();

	UFUNCTION()
	void ClickOnAddNewWorker();

	UFUNCTION()
	void ClickOnCreateWorker();

	UFUNCTION()
	void ClickOnTakeApartWorker();

	UFUNCTION()
	void ClickTurnOnPawnList();

	UFUNCTION()
	void ClickTurnOnAddonList();

	UFUNCTION()
	void ClickCloseButton();

	FString InitDefaultHubName();

	void SelectHub(TObjectPtr<USinglePartOfHubWidget> Widget);

	void SelectPawnPossition(TObjectPtr<USinglePartOfHubImageWidget> Widget);
	void SelectAddonPossition(TObjectPtr<USinglePartOfHubImageWidget> Widget);

	void UpdateAttachedPawnList();
	void UpdateExistedAddonList();
	void UpdateAttachedHubList();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UCanvasPanel> HubMainCanvas;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UScrollBox> AttachedWorkerPawnToCore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UScrollBox> AttachedWorkerHubToCore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UScrollBox> ExistedAddonsInCore;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> AddNewWorker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> CreateWorker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> TakeApartWorker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UWidgetSwitcher> SinglePartSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> ButtonClose;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> TurnOnPawnList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UButton> TurnOnAddonList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<USinglePartOfHubImageWidget> HubImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<USinglePartOfHubImageWidget> TopPawnImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<USinglePartOfHubImageWidget> LeftPawnImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<USinglePartOfHubImageWidget> RightPawnImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<USinglePartOfHubImageWidget> RightAddon0Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<USinglePartOfHubImageWidget> RightAddon1Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<USinglePartOfHubImageWidget> LeftAddon0Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<USinglePartOfHubImageWidget> LeftAddon1Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<USinglePartOfHubImageWidget> TopAddon0Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<USinglePartOfHubImageWidget> TopAddon1Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UReadOnlyWidget> TopPawnReadOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UReadOnlyWidget> LeftPawnReadOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bindwidget))
	TObjectPtr<UReadOnlyWidget> RightPawnReadOnly;


	UPROPERTY(EditAnywhere)
	TSubclassOf<USinglePartOfHubWidget> SinglePartOfHubWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<AMainPlayer> MainCore;

	UPROPERTY()
	TObjectPtr<ACoreHUD> CoreHUD;
	
	UPROPERTY()
	TObjectPtr<AWorkerHub> SelectedHub = nullptr;

	UPROPERTY()
	TMap<TObjectPtr<USinglePartOfHubWidget>, TObjectPtr<AWorkerPawn>> AttachedPawns;

	UPROPERTY()
	TMap<TObjectPtr<USinglePartOfHubWidget>, TObjectPtr<AWorkerHub>> AttachedHubs;

	UPROPERTY()
	TMap<TObjectPtr<USinglePartOfHubWidget>, TObjectPtr<AWorkerAddon>> ExistedAddons;

	UPROPERTY()
	TObjectPtr<USinglePartOfHubWidget> SelectedPartOfHubWidget = nullptr;

	UPROPERTY()
	TObjectPtr<USinglePartOfHubWidget> SelectedHubWidget = nullptr;

	UPROPERTY()
	TMap<TObjectPtr<AWorkerPawn>, TObjectPtr<USinglePartOfHubImageWidget>> SelectedPawns;

	UPROPERTY()
	TMap<TObjectPtr<AWorkerAddon>, TObjectPtr<USinglePartOfHubImageWidget>> SelectedAddons;

	UPROPERTY()
	TMap<EWorkerType, TObjectPtr<UReadOnlyWidget>> ReadOnlyList;

	UPROPERTY()
	TMap<EWorkerType, TObjectPtr<USinglePartOfHubImageWidget>> PawnPossitionList;

	UPROPERTY()
	TArray<TObjectPtr<USinglePartOfHubImageWidget>> AddonPossitionList;
};
