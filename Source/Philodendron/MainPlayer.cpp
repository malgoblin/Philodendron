#include "MainPlayer.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Components/BoxComponent.h"
#include "MainGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "CoreHUD.h"
#include "MainAIController.h"
#include "InteractableActor.h"

#include "EnumWorkerAddonType.h"
#include "GameFramework/WorldSettings.h"

#include "WorkerMap.h"
#include "NavAreas/NavArea_Null.h"
#include "EnumAnimalType.h"
#include "Components/SceneCaptureComponent2D.h"

#include "EnumActionList.h"
#include "EnumRecipeType.h"
#include "KnownObjectInfo.h"
#include "WorkerMap.h"
#include "ActionTreeElement.h"
#include "ActionTree.h"

AMainPlayer::AMainPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CoreFlipbookComponent"));
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CoreCollision"));


	FlipbookComponent->SetupAttachment(RootComponent);

	BoxCollisionComponent->SetupAttachment(RootComponent);

	BoxCollisionComponent->bDynamicObstacle = true;

	BoxCollisionComponent->AreaClass = UNavArea_Null::StaticClass();

	GameCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	GameCamera->SetupAttachment(RootComponent);
	GameCamera->SetRelativeLocation(FVector(45.f, 3365.f, 2000.f));
	GameCamera->SetRelativeRotation(FRotator(CameraDefaultsSettings::CameraRotation, -90.0f, 0.0f));
	GameCamera->ProjectionMode = ECameraProjectionMode::Perspective;
	GameCamera->FieldOfView = 25;
	//GameCamera->OrthoWidth = 1400;

}

void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AMainPlayer::InitCore(TObjectPtr<UPaperFlipbook> CFlipbook)
{
	CoreFlipbook = CFlipbook;
	FlipbookComponent->SetFlipbook(CoreFlipbook);
	GameCamera->Activate();
	FlipbookComponent->CastShadow = true;

	ExistedActions = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetKnownActionInCore();

	const TArray<EActionType>* TempActionList = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetAvailableActions(EInteractableActorType::Core);
	if (TempActionList)
	{
		AvailableAction = *TempActionList;
	}

	CoreHUD = Cast<ACoreHUD>(this->GetWorld()->GetFirstPlayerController()->GetHUD());
	CoreHUD->RegisterCore(this);

	MinimalFoVCamera = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetMinimalFoVCamera();
	MaxFoVCamera = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetMaxFoVCamera();
}

void AMainPlayer::SetStartedAddons(const TArray<TObjectPtr<AWorkerAddon>>& StartedAddons)
{
	AddonsInCore = StartedAddons;
}

void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("MoveX", this, &AMainPlayer::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &AMainPlayer::Move_YAxis);
	InputComponent->BindAxis("MoveZ", this, &AMainPlayer::Move_ZAxis);
	InputComponent->BindAction("CreatePointOfInterest", IE_Released, this, &AMainPlayer::CreatePointOfInterest);
	InputComponent->BindAction("OpenCloseCoreMenu", IE_Released, this, &AMainPlayer::OpenCloseCoreMenu);
	InputComponent->BindAction("Pause/Continue", IE_Released, this, &AMainPlayer::PauseContinue);
	InputComponent->BindAction("MainMenu", IE_Released, this, &AMainPlayer::MainMenu);
}

void AMainPlayer::Move_XAxis(float AxisValue)
{
	if (!ControlsMustWork)
	{
		return;
	}

	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * CameraSpeed;
	FVector NewLocation = GameCamera->GetRelativeLocation() + (CurrentVelocity);
	GameCamera->SetRelativeLocation(NewLocation);
}

void AMainPlayer::Move_YAxis(float AxisValue)
{
	if (!ControlsMustWork)
	{
		return;
	}

	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * CameraSpeed;
	FVector NewLocation = GameCamera->GetRelativeLocation() + (CurrentVelocity);
	GameCamera->SetRelativeLocation(NewLocation);
}

void AMainPlayer::Move_ZAxis(float AxisValue)
{
	if (!ControlsMustWork)
	{
		return;
	}

	float NewFoV = GameCamera->FieldOfView + FMath::Clamp(AxisValue, -1.0f, 1.0f) * CameraSpeed;

	if (NewFoV > MaxFoVCamera || NewFoV < MinimalFoVCamera)
		return;

	GameCamera->FieldOfView = NewFoV;
}

