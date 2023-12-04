#include "WorkerAddon.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "MainGameStateBase.h"
#include "WorkerPawn.h"

AWorkerAddon::AWorkerAddon()
{
	PrimaryActorTick.bCanEverTick = false;
	this->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	AddonFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AddonFlipbookComponent"));

	AddonFlipbookComponent->SetupAttachment(RootComponent);
}

void AWorkerAddon::BeginPlay()
{
	Super::BeginPlay();
}

void AWorkerAddon::InitAddon(EWorkerAddonType Type)
{
	AddonType = Type;
	InitAddonName();
}

bool AWorkerAddon::ConnectAddon(EWorkerType WType, AWorkerPawn* WPawn)
{
	AddonPosition = WType;
	AddonOwner = WPawn;
	const FDeviceAnimationStuct* Flipbook = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetAddonFlipbooks(AddonType, AddonPosition);
	if (!Flipbook)
	{
		return false;
	}
	AddonFlipbooks.Stop = Flipbook->Stop;
	AddonFlipbooks.Work = Flipbook->Work;
;
	AddonFlipbookComponent->SetFlipbook(AddonFlipbooks.Work);
	AddonFlipbookComponent->CastShadow = true;
	return true;
}

void AWorkerAddon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const TObjectPtr<AWorkerPawn>& AWorkerAddon::GetAddonOwner() const
{
	return AddonOwner;
}

EWorkerAddonType AWorkerAddon::GetAddonType() const
{
	return AddonType;
}

FString AWorkerAddon::GetAddonInfo() const
{
	return AddonName + "\n";
}

TObjectPtr<UPaperFlipbookComponent> AWorkerAddon::GetAddonFlipbookComponent()
{
	return AddonFlipbookComponent;
}

const FString& AWorkerAddon::GetAddonName() const
{
	return AddonName;
}

void AWorkerAddon::InitAddonName()
{
	const FString* TempAddonName = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetAddonName(AddonType);

	if (!TempAddonName)
	{
		AddonName = "!!! MISSING NAME !!!";
	}
	else
	{
		AddonName = *TempAddonName;
	}
}
