#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "WorkerPawn.h"
#include "WorkerHub.h"
#include "WorkerAddon.h"
#include "CoreLog.h"
#include "MainPlayer.generated.h"

class UPaperFlipbook;

enum class EActionType;
enum class ERecipeType;
class UKnownObjectInfo;
class UWorkerMap;

class UCameraComponent;
class ACoreHUD;
class USceneCaptureComponent2D;
class UPaperFlipbookComponent;
class UBoxComponent;

struct CameraDefaultsSettings
{
	static constexpr float CameraSpeed = 3.0f;
	static constexpr float CameraRotation = -30.0f;
};

UCLASS()
class PHILODENDRON_API AMainPlayer : public APawn
{
	GENERATED_BODY()

public:
	AMainPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void InitCore(TObjectPtr<UPaperFlipbook> CFlipbook);

	void Move_XAxis(float AxisValue);
	void Move_YAxis(float AxisValue);
	void Move_ZAxis(float AxisValue);
	void CreatePointOfInterest();
	void OpenCloseCoreMenu();
	void PauseContinue();
	void MainMenu();


	void PauseGame();
	void ContinueGame();
	void SetMainMenuStatus(bool OpenMainMenu);


	const TArray<TObjectPtr<AWorkerPawn>>& GetAttachedWorkers() const;
	const TArray<TObjectPtr<AWorkerHub>>& GetAttachedWorkerHubs() const;
	const TArray<TObjectPtr<AWorkerAddon>>& GetExistedAddons() const;

	bool AttachWorkerToCore(TObjectPtr<AWorkerPawn> WPawn);
	bool AttachHub(TObjectPtr<AWorkerHub> WHub);

	FVector GetLocation() const;
	UPaperFlipbook* GetIActorPaperFlipbook() const;


	const TArray<EActionType>& GetKnownActionList() const;
	const TArray<ERecipeType>& GetKnownRecipeList() const;
	const TArray<TObjectPtr<UKnownObjectInfo>> GetKnownObject() const;

	void SetStartedAddons(const TArray<TObjectPtr<AWorkerAddon>>& StartedAddons);

	void RemoveHub(TObjectPtr<AWorkerHub>& WHub);

	void TurnOffControls();
	void TurnOnControls();

	const TArray<FString>* GetCoreLog() const;

	int32 GetHubNumber() const;

	void CreatedStartedWorkers(int32 WorkerCount);

	TObjectPtr<AWorkerHub> CreateTempHub(const FString& HubName);


	TObjectPtr<AWorkerHub> LoadHub(FVector&& Location);

	bool CreateNewHubFromTemp(const TMap<EWorkerType, TObjectPtr<AWorkerPawn>>& PawnMap, const TMap<TObjectPtr<AWorkerPawn>, bool>& PawnMapReadOnly, const TMap<TObjectPtr<AWorkerPawn>, TMap<EAddonPossitionType, TObjectPtr<AWorkerAddon>>>& AddonMap);
	bool UpdateExistedHub(TObjectPtr<AWorkerHub> WHub, const TMap<EWorkerType, TObjectPtr<AWorkerPawn>>& PawnMap, const TMap<TObjectPtr<AWorkerPawn>, bool>& PawnMapReadOnly, const TMap<TObjectPtr<AWorkerPawn>, TMap<EAddonPossitionType, TObjectPtr<AWorkerAddon>>>& AddonMap);

	void FreeWorkerHub(TObjectPtr<AWorkerHub>& SelectedWorkerHub);
	
	bool AddAddonsToCore(TObjectPtr<AWorkerAddon>& NewAddon);

	bool HasAddon(const TObjectPtr<AWorkerAddon>& NewAddon) const;

	const TObjectPtr<UPaperFlipbook> GetCoreFlipbook() const;

	void AddNewAction(EActionType AType);
	void AddNewRecipe(ERecipeType RType);
	void AddNewKnownObjectInfo(const TObjectPtr<UKnownObjectInfo>& Info);

	const TArray<TObjectPtr<AWorkerPawn>>& GetAllPawns() const;

	TObjectPtr<AWorkerPawn> GenerateNewPawn();

	const TArray<TObjectPtr<UWorkerMap>>& GetCreatedWorkerMap() const;

	TObjectPtr<UWorkerMap> CreateNewMap(const TArray<TObjectPtr<UKnownObjectInfo>>& ObjList);

	void SetIDForMap(int32 ID);
	void SetIDForHub(int32 ID);

	int32 GetIDForMap() const;
	int32 GetIDForHub() const;

	const TArray<TObjectPtr<AWorkerHub>>& GetExistedWorkerHubs() const;
	const TArray<TObjectPtr<AWorkerPawn>>& GetExistedPawn() const;

protected:
	virtual void BeginPlay() override;

private:
	bool UpdateHub(TObjectPtr<AWorkerHub>& WHub, const TMap<EWorkerType, TObjectPtr<AWorkerPawn>>& PawnMap, const TMap<TObjectPtr<AWorkerPawn>, bool>& PawnMapReadOnly, const TMap<TObjectPtr<AWorkerPawn>, TMap<EAddonPossitionType, TObjectPtr<AWorkerAddon>>>& AddonMap);
	void CreateNewWorkerPawn(const FString& Name);
	void AddNewInfo(const TObjectPtr<UKnownObjectInfo>& Info);
	TObjectPtr<AWorkerHub> GenerateHubObject(FVector&& Location);

private:
	UPROPERTY()
	FVector CurrentVelocity = FVector::ZeroVector;

	UPROPERTY()
	int32 MinimalFoVCamera = 0;
	
	UPROPERTY()
	int32 MaxFoVCamera = 0;

	UPROPERTY()
	TArray<EActionType> AvailableAction;

	UPROPERTY()
	TArray<EActionType> ExistedActions;
	
	UPROPERTY()
	TArray<TObjectPtr<UKnownObjectInfo>> ObjectList;

	UPROPERTY()
	TArray<ERecipeType> ExistedRecipes;
	
	UPROPERTY()
	TArray<TObjectPtr<AWorkerPawn>> ExistedWorkers;

	UPROPERTY()
	TArray<TObjectPtr<AWorkerHub>> ExistedWorkerHubs;

	UPROPERTY()
	TArray<TObjectPtr<AWorkerPawn>> AttachedWorkers;

	UPROPERTY()
	TArray<TObjectPtr<AWorkerHub>> AttachedWorkerHubs;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ACoreHUD> CoreHUD = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UWorkerMap>> CreatedWorkerMap;

	UPROPERTY()
	TArray<TObjectPtr<AWorkerAddon>> AddonsInCore;

	UPROPERTY()
	TSubclassOf<AWorkerPawn> AWorkerPawnClass = AWorkerPawn::StaticClass();
	
	UPROPERTY()
	TSubclassOf<AWorkerHub> AWorkerHubClass = AWorkerHub::StaticClass();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> GameCamera = nullptr;

	UPROPERTY(EditAnywhere)
	float CameraSpeed = CameraDefaultsSettings::CameraSpeed;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> CoreFlipbook = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbookComponent> FlipbookComponent = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollisionComponent = nullptr;

	UPROPERTY()
	bool ControlsMustWork = true;

	UCoreLog CoreLog;

	UPROPERTY()
	TObjectPtr<AWorkerHub> TempHub = nullptr;

	UPROPERTY()
	bool bMainMenuOpen = false;
	
	UPROPERTY()
	int32 IDForWorkerMap = 0;
	
	UPROPERTY()
	int32 IDForHub = 0;
};