bool AMainPlayer::AddAddonsToCore(TObjectPtr<AWorkerAddon>& NewAddon)
{
	NewAddon->SetActorLocation(this->GetActorLocation());
	NewAddon->SetHidden(true);

	AddonsInCore.Add(NewAddon);

	return true;
}

bool AMainPlayer::HasAddon(const TObjectPtr<AWorkerAddon>& NewAddon) const
{
	return AddonsInCore.Contains(NewAddon);
}

void AMainPlayer::CreatePointOfInterest()
{
	if (!ControlsMustWork)
	{
		return;
	}

	FHitResult HitResult;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this->GetWorld(), 0);
	PlayerController->GetHitResultUnderCursor(ECC_Pawn, true, HitResult);

	//PlayerController = UGameplayStatics::GetPlayerController(this->GetWorld(), 0);
	//PlayerController->GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	FVector location = HitResult.Location;
	FRotator rotation = { 0, 0, 0 };

}

void AMainPlayer::PauseContinue()
{
	if (!ControlsMustWork)
	{
		return;
	}

	if (this->GetWorldSettings()->TimeDilation == 1.0f)
	{
		PauseGame();
	}
	else
	{
		ContinueGame();
	}
}

void AMainPlayer::PauseGame()
{
	this->GetWorldSettings()->SetTimeDilation(0);
}

void AMainPlayer::ContinueGame()
{
	this->GetWorldSettings()->SetTimeDilation(1);
}

void AMainPlayer::CreatedStartedWorkers(int32 WorkerCount)
{
	for (int32 i = 0; i < WorkerCount; ++i)
	{
		CreateNewWorkerPawn("WorkerPawn_" + FString::FromInt(i));
	}
}
#pragma optimize("", off)
void AMainPlayer::CreateNewWorkerPawn(const FString& Name)
{
	TObjectPtr<AWorkerPawn> NewWorker = GenerateNewPawn();

	NewWorker->SetWorkerName(Name);
	NewWorker->SetPawnID(ExistedWorkers.Num());
	AttachWorkerToCore(NewWorker);
}
#pragma optimize("", on)

TObjectPtr<AWorkerPawn> AMainPlayer::GenerateNewPawn()
{
	AWorkerPawn* NewWorker = this->GetWorld()->SpawnActor<AWorkerPawn>(AWorkerPawnClass, this->GetLocation(), { 0, 0, 0 });

	NewWorker->InitWorker();

	ExistedWorkers.Add(NewWorker);

	return NewWorker;
}

const TArray<TObjectPtr<AWorkerPawn>>& AMainPlayer::GetExistedPawn() const
{
	return ExistedWorkers;
}

bool AMainPlayer::CreateNewHubFromTemp(const TMap<EWorkerType, TObjectPtr<AWorkerPawn>>& PawnMap, const TMap<TObjectPtr<AWorkerPawn>, bool>& PawnMapReadOnly, const TMap<TObjectPtr<AWorkerPawn>, TMap<EAddonPossitionType, TObjectPtr<AWorkerAddon>>>& AddonMap)
{
	if (!TempHub)
	{
		return false;
	}

	if (!UpdateHub(TempHub, PawnMap, PawnMapReadOnly, AddonMap))
	{
		TempHub = nullptr;
		return false;
	}

	TempHub->SetHubID(ExistedWorkerHubs.Num());

	ExistedWorkerHubs.Add(TempHub);
	AttachedWorkerHubs.Add(TempHub);
	TempHub->SetHubID(IDForHub);
	IDForHub += 1;

	TempHub = nullptr;
	return true;
}

bool AMainPlayer::UpdateExistedHub(TObjectPtr<AWorkerHub> WHub, const TMap<EWorkerType, TObjectPtr<AWorkerPawn>>& PawnMap, const TMap<TObjectPtr<AWorkerPawn>, bool>& PawnMapReadOnly, const TMap<TObjectPtr<AWorkerPawn>, TMap<EAddonPossitionType, TObjectPtr<AWorkerAddon>>>& AddonMap)
{
	if (!WHub)
	{
		return false;
	}

	if (!UpdateHub(WHub, PawnMap, PawnMapReadOnly, AddonMap))
		return false;

	return true;
}


