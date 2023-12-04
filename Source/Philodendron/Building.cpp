
#include "Building.h"
#include "MainGameStateBase.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "NavAreas/NavArea_Null.h"

ABuilding::ABuilding()
{
	PrimaryActorTick.bCanEverTick = true;

	this->SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	BuildingFlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("BuildingFlipbookComponent"));

	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractableActorCollision"));

	BuildingFlipbookComponent->SetupAttachment(RootComponent);

	BoxCollisionComponent->SetupAttachment(RootComponent);


	BoxCollisionComponent->bDynamicObstacle = true;
	BoxCollisionComponent->AreaClass = UNavArea_Null::StaticClass();
}

void ABuilding::BeginPlay()
{
	Super::BeginPlay();
}

void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BuildingInfo.bBuildingBusy)
	{
		BuildingInfo.TimeToMakeWork -= DeltaTime;
		if (BuildingInfo.TimeToMakeWork < 0)
		{
			BuildingInfo.TimeToMakeWork = 0;
			FinishWork();
			BuildingInfo.bBuildingBusy = false;
		}
	}
}

void ABuilding::InitBuilding(EBuildingType Type, const FDeviceAnimationStuct* Flipbook, int32 DistanceForCreate)
{
	BuildingInfo.BuildingType = Type;
	BuildingFlipbooks = Flipbook;

	BuildingFlipbookComponent->SetFlipbook(BuildingFlipbooks->Idle);
	BuildingFlipbookComponent->CastShadow = true;

	BuildingInfo.DistanceForCreatResources = DistanceForCreate;
}

EBuildingType ABuilding::GetBuildingType() const
{
	return BuildingInfo.BuildingType;
}

TObjectPtr<UPaperFlipbook> ABuilding::GetBuildingPaperFlipbook() const
{
	return BuildingFlipbookComponent->GetFlipbook();
}

bool ABuilding::IsBuildingBusy() const
{
	return BuildingInfo.bBuildingBusy;
}

bool ABuilding::MakeRecipe(ERecipeType Type)
{
	if (BuildingInfo.bBuildingBusy)
		return false;


	BuildingInfo.TimeToMakeWork = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetTimeToMakeRecipe(Type);

	if (BuildingInfo.TimeToMakeWork < 0)
		return false;

	BuildingInfo.RecipeForCreating = Type;
	BuildingInfo.bBuildingBusy = true;

	return true;
}

void ABuilding::FinishWork()
{
	if (BuildingInfo.RecipeForCreating == ERecipeType::Invalid)
		return;

	EGlobalRecipeType GlobalType = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetGlobalRecipeType(BuildingInfo.RecipeForCreating);

	if (GlobalType != EGlobalRecipeType::Resource)
		return;

	EResourceType ResourceType = Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->GetResourceWhichCreating(BuildingInfo.RecipeForCreating);

	if (ResourceType == EResourceType::Invalid)
		return;

	FVector Size = BuildingFlipbookComponent->GetFlipbook()->GetRenderBounds().GetBox().GetSize();
	Size.Z = 0;

	int32 MaxSize = Size.GetMax();

	int32 Distance = BuildingInfo.DistanceForCreatResources + MaxSize / 2;

	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->CreateNewResource(ResourceType, this->GetActorLocation(), Distance);
}

const FSupportBuildingInfo& ABuilding::GetBuildingInfo() const
{
	return BuildingInfo;
}

void ABuilding::LoadBuildingInfo(FSupportBuildingInfo&& Info)
{
	BuildingInfo = Info;
}