bool AMainPlayer::UpdateHub(TObjectPtr<AWorkerHub>& WHub, const TMap<EWorkerType, TObjectPtr<AWorkerPawn>>& PawnMap, const TMap<TObjectPtr<AWorkerPawn>, bool>& PawnMapReadOnly, const TMap<TObjectPtr<AWorkerPawn>, TMap<EAddonPossitionType, TObjectPtr<AWorkerAddon>>>& AddonMap)
{
	for (const auto& Pawn : PawnMap)
	{
		if (!Pawn.Value)
		{
			continue;
		}

		Pawn.Value->SetReadOnly(PawnMapReadOnly[Pawn.Value]);

		if (WHub->HasPawn(Pawn.Value))
			continue;

		if (!WHub->AttachWorker(Pawn.Value, Pawn.Key))
		{
			return false;
		}
		AttachedWorkers.Remove(Pawn.Value);
	}

	for (const auto& PawnAddon : AddonMap)
	{
		PawnAddon.Key->GiveAllAddonsToCore();

		for (const auto& Addon : PawnAddon.Value)
		{
			if (!Addon.Value)
			{
				continue;
			}

			if (!PawnAddon.Key->AddNewAddon(Addon.Value, Addon.Key))
			{
				return false;
			}
			AddonsInCore.Remove(Addon.Value);
		}
	}
	return true;
}

TObjectPtr<AWorkerHub> AMainPlayer::CreateTempHub(const FString& HubName)
{
	int32 MaxValueToSpawnWorker = 50;
	FVector location = this->GetTransform().GetLocation();
	location.X -= FMath::RandRange(0, MaxValueToSpawnWorker - 1) % MaxValueToSpawnWorker;
	location.Y -= FMath::RandRange(0, MaxValueToSpawnWorker - 1) % MaxValueToSpawnWorker;
	location.Z += 10;

	TempHub = GenerateHubObject(MoveTemp(location));
	TempHub->InitWorkerHub(HubName);

	return TempHub;
}

TObjectPtr<AWorkerHub> AMainPlayer::GenerateHubObject(FVector&& Location)
{
	FRotator rotation = { 0, 0, 0 };
	TObjectPtr<AWorkerHub> NewHub = this->GetWorld()->SpawnActor<AWorkerHub>(AWorkerHubClass, Location, rotation);
	NewHub->InitWorkerHub("NewHub");

	NewHub->AIControllerClass = AMainAIController::StaticClass();
	NewHub->SpawnDefaultController();
	Cast<AMainAIController>(NewHub->GetController())->CreateActionTree();

	return NewHub;
}

TObjectPtr<AWorkerHub> AMainPlayer::LoadHub(FVector&& Location)
{
	TObjectPtr<AWorkerHub> TempHubObj = GenerateHubObject(MoveTemp(Location));

	if (!TempHubObj)
		return nullptr;

	ExistedWorkerHubs.Add(TempHubObj);

	return TempHubObj;
}

void AMainPlayer::OpenCloseCoreMenu()
{
	if (bMainMenuOpen)
		return;

	CoreHUD->OpenCloseMenu();
}

void AMainPlayer::MainMenu()
{
	CoreHUD->OpenCloseMainMenu();
}

#pragma optimize("", off)
bool AMainPlayer::AttachWorkerToCore(TObjectPtr<AWorkerPawn> WPawn)
{
	if (!WPawn)
	{
		return false;
	}

	WPawn->SetActorLocation(this->GetActorLocation());
	WPawn->SetHidden(true);
	AttachedWorkers.Add(WPawn);

	return true;
}
#pragma optimize("", on)
bool AMainPlayer::AttachHub(TObjectPtr<AWorkerHub> WHub)
{
	if (!WHub || AttachedWorkerHubs.Contains(WHub))
	{
		return false;
	}
	
	WHub->TurnOffHub();
	AttachedWorkerHubs.Add(WHub);

	for (const auto& Action : WHub->GetActionList())
	{
		AddNewAction(Action);
	}

	for (const auto& Recipe : WHub->GetRecipeList())
	{
		AddNewRecipe(Recipe);
	}

	for (const auto& Obj : WHub->GetInfoList())
	{
		AddNewKnownObjectInfo(Obj);
	}

	CoreHUD->AddNewAttachedHub(WHub);

	return true;
}

void AMainPlayer::AddNewAction(EActionType AType)
{
	if (ExistedActions.Contains(AType))
		return;
	ExistedActions.Add(AType);
}

void AMainPlayer::AddNewRecipe(ERecipeType RType)
{
	if (ExistedRecipes.Contains(RType))
		return;
	ExistedRecipes.Add(RType);
}

void AMainPlayer::AddNewKnownObjectInfo(const TObjectPtr<UKnownObjectInfo>& Info)
{
	bool ObjExisted = false;
	for (const auto& KnownObject : ObjectList)
	{
		if (Info->GetKnownObjectLocation() == KnownObject->GetKnownObjectLocation())
		{
			ObjExisted = true;
			break;
		}
	}
	if (!ObjExisted)
	{
		ObjectList.Add(Info);
	}
}

void AMainPlayer::AddNewInfo(const TObjectPtr<UKnownObjectInfo>& Info)
{
	if (!ObjectList.Contains(Info))
	{
		ObjectList.Add(Info);
	}
}

const TArray<TObjectPtr<AWorkerPawn>>& AMainPlayer::GetAttachedWorkers() const
{
	return AttachedWorkers;
}

const TArray<TObjectPtr<AWorkerHub>>& AMainPlayer::GetAttachedWorkerHubs() const
{
	return AttachedWorkerHubs;
}

void AMainPlayer::FreeWorkerHub(TObjectPtr<AWorkerHub>& SelectedWorkerHub)
{
	AttachedWorkerHubs.Remove(SelectedWorkerHub);

	SelectedWorkerHub->SetHubHidden(false);

	SelectedWorkerHub->StartWork();
}

FVector AMainPlayer::GetLocation() const
{
	return this->GetActorLocation();
}

UPaperFlipbook* AMainPlayer::GetIActorPaperFlipbook() const
{
	return FlipbookComponent->GetFlipbook();
}


void AMainPlayer::RemoveHub(TObjectPtr<AWorkerHub>& WHub)
{
	AttachedWorkerHubs.Remove(WHub);
	ExistedWorkerHubs.Remove(WHub);
	WHub->RemoveHub();
	WHub->MarkAsGarbage();
}

void AMainPlayer::TurnOffControls()
{
	ControlsMustWork = false;
}

void AMainPlayer::TurnOnControls()
{
	ControlsMustWork = true;
}

const TArray<FString>* AMainPlayer::GetCoreLog() const
{
	return CoreLog.GetCoreLog();
}

int32 AMainPlayer::GetHubNumber() const
{
	return ExistedWorkerHubs.Num();
}

const TArray<TObjectPtr<AWorkerAddon>>& AMainPlayer::GetExistedAddons() const
{
	return AddonsInCore;
}

const TArray<EActionType>& AMainPlayer::GetKnownActionList() const
{
	return ExistedActions;
}

const TArray<ERecipeType>& AMainPlayer::GetKnownRecipeList() const
{
	return ExistedRecipes;
}

const TArray<TObjectPtr<UKnownObjectInfo>> AMainPlayer::GetKnownObject() const
{
	return ObjectList;
}

const TObjectPtr<UPaperFlipbook> AMainPlayer::GetCoreFlipbook() const
{
	return CoreFlipbook;
}

void AMainPlayer::SetMainMenuStatus(bool OpenMainMenu)
{
	bMainMenuOpen = OpenMainMenu;
}

const TArray<TObjectPtr<AWorkerPawn>>& AMainPlayer::GetAllPawns() const
{
	return ExistedWorkers;
}

const TArray<TObjectPtr<UWorkerMap>>& AMainPlayer::GetCreatedWorkerMap() const
{
	return CreatedWorkerMap;
}

TObjectPtr<UWorkerMap> AMainPlayer::CreateNewMap(const TArray<TObjectPtr<UKnownObjectInfo>>& ObjList)
{
	TObjectPtr<UWorkerMap> NewMap = NewObject<UWorkerMap>(this);

	if (!NewMap)
		return nullptr;

	NewMap->AddInfoListToMap(ObjList);
	NewMap->SetMapID(IDForWorkerMap);

	IDForWorkerMap += 1;

	CreatedWorkerMap.Add(NewMap);

	return NewMap;
}

void AMainPlayer::SetIDForMap(int32 ID)
{
	IDForWorkerMap = ID;
}

void AMainPlayer::SetIDForHub(int32 ID)
{
	IDForHub = ID;
}

int32 AMainPlayer::GetIDForMap() const
{
	return IDForWorkerMap;
}

int32 AMainPlayer::GetIDForHub() const
{
	return IDForHub;
}

const TArray<TObjectPtr<AWorkerHub>>& AMainPlayer::GetExistedWorkerHubs() const
{
	return ExistedWorkerHubs;
}