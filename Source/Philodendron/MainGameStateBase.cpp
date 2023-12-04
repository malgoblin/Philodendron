// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameStateBase.h"

#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "ObjectManager.h"
#include "MainPlayer.h"

#include "WorkerAddon.h"
#include "Resource.h"

#include "InteractableActor.h"
#include "InteractableActorChanged.h"
#include "Kismet/GameplayStatics.h"
#include "Resource.h"

#include "Building.h"

#include "WorkerHub.h"

#include "NavigationSystem.h"
#include "LevelGenerator.h"
#include "Engine/TextureRenderTarget2D.h"
#include "WorkerMap.h"
#include "EnumActionTreeElementName.h"
#include "ManagerActions.h"
#include "ManagerActionTree.h"
#include "ManagerRecipe.h"
#include "ManagerInteractableActors.h"
#include "ManagerResources.h"
#include "ManagerMakingActions.h"
#include "ManagerAddons.h"
#include "ManagerBuilding.h"

#include "Engine/EngineTypes.h"
#include "Engine/PointLight.h"
#include "Engine/DirectionalLight.h"
#include "Components/LightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "MainSaveGame.h"
#include "MapHelperToCreate.h"
#include "KnownObjectInfo.h"


AMainGameStateBase::AMainGameStateBase()
{

}


bool AMainGameStateBase::CreateNewInteractableActor(EInteractableActorType Type, FVector Location, int32 ObjectNumber, EInteractableActorCreatingType CreatingType)
{
	TObjectPtr<AInteractableActor> NewActor = ManagerIActors->CreateNewInteractableActor(Type, Location, ObjectNumber, CreatingType);

	if (!NewActor)
	{
		return false;
	}

	ExistedActorsList.Add(NewActor);
	return true;
}

bool AMainGameStateBase::CreateInteractableActorFromRecipe(EInteractableActorType Type, FVector Location)
{
	int32 ObjNumber = ManagerIActors->GenerateRandomObject(Type);

	if (ObjNumber < 0)
		return false;

	return CreateNewInteractableActor(Type, Location, ObjNumber, EInteractableActorCreatingType::TakeFromIAManager);
}

FVector AMainGameStateBase::GetCoreLocation() const
{
	return MainCore->GetLocation();
}

TWeakObjectPtr<AMainPlayer> AMainGameStateBase::GetCore() const
{
	TWeakObjectPtr<AMainPlayer> Temp(MainCore);
	return Temp;
}


TObjectPtr<AWorkerAddon> AMainGameStateBase::CreateNewAddon(EWorkerAddonType AddonType, const FVector& Location)
{
	AWorkerAddon* NewAddon = this->GetWorld()->SpawnActor<AWorkerAddon>(AWorkerAddon::StaticClass(), Location, {0, 0, 0});
	if (!NewAddon)
	{
		return nullptr;
	}
	NewAddon->InitAddon(AddonType);
	NewAddon->SetHidden(true);

	ExistedAddonList.Add(NewAddon);
	return NewAddon;
}

const TArray<TObjectPtr<AWorkerAddon>>& AMainGameStateBase::GetAddonList() const
{
	return ExistedAddonList;
}

void AMainGameStateBase::RemoveExistedActor(AInteractableActor* InteractableActor)
{
	ExistedActorsList.Remove(InteractableActor);
}

bool AMainGameStateBase::CreateNewResource(EResourceType ResourceType, const FVector& Source, float Distance)
{
	FVector Location;

	if (!FindPlaceForNewResource(&Location, Source, Distance))
	{
		return false;
	}

	return GenerateNewResource(ResourceType, Location) != nullptr;
}

TObjectPtr<AResource> AMainGameStateBase::GenerateNewResource(EResourceType ResourceType, const FVector& Location)
{
	FRotator Rotation = { 0, 0, 0 };

	TObjectPtr<AResource> TempResource = ManagerResources->CreateNewResource(ResourceType, Location, Rotation);

	if (!TempResource)
	{
		return nullptr;
	}

	if (!ResourcesOnLand.Contains(ResourceType))
	{
		ResourcesOnLand.Add(ResourceType, {});
	}

	ResourcesOnLand[ResourceType].ResourceList.Add(TempResource);
	return TempResource;
}

bool AMainGameStateBase::FindPlaceForNewResource(FVector* Location, const FVector& Source, float Distance)
{
	FVector CurrentDir = { Distance, 0.0, 0.0 };

	FVector NewPoint = Source;

	double angle = 0.f;
	// to const
	constexpr double angleDiff = 30.f;
	while (angle < 360.f) // math lib?
	{
		CurrentDir = CurrentDir.RotateAngleAxis(angle, FVector::ZAxisVector);
		angle += angleDiff;

		NewPoint = Source + CurrentDir;
		bool IsExistingPoint = false;
		for (const auto& ResourceType : ResourcesOnLand)
		{
			for (const auto& Resource : ResourceType.Value.ResourceList)
			{
				if (Resource->GetActorLocation() == NewPoint)
				{
					IsExistingPoint = true;
					break;
				}
			}
		}
		if (!IsExistingPoint)
		{
			*Location = NewPoint;
			return true;
		}
	}

	return false;
}

void AMainGameStateBase::SetObjectManager(const TObjectPtr<AObjectManager>& OManager)
{
	ObjManager = OManager;
}

TObjectPtr<UPaperFlipbook> AMainGameStateBase::GetWorkerPawnFlipbook(EWorkerType WType)
{
	return ObjManager ? ObjManager->GetWorkerPawnFlipbook(WType) : nullptr;
}

const FDeviceAnimationStuct* AMainGameStateBase::GetAddonFlipbooks(EWorkerAddonType AddonType, EWorkerType WType)
{
	return ManagerAddons ? ManagerAddons->GetAddonFlipbooks(AddonType, WType) : nullptr;
}

void AMainGameStateBase::SetLevelGenerator(const TObjectPtr<ALevelGenerator>& LGenerator)
{
	LvlGenerator = LGenerator;
}
#pragma optimize("", off)
bool AMainGameStateBase::CreateCore(const FVector& Location)
{
	UPaperFlipbook* CoreFlipbook = ObjManager->GetCoreFlipbook();

	if (!CoreFlipbook)
	{
		return false;
	}

	MainCore = this->GetWorld()->SpawnActor<AMainPlayer>(AMainPlayer::StaticClass(), Location, { 0, 0, 0 });

	if (!MainCore)
	{
		return false;
	}

	MainCore->InitCore(CoreFlipbook);

	return true;
}
#pragma optimize("", on)
TObjectPtr<UTextureRenderTarget2D> AMainGameStateBase::GetInterfaceCam() const
{
	return ObjManager->GetInterfaceCam();
}

void AMainGameStateBase::GenerateActionForInteractableActor()
{
	for (const auto& IActor : ExistedActorsList)
	{
		IActor->SetActionList();
	}
}

void AMainGameStateBase::GenerateStartedAddons(const TMap<EWorkerAddonType, int32>& StartedAddons)
{
	if (!MainCore)
	{
		return;
	}
	for (const auto& Addon : StartedAddons)
	{
		for (int32 i = 0; i < Addon.Value; ++i)
		{
			CreateNewAddon(Addon.Key, MainCore->GetLocation());
		}
	}

	Cast<AMainPlayer>(MainCore.Get())->SetStartedAddons(ExistedAddonList);
}

void AMainGameStateBase::GenerateStartedWorkers(int32 WorkerCount)
{
	if (!MainCore)
	{
		return;
	}
	Cast<AMainPlayer>(MainCore.Get())->CreatedStartedWorkers(WorkerCount);
}

TObjectPtr<UPaperFlipbook> AMainGameStateBase::GetWorkerHubFlipbook()
{
	return ObjManager->GetWorkerHubFlipbook();
}

void AMainGameStateBase::SetManagerActions(const TObjectPtr<AManagerActions>& Manager)
{
	ManagerActions = Manager;
}

void AMainGameStateBase::SetManagerActionTree(const TObjectPtr<AManagerActionTree>& Manager)
{
	ManagerActionTree = Manager;
}

const TMap<EActionTreeElementName, FElementCost>& AMainGameStateBase::GetActionTreeElementCosts() const
{
	return ManagerActionTree->GetActionTreeElementCosts();
}

const TMap<EActionTreeElementName, FElementReward>& AMainGameStateBase::GetActionTreeElementReward() const
{
	return ManagerActionTree->GetActionTreeElementReward();
}

int32 AMainGameStateBase::GetActionSize(EActionType Type) const
{
	return ManagerActions->GetActionSize(Type);
}

float AMainGameStateBase::GetActionTime(EActionType Type) const
{
	return ManagerActions->GetActionTime(Type);
}

int32 AMainGameStateBase::GetRecipeSize(ERecipeType Type) const
{
	return ManagerRecipe->GetRecipeSize(Type);
}

void AMainGameStateBase::SetManagerRecipe(const TObjectPtr<AManagerRecipe>& Manager)
{
	ManagerRecipe = Manager;
}

void AMainGameStateBase::SetManagerInteractableActor(const TObjectPtr<AManagerInteractableActors>& Manager)
{
	ManagerIActors = Manager;
}

const TArray<EActionType>* AMainGameStateBase::GetAvailableActions(EInteractableActorType Type) const
{
	return ManagerIActors->GetAvailableActions(Type);
}

int32 AMainGameStateBase::GetSizeInfoAboutIActor(EInteractableActorType Type) const
{
	return ManagerIActors->GetSizeInfoAboutIActor(Type);
}
const TArray<EActionType>& AMainGameStateBase::GetKnownActionInCore() const
{
	return ManagerIActors->GetKnownActionInCore();
}

void AMainGameStateBase::SetManagerResources(const TObjectPtr<AManagerResources>& Manager)
{
	ManagerResources = Manager;
}

float AMainGameStateBase::GetDistanceForCreateResources(EInteractableActorType Type) const
{
	return ManagerIActors->GetDistanceForCreateResources(Type);
}

bool AMainGameStateBase::NeedInteractableActorForAction(EActionType Type) const
{
	return ManagerActions->NeedInteractableActorForAction(Type);
}

const TMap<EWorkerAddonType, int32>* AMainGameStateBase::GetConditionAddons(EActionType Type) const
{
	return ManagerActions->GetConditionAddons(Type);
}

const TArray<EResourceType>* AMainGameStateBase::GetResourcesMinedAction(EActionType Type) const
{
	return ManagerActions->GetResourcesMinedAction(Type);
}

const FPlantAnimationStruct* AMainGameStateBase::GetAnimationForIActor(EInteractableActorType Type, int32 ObjectNumber) const
{
	return ManagerIActors->GetAnimationForIActor(Type, ObjectNumber);
}

FVector3d AMainGameStateBase::GetBoxSizeFinalFlipbook(EInteractableActorType Type, int32 ObjectNumber) const
{
	return ManagerIActors->GetBoxSizeFinalFlipbook(Type, ObjectNumber);
}

int32 AMainGameStateBase::GenerateRandomObject(EInteractableActorType Type) const
{
	return ManagerIActors->GenerateRandomObject(Type);
}

const TMap<EActionTreeElementName, FLinkedElements>& AMainGameStateBase::GetActionTreeElementParents() const
{
	return ManagerActionTree->GetActionTreeElementParents();
}

const TMap<EActionTreeElementName, FLinkedElements>& AMainGameStateBase::GetActionTreeElementChildren() const
{
	return ManagerActionTree->GetActionTreeElementChildren();
}

void AMainGameStateBase::GenerateMainLevel()
{
	LvlGenerator->GenerateLevel();
}

EActionTreeElementName AMainGameStateBase::GetActionTreeElementNameForActionName(EActionType AType) const
{
	return ManagerActionTree->GetActionTreeElementNameForActionName(AType);
}

EActionTreeElementName AMainGameStateBase::GetActionTreeElementNameForRecipeName(ERecipeType RType) const
{
	return ManagerActionTree->GetActionTreeElementNameForRecipeName(RType);
}

int32 AMainGameStateBase::GetWorkerPawnMemorySize() const
{
	return ObjManager->GetWorkerPawnMemorySize();
}

int32 AMainGameStateBase::GetTreeElementSize(EActionTreeElementName TreeElementName) const
{
	return ManagerActionTree->GetTreeElementSize(TreeElementName);
}

const FElementReward* AMainGameStateBase::GetRewardForTreeElement(EActionTreeElementName TreeElement) const
{
	return ManagerActionTree->GetRewardForTreeElement(TreeElement);
}

void AMainGameStateBase::SetManagerMakingActions(const TObjectPtr<AManagerMakingActions>& Manager)
{
	ManagerMakingActions = Manager;
}

bool AMainGameStateBase::PrepairToNewAction(TObjectPtr<AWorkerHub> WHub, EActionType Type, TWeakObjectPtr<AInteractableActor>& IActor, ERecipeType RecipeType)
{
	return ManagerMakingActions->PrepairToNewAction(WHub, Type, IActor, RecipeType);
}

int32 AMainGameStateBase::GetDistanceToMakeAction(EActionType Type) const
{
	return ManagerActions->GetDistanceToMakeAction(Type);
}

float AMainGameStateBase::GetDefaultWorkerSpeed() const
{
	return ObjManager->GetDefaultWorkerSpeed();
}

float AMainGameStateBase::GetDefaultWorkerSpottingRadius() const
{
	return ObjManager->GetDefaultWorkerSpottingRadius();
}

float AMainGameStateBase::GetWorkerRestTime() const
{
	return ObjManager->GetWorkerRestTime();
}
bool AMainGameStateBase::ContinueToPrepair(TObjectPtr<AWorkerHub> WHub)
{
	return ManagerMakingActions->ContinueToPrepair(WHub);
}

int32 AMainGameStateBase::GetMinimalFoVCamera() const
{
	return ObjManager->GetMinimalFoVCamera();
}

int32 AMainGameStateBase::GetMaxFoVCamera() const
{
	return ObjManager->GetMaxFoVCamera();
}

bool AMainGameStateBase::AddAddonsToCore(TObjectPtr<AWorkerAddon> NewAddon)
{
	return MainCore->AddAddonsToCore(NewAddon);
}

bool AMainGameStateBase::AttachWorkerToCore(TObjectPtr<AWorkerPawn> WPawn)
{
	return MainCore->AttachWorkerToCore(WPawn);
}

bool AMainGameStateBase::CreateHole(const FVector& Location)
{
	int32 ObjNumber = GenerateRandomObject(EInteractableActorType::Hole);
	if (ObjNumber < 0)
		return false;

	return CreateNewInteractableActor(EInteractableActorType::Hole, Location, ObjNumber, EInteractableActorCreatingType::FinalStage);
}

const TMap<EResourceType, int32>* AMainGameStateBase::GetNeededResourcesForRecipe(ERecipeType Type) const
{
	return ManagerRecipe->GetNeededResourcesForRecipe(Type);
}

TWeakObjectPtr<AResource> AMainGameStateBase::FindClosestResourceInArea(const TArray<EResourceType>& TypeList, const FVector& Source, int32 RadiusSq) const
{	
	TObjectPtr<AResource> ClosestResource = nullptr;

	TArray<EResourceType> TempList = TypeList;

	for (const auto& Resource : ResourcesOnLand)
	{
		if (!TempList.Contains(Resource.Key))
			continue;
		
		for (const auto& ResObj : Resource.Value.ResourceList)
		{
			if (ClosestResource && FVector::DistSquared(Source, ClosestResource->GetLocation()) > FVector::DistSquared(Source, ResObj->GetLocation()))
			{
				ClosestResource = ResObj;
				continue;
			}
			if (!ClosestResource && FVector::DistSquared(Source, ResObj->GetLocation()) <= RadiusSq)
			{
				ClosestResource = ResObj;
			}
		}

		TempList.Remove(Resource.Key);
	}

	TWeakObjectPtr<AResource> TempResource(ClosestResource);

	return TempResource;
}

bool AMainGameStateBase::IsResourceExist(const TObjectPtr<AResource>& Resource) const
{
	return true;
}

TArray<TWeakObjectPtr<AInteractableActor>> AMainGameStateBase::FindSpottingInteractableActorPossibleForAction(FVector Source, float RadiusSq) const
{
	TArray<TWeakObjectPtr<AInteractableActor>> ActorList;

	for (const auto& IActor : ExistedActorsList)
	{
		const double DistSq = FVector2D::DistSquared(FVector2D(Source), FVector2D(IActor->GetLocation()));
		if (DistSq <= RadiusSq)
		{
			TWeakObjectPtr<AInteractableActor> Temp(IActor);

			ActorList.Add(Temp);
		}
	}

	return ActorList;
}

void AMainGameStateBase::SetManagerAddons(const TObjectPtr<AManagerAddons>& Manager)
{
	ManagerAddons = Manager;
}

const TMap<EAddonModifiers, float>* AMainGameStateBase::GetAddonModifiers(EWorkerAddonType Type) const
{
	return ManagerAddons->GetAddonModifiers(Type);
}

bool AMainGameStateBase::GetResourceToWorker(TObjectPtr<AWorkerHub>& WHub, TWeakObjectPtr<AResource> Res)
{
	if (!Res.IsValid())
		return false;

	if (!ResourcesOnLand.Contains(Res->GetResourceType()))
		return false;

	TObjectPtr<AResource> ResForGet = nullptr;

	for (const auto& Resource : ResourcesOnLand[Res->GetResourceType()].ResourceList)
	{
		if (Resource.Get() == Res.Get())
		{
			if (WHub->TakeResource(Resource))
			{
				ResForGet = Resource;
				break;
			}
			return false;
		}
	}

	if (!ResForGet)
		return false;

	ResourcesOnLand[Res->GetResourceType()].ResourceList.Remove(ResForGet);
	return true;
}

float AMainGameStateBase::GetTimeToMakeRecipe(ERecipeType Type) const
{
	return ManagerRecipe->GetTimeToMakeRecipe(Type);
}

EPlaceWhereMakingRecipe AMainGameStateBase::GetPlaceWhereMakingRecipe(ERecipeType Type) const
{
	return ManagerRecipe->GetPlaceWhereMakingRecipe(Type);
}

float AMainGameStateBase::GetEmptyAreaAroundEmptyPlace(ERecipeType Type) const
{
	return ManagerRecipe->GetEmptyAreaAroundEmptyPlace(Type);
}

bool AMainGameStateBase::IsPointEmpty(const FVector& Point, float EmptyArea) const
{
	float EmptyAreaSq = EmptyArea * EmptyArea;

	for (const auto& IActor : ExistedActorsList)
	{
		if (FVector::DistSquared2D(Point, IActor->GetLocation()) <= EmptyAreaSq)
			return false;
	}

	for (const auto& Resource : ResourcesOnLand)
	{
		for (const auto& ResObj : Resource.Value.ResourceList)
		{
			if (FVector::DistSquared2D(Point, ResObj->GetLocation()) <= EmptyAreaSq)
				return false;
		}
	}

	for (const auto& Building : ExistedBuilding)
	{
		if (FVector::DistSquared2D(Point, Building->GetActorLocation()) <= EmptyAreaSq)
			return false;
	}

	if (FVector::DistSquared2D(Point, MainCore->GetLocation()) <= EmptyAreaSq)
		return false;

	return true;
}

EGlobalRecipeType AMainGameStateBase::GetGlobalRecipeType(ERecipeType Type) const
{
	return ManagerRecipe->GetGlobalRecipeType(Type);
}

EInteractableActorType AMainGameStateBase::GetInteractableActorForCreating(ERecipeType Type) const
{
	return ManagerRecipe->GetInteractableActorForCreating(Type);
}

EBuildingType AMainGameStateBase::GetNeededBuildingForCretingRecipe(ERecipeType Type) const
{
	return ManagerRecipe->GetNeededBuildingForCretingRecipe(Type);
}

EInteractableActorType AMainGameStateBase::GetNeededInteractableActorWhereCreateRecipe(ERecipeType Type) const
{
	return ManagerRecipe->GetNeededInteractableActorWhereCreateRecipe(Type);
}

EBuildingType AMainGameStateBase::GetBuildingWhichCreating(ERecipeType Type) const
{
	return ManagerRecipe->GetBuildingWhichCreating(Type);
}

TArray<TWeakObjectPtr<AInteractableActor>> AMainGameStateBase::FindInteractableActorInArea(EInteractableActorType ActorType, const FVector& Source, int32 RadiusSq) const
{
	TArray<TWeakObjectPtr<AInteractableActor>> ActorList;

	for (const auto& IActor : ExistedActorsList)
	{
		if (IActor->GetActorType() == ActorType && FVector::DistSquared2D(Source, IActor->GetLocation()) <= RadiusSq)
		{
			TWeakObjectPtr<AInteractableActor> NewWeak(IActor);
			ActorList.Add(NewWeak);
		}
	}
	return ActorList;
}

void AMainGameStateBase::SetManagerBuilding(const TObjectPtr<AManagerBuilding>& Manager)
{
	ManagerBuilding = Manager;
}

bool AMainGameStateBase::CreateBuildingOnInteractableActor(EBuildingType Type, TWeakObjectPtr<AInteractableActor> IActor, AWorkerHub* WHub)
{
	if (!WHub || !IActor.IsValid() || Type == EBuildingType::Invalid)
		return false;

	if (!IActor->IsWorkerHubLocked(WHub))
		return false;

	TObjectPtr<AInteractableActor> ActorForRemove = nullptr;

	for (const auto& Actor : ExistedActorsList)
	{
		if (IActor.Get() == Actor.Get())
		{
			ActorForRemove = Actor;
			break;
		}
	}

	TObjectPtr<ABuilding> NewBuilding = ManagerBuilding->CreateNewBuilding(Type, ActorForRemove->GetLocation());

	if (!NewBuilding)
		return false;

	ActorForRemove->SetHidden(true);

	ExistedBuilding.Add(NewBuilding);

	ExistedActorsList.Remove(ActorForRemove);

	return true;
}

bool AMainGameStateBase::CreateBuildingOnEmptyPlace(EBuildingType Type, FVector Location)
{
	TObjectPtr<ABuilding> NewBuilding = ManagerBuilding->CreateNewBuilding(Type, Location);

	if (!NewBuilding)
		return false;

	ExistedBuilding.Add(NewBuilding);

	return true;
}



EResourceType AMainGameStateBase::GetResourceWhichCreating(ERecipeType Type) const
{
	return ManagerRecipe->GetResourceWhichCreating(Type);
}

TArray<TWeakObjectPtr<ABuilding>> AMainGameStateBase::FindBuildingInArea(EBuildingType BuildType, const FVector& Source, int32 RadiusSq) const
{
	TArray<TWeakObjectPtr<ABuilding>> BuildingList;

	for (const auto& Building : ExistedBuilding)
	{
		if (Building->GetBuildingType() == BuildType && FVector::DistSquared2D(Source, Building->GetActorLocation()) <= RadiusSq)
		{
			TWeakObjectPtr<ABuilding> ValidBuilding(Building);
			BuildingList.Add(ValidBuilding);
		}

	}
	return BuildingList;
}

ENeedWorkerToMakeRecipe AMainGameStateBase::GetNeededWorkerToMakeRecipe(ERecipeType Type) const
{
	return ManagerRecipe->GetNeededWorkerToMakeRecipe(Type);
}

const FString* AMainGameStateBase::GetActionName(EActionType Type) const
{
	return ManagerActions->GetActionName(Type, GameLanguages);
}

const FString* AMainGameStateBase::GetRecipeName(ERecipeType Type) const
{
	return ManagerRecipe->GetRecipeName(Type, GameLanguages);
}

void AMainGameStateBase::SetGameLanguages(EPossibleLanguages Languages)
{
	GameLanguages = Languages;
}

EPossibleLanguages AMainGameStateBase::GetGameLanguages() const
{
	return GameLanguages;
}

const TMap<EPossibleLanguages, FString>* AMainGameStateBase::GetAvailableLanguages() const
{
	return ObjManager->GetAvailableLanguages();
}

const FString* AMainGameStateBase::GetAddonName(EWorkerAddonType Type) const
{
	return ManagerAddons->GetAddonName(Type, GameLanguages);
}

const FString* AMainGameStateBase::GetInteractableActorName(EInteractableActorType Type) const
{
	return ManagerIActors->GetInteractableActorName(Type, GameLanguages);
}

const TObjectPtr<UTexture2D>& AMainGameStateBase::GetWorkerPawnIcon() const
{
	return ObjManager->GetWorkerPawnIcon();
}

const TObjectPtr<UTexture2D>& AMainGameStateBase::GetWorkerPawnImage() const
{
	return ObjManager->GetWorkerPawnImage();
}

const TObjectPtr<UTexture2D>& AMainGameStateBase::GetWorkerHubIcon() const
{
	return ObjManager->GetWorkerHubIcon();
}

const TObjectPtr<UTexture2D>& AMainGameStateBase::GetWorkerHubImage() const
{
	return ObjManager->GetWorkerHubImage();
}

const TObjectPtr<UTexture> AMainGameStateBase::GetAddonIcon(EWorkerAddonType Type) const
{
	return ManagerAddons->GetAddonIcon(Type);
}

const TObjectPtr<UTexture> AMainGameStateBase::GetAddonImage(EWorkerAddonType Type) const
{
	return ManagerAddons->GetAddonImage(Type);
}

void AMainGameStateBase::CreateSun(FVector&& MapEdge)
{
	FVector TempPossition = MapEdge + ObjManager->GetSunPossitionRelativelyMapEdge();
	GenerateSunInPossition(MoveTemp(TempPossition));
}

void AMainGameStateBase::GenerateSunInPossition(FVector&& Possition)
{
	PLight = this->GetWorld()->SpawnActor<APointLight>();

	PLight->SetMobility(EComponentMobility::Movable);

	PLight->SetActorLocation(Possition);

	PLight->SetRadius(ObjManager->GetSunWorkRadius());
	PLight->GetLightComponent()->Intensity = ObjManager->GetSunIntensity();
	PLight->GetLightComponent()->SetUseTemperature(ObjManager->HasUseTemperature());
	PLight->GetLightComponent()->SetTemperature(ObjManager->GetTemperature());
	Cast<UPointLightComponent>(PLight->GetLightComponent())->SourceRadius = ObjManager->GetSourceRadius();
	PLight->SetEnabled(true);

	DLight = this->GetWorld()->SpawnActor<ADirectionalLight>();
	DLight->SetMobility(EComponentMobility::Stationary);

	DLight->GetLightComponent()->SetUseTemperature(ObjManager->HasUseTemperature());
	DLight->GetLightComponent()->SetTemperature(ObjManager->GetTemperature());
	DLight->GetLightComponent()->SetIntensity(ObjManager->GetDirectionalIntensity());
	Cast<UDirectionalLightComponent>(DLight->GetLightComponent())->Intensity = ObjManager->GetDirectionalIntensity();
	DLight->SetEnabled(true);

}

void AMainGameStateBase::SaveGame()
{
	TObjectPtr<UMainSaveGame> Save = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));

	Save->SeedForGenerateTileMap = LvlGenerator->GetSeedForMapTileGeneration();
	Save->SunPossition = PLight->GetActorLocation();

	SaveCore(Save);
	SaveInteractableActors(Save);
	SaveAddons(Save);
	SavePawns(Save);
	SaveResources(Save);
	SaveHubs(Save);
	SaveBuilding(Save);
	SaveMakingAction(Save);

	UGameplayStatics::SaveGameToSlot(Save, "SaveFile", 0);
}

bool AMainGameStateBase::LoadGame()
{
	this->GetWorldSettings()->SetTimeDilation(0);

	TObjectPtr<UMainSaveGame> Save = Cast<UMainSaveGame>(UGameplayStatics::CreateSaveGameObject(UMainSaveGame::StaticClass()));

	Save = Cast<UMainSaveGame>(UGameplayStatics::LoadGameFromSlot("SaveFile", 0));

	if (!Save)
		return false;

	LvlGenerator->GenerateMap(Save->SeedForGenerateTileMap);
	GenerateSunInPossition(MoveTemp(Save->SunPossition));

	LoadAddons(Save);
	LoadInteractableActors(Save);
	LoadCore(Save);
	LoadPawns(Save);
	LoadAddonsToCore(Save);
	LoadResources(Save);
	LoadHubs(Save);
	LoadAttachedHubPawnToCore(Save);
	LoadBuilding(Save);
	LoadMakingAction(Save);
	this->GetWorldSettings()->SetTimeDilation(1);

	return true;

}

void AMainGameStateBase::SaveCore(TObjectPtr<UMainSaveGame>& SaveFile)
{
	SaveFile->CoreInfo.CorePossition = MainCore->GetActorLocation();

	for (const auto& Action : MainCore->GetKnownActionList())
	{
		SaveFile->CoreInfo.ExistedActions.Add(Action);
	}

	for (const auto& Recipe : MainCore->GetKnownRecipeList())
	{
		SaveFile->CoreInfo.ExistedRecipe.Add(Recipe);
	}

	for (const auto& Info : MainCore->GetKnownObject())
	{
		SaveFile->CoreInfo.KnownObjectInfo.Add(Info->GetKnownObjectInfo());
	}

	for (const auto& Pawn : MainCore->GetAttachedWorkers())
	{
		SaveFile->CoreInfo.AttachedPawns.Add(Pawn->GetPawnID());
	}

	for (const auto& Hub : MainCore->GetAttachedWorkerHubs())
	{
		SaveFile->CoreInfo.AttachedHubs.Add(Hub->GetHubID());
	}

	for (const auto& Map : MainCore->GetCreatedWorkerMap())
	{
		FWorkerMapInfo TempMapInfo;
		TempMapInfo.MapID = Map->GetMapID();
		for (const auto& Obj : Map->GetMapInfo())
		{
			TempMapInfo.KnownObjectInfo.Add(Obj->GetKnownObjectInfo());
		}

		SaveFile->CoreInfo.WorkerMapList.Add(TempMapInfo);
	}

	for (const auto& Addon : MainCore->GetExistedAddons())
	{
		SaveFile->CoreInfo.AddonList.Add(Addon->GetAddonType());
	}

	SaveFile->CoreInfo.MapID = MainCore->GetIDForMap();
	SaveFile->CoreInfo.HubID = MainCore->GetIDForHub();
}

void AMainGameStateBase::SaveInteractableActors(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (const auto& Actor : ExistedActorsList)
	{
		FInteractableActorInfo TempStructInfo;
		TempStructInfo.IAInfo = Actor->GetInteractableActorInfo();

		if (Actor->GetWorkerHubLockActor().IsValid())
		{
			TempStructInfo.IDWorkerHubLockActor = Actor->GetWorkerHubLockActor()->GetHubID();
		}
		TempStructInfo.IActorPossition = Actor->GetActorLocation();

		for (const auto& WHub : Actor->GetLockedResources())
		{
			if (!WHub.Key.IsValid())
				continue;

			TempStructInfo.LockedResources.Add(WHub.Key->GetHubID(), WHub.Value);
		}

		AInteractableActorChanged* TempChangeActor = Cast<AInteractableActorChanged>(Actor);
		if (!TempChangeActor)
		{
			SaveFile->InteractableActorList.Add(TempStructInfo);
			continue;
		}

		FInteractableActorChangedInfo TempStructChangedInfo;
		TempStructChangedInfo.IAInfo = TempStructInfo;
		TempStructChangedInfo.IAChangedInfo = TempChangeActor->GetInteractableActorChangedInfo();
		SaveFile->InteractableActorChangedList.Add(TempStructChangedInfo);
	}
}

void AMainGameStateBase::SavePawns(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (const auto& Pawn : MainCore->GetAllPawns())
	{
		FPawnInfo TempPawnInfo;
		TempPawnInfo.PawnInfo = Pawn->GetPawnInfo();

		for (const auto& Addon : Pawn->GetWorkerAddon())
		{
			if (!Addon.Value)
				continue;

			TempPawnInfo.ConnectedAddons.Add(Addon.Key, Addon.Value->GetAddonType());
		}

		for (const auto& KnownObj : Pawn->GetObjectList())
		{
			TempPawnInfo.KnownObjectInfo.Add(KnownObj->GetKnownObjectInfo());
		}

		for (const auto& PartOfObj : Pawn->GetPartOfObjectInfo())
		{
			FPartOfKnownObject TempPartOfObj;
			TempPartOfObj.PartOfObject = PartOfObj.Key->GetKnownObjectInfo();
			TempPartOfObj.PartOfObjectSize = PartOfObj.Value;
			TempPawnInfo.PartOfKnownObjectInfo.Add(TempPartOfObj);
		}

		TempPawnInfo.WorkerMapID = Pawn->GetWorkerMapID();
		TempPawnInfo.PartOfWorkerMapID = Pawn->GetPartOfWorkerMapID();
		TempPawnInfo.PartOfWorkerMapSize = Pawn->GetPartOfMapSize();

		if (Pawn->GetMasterHub())
		{
			TempPawnInfo.IDMasterHub = Pawn->GetMasterHub()->GetHubID();
		}

		SaveFile->PawnList.Add(TempPawnInfo);
	}
}

void AMainGameStateBase::SaveAddons(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (const auto& Addon : ExistedAddonList)
	{
		SaveFile->AddonList.Add(Addon->GetAddonType());
	}
}

void AMainGameStateBase::SaveResources(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (const auto& Resource : ResourcesOnLand)
	{
		for (const auto& SingleResource : Resource.Value.ResourceList)
		{
			FResourceInfo Info;
			Info.Location = SingleResource->GetActorLocation();
			Info.Type = Resource.Key;

			SaveFile->ResourceList.Add(Info);
		}
	}
}
#pragma optimize("", off)
void AMainGameStateBase::SaveHubs(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (const auto& Hub : MainCore->GetExistedWorkerHubs())
	{
		FHubInfo TempHub;

		TempHub.Info = Hub->GetHubInfo();

		for (const auto& Resource : Hub->GetPossessedResourceList())
		{
			TempHub.PossessedResources.Add(Resource->GetResourceType());
		}

		for (const auto& Pawn : Hub->GetConnectedPawn())
		{
			if (!Pawn.Value)
				continue;

			TempHub.ConnectedPawns.Add(Pawn.Key, Pawn.Value->GetPawnID());
		}

		TempHub.Location = Hub->GetActorLocation();

		TempHub.AIInfo = Hub->GetAIInfo();

		SaveFile->HubList.Add(TempHub);
	}
}
#pragma optimize("", on)
void AMainGameStateBase::SaveBuilding(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (const auto& Building : ExistedBuilding)
	{
		FBuildingSaveInfo TempInfo;

		TempInfo.Info = Building->GetBuildingInfo();
		TempInfo.Location = Building->GetActorLocation();

		SaveFile->BuildingList.Add(TempInfo);
	}
}

void AMainGameStateBase::SaveMakingAction(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (const auto& Action : ManagerMakingActions->GetCurrentPrepairAction())
	{
		FMakingActionInfo TempInfo;
		TempInfo.HubID = Action.Key->GetHubID();
		TempInfo.RecipeType = Action.Value.RecipeType;
		TempInfo.RemainingTime = Action.Value.RemainingTime;
		TempInfo.SubActionType = Action.Value.SubActionType;
		TempInfo.Type = Action.Value.Type;

		if (Action.Value.CurrentBuilding.IsValid())
		{
			TempInfo.BuildingLocation = Action.Value.CurrentBuilding->GetActorLocation();
		}

		if (Action.Value.CurrentInteractableActor.IsValid())
		{
			TempInfo.IActorLocation = Action.Value.CurrentInteractableActor->GetActorLocation();
		}

		if (Action.Value.CurrentResource.IsValid())
		{
			TempInfo.ResourceLocation = Action.Value.CurrentResource->GetActorLocation();
		}

		SaveFile->MakingActionList.Add(TempInfo);
	}
}

void AMainGameStateBase::LoadCore(TObjectPtr<UMainSaveGame>& SaveFile)
{
	CreateCore(SaveFile->CoreInfo.CorePossition);

	for (const auto& Action : SaveFile->CoreInfo.ExistedActions)
	{
		MainCore->AddNewAction(Action);
	}

	for (const auto& Recipe : SaveFile->CoreInfo.ExistedRecipe)
	{
		MainCore->AddNewRecipe(Recipe);
	}

	for (auto& Info : SaveFile->CoreInfo.KnownObjectInfo)
	{
		UKnownObjectInfo* ObjectInfo = NewObject<UKnownObjectInfo>(this);

		ObjectInfo->SetInfo(MoveTemp(Info));
		ObjectInfo->SetWeakObjectPtr(nullptr);

		for (const auto& IActor : ExistedActorsList)
		{
			if (IActor->GetActorLocation() == Info.KnownObjectLocation)
			{
				TWeakObjectPtr<AInteractableActor> TempWeakPtr(IActor);
				ObjectInfo->SetWeakObjectPtr(TempWeakPtr);
				break;
			}
		}

		MainCore->AddNewKnownObjectInfo(ObjectInfo);
	}

	for (auto& Map : SaveFile->CoreInfo.WorkerMapList)
	{
		TArray<TObjectPtr<UKnownObjectInfo>> ObjectList;

		for (const auto& MapObj : Map.KnownObjectInfo)
		{
			for (const auto& KnownObj : MainCore->GetKnownObject())
			{
				if (KnownObj->GetKnownObjectLocation() == MapObj.KnownObjectLocation)
				{
					ObjectList.Add(KnownObj);
					break;
				}
			}
		}
		
		TObjectPtr<UWorkerMap> TempMap = MainCore->CreateNewMap(ObjectList);
		TempMap->SetMapID(Map.MapID);
	}

	MainCore->SetIDForMap(SaveFile->CoreInfo.MapID);
	MainCore->SetIDForHub(SaveFile->CoreInfo.HubID);
}
#pragma optimize("", off)
void AMainGameStateBase::LoadInteractableActors(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (auto& Actor : SaveFile->InteractableActorList)
	{
		TObjectPtr<AInteractableActor> TempActor = ManagerIActors->GenerateIActorObject(Actor.IAInfo.ActorType, Actor.IActorPossition, EInteractableActorCreatingType::FinalStage);

		if (!TempActor)
			continue;

		TempActor->LoadInteractableActor(MoveTemp(Actor.IAInfo));
		ExistedActorsList.Add(TempActor);
	}

	for (auto& Actor : SaveFile->InteractableActorChangedList)
	{
		TObjectPtr<AInteractableActorChanged> TempActor = Cast<AInteractableActorChanged>(ManagerIActors->GenerateIActorObject(Actor.IAInfo.IAInfo.ActorType, Actor.IAInfo.IActorPossition, EInteractableActorCreatingType::FirstStage));

		if (!TempActor)
			continue;

		TempActor->LoadInteractableActor(MoveTemp(Actor.IAInfo.IAInfo), MoveTemp(Actor.IAChangedInfo));
		ExistedActorsList.Add(TempActor);
	}
}
#pragma optimize("", on)

#pragma optimize("", off)
void AMainGameStateBase::LoadPawns(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (auto& Pawn : SaveFile->PawnList)
	{
		TObjectPtr<AWorkerPawn> TempPawn = MainCore->GenerateNewPawn();

		TempPawn->LoadPawnInfo(MoveTemp(Pawn.PawnInfo));
		
		TObjectPtr<UWorkerMap> TempFullMap = nullptr;
		TObjectPtr<UWorkerMap> TempPartOfMap = nullptr;


		for (const auto& Map : MainCore->GetCreatedWorkerMap())
		{
			if (Map->GetMapID() == Pawn.WorkerMapID)
			{
				TempFullMap = Map;
				break;
			}

			if (Map->GetMapID() == Pawn.PartOfWorkerMapID)
			{
				TempPartOfMap = Map;
				break;
			}
		}

		TempPawn->LoadWorkerMap(TempFullMap, TempPartOfMap, Pawn.PartOfWorkerMapSize);

		for (const auto& AType : Pawn.ConnectedAddons)
		{
			for (auto& Addon : ExistedAddonList)
			{
				if (Addon->GetAddonType() == AType.Value && !Addon->GetAddonOwner())
				{
					TempPawn->AddNewAddon(Addon, AType.Key);
					break;
				}
			}
		}

		TArray<TObjectPtr<UKnownObjectInfo>> TempObjectList;

		TMap<TObjectPtr<UKnownObjectInfo>, int32> TempPartOfObjectInfo;

		for (auto& FullObj : Pawn.KnownObjectInfo)
		{
			UKnownObjectInfo* ObjectInfo = NewObject<UKnownObjectInfo>(this);

			ObjectInfo->SetInfo(MoveTemp(FullObj));
			ObjectInfo->SetWeakObjectPtr(nullptr);

			for (const auto& IActor : ExistedActorsList)
			{
				if (IActor->GetActorLocation() == FullObj.KnownObjectLocation)
				{
					TWeakObjectPtr<AInteractableActor> TempWeakPtr(IActor);
					ObjectInfo->SetWeakObjectPtr(TempWeakPtr);
					break;
				}
			}

			TempObjectList.Add(ObjectInfo);
		}

		for (auto& PartObj : Pawn.PartOfKnownObjectInfo)
		{
			UKnownObjectInfo* ObjectInfo = NewObject<UKnownObjectInfo>(this);

			ObjectInfo->SetInfo(MoveTemp(PartObj.PartOfObject));
			ObjectInfo->SetWeakObjectPtr(nullptr);

			for (const auto& IActor : ExistedActorsList)
			{
				if (IActor->GetActorLocation() == PartObj.PartOfObject.KnownObjectLocation)
				{
					TWeakObjectPtr<AInteractableActor> TempWeakPtr(IActor);
					ObjectInfo->SetWeakObjectPtr(TempWeakPtr);
					break;
				}
			}
			
			TempPartOfObjectInfo.Add(ObjectInfo, PartObj.PartOfObjectSize);
		}

		TempPawn->LoadKnownObjects(MoveTemp(TempObjectList), MoveTemp(TempPartOfObjectInfo));


	}
}
#pragma optimize("", on)
void AMainGameStateBase::LoadAddons(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (const auto& AType : SaveFile->AddonList)
	{
		CreateNewAddon(AType, { 0, 0, 0 });
	}
}

void AMainGameStateBase::LoadAddonsToCore(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (const auto& AType : SaveFile->CoreInfo.AddonList)
	{
		for (auto& Addon : ExistedAddonList)
		{
			if (!MainCore->HasAddon(Addon) && Addon->GetAddonType() == AType && !Addon->GetAddonOwner())
			{
				MainCore->AddAddonsToCore(Addon);
				break;
			}
		}
	}
}

void AMainGameStateBase::LoadResources(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (const auto& Resource : SaveFile->ResourceList)
	{
		GenerateNewResource(Resource.Type, Resource.Location);
	}
}
#pragma optimize("", off)
void AMainGameStateBase::LoadHubs(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (auto& HubInfo : SaveFile->HubList)
	{
		TObjectPtr<AWorkerHub> TempHub = MainCore->LoadHub(MoveTemp(HubInfo.Location));

		TempHub->SetHubInfo(MoveTemp(HubInfo.Info));

		for (const auto& PawnID : HubInfo.ConnectedPawns)
		{
			for (auto& Pawn : MainCore->GetExistedPawn())
			{
				if (Pawn->GetPawnID() == PawnID.Value)
				{
					TempHub->AttachWorker(Pawn, PawnID.Key);
					break;
				}
			}
		}

		for (const auto& ResourceType : HubInfo.PossessedResources)
		{
			TObjectPtr<AResource> TempResource = GenerateNewResource(ResourceType, { 0, 0, 0 });

			if (!TempResource)
				continue;

			GetResourceToWorker(TempHub, TempResource);
		}

		TempHub->LoadAIInfo(MoveTemp(HubInfo.AIInfo));
		int32 t = 0;
		t += 1;
	}
}
#pragma optimize("", on)
void AMainGameStateBase::LoadAttachedHubPawnToCore(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (const auto& PawnID : SaveFile->CoreInfo.AttachedPawns)
	{
		for (auto& Pawn : MainCore->GetExistedPawn())
		{
			if (Pawn->GetPawnID() == PawnID)
			{
				MainCore->AttachWorkerToCore(Pawn);
				break;
			}
		}
	}

	for (const auto& HubID : SaveFile->CoreInfo.AttachedHubs)
	{
		for (auto& Hub : MainCore->GetExistedWorkerHubs())
		{
			if (Hub->GetHubID() == HubID)
			{
				MainCore->AttachHub(Hub);
				break;
			}
		}
	}
}

void AMainGameStateBase::LoadBuilding(TObjectPtr<UMainSaveGame>& SaveFile)
{
	for (auto& Info : SaveFile->BuildingList)
	{
		TObjectPtr<ABuilding> NewBuilding = ManagerBuilding->CreateNewBuilding(Info.Info.BuildingType, Info.Location);

		if (!NewBuilding)
			return;
		NewBuilding->LoadBuildingInfo(MoveTemp(Info.Info));
		ExistedBuilding.Add(NewBuilding);
	}
}

void AMainGameStateBase::LoadMakingAction(TObjectPtr<UMainSaveGame>& SaveFile)
{
	TMap<TObjectPtr<AWorkerHub>, FInfoAboutAction> TempList;

	for (const auto& Info : SaveFile->MakingActionList)
	{
		FInfoAboutAction TempInfo;

		TempInfo.RecipeType = Info.RecipeType;
		TempInfo.RemainingTime = Info.RemainingTime;
		TempInfo.SubActionType = Info.SubActionType;
		TempInfo.Type = Info.Type;

		for (const auto& IActor : ExistedActorsList)
		{
			if (IActor->GetActorLocation() == Info.IActorLocation)
			{
				TWeakObjectPtr<AInteractableActor> TempActor(IActor);

				TempInfo.CurrentInteractableActor = TempActor;
				break;
			}
		}

		for (const auto& Building : ExistedBuilding)
		{
			if (Building->GetActorLocation() == Info.BuildingLocation)
			{
				TWeakObjectPtr<ABuilding> TempBuilding(Building);

				TempInfo.CurrentBuilding = TempBuilding;
				break;
			}
		}

		for (const auto& Resource : ResourcesOnLand)
		{
			for (const auto& ResObj : Resource.Value.ResourceList)
			{
				if (ResObj->GetActorLocation() == Info.ResourceLocation)
				{
					TWeakObjectPtr<AResource> TempResource(ResObj);

					TempInfo.CurrentResource = TempResource;
					break;
				}
			}
		}

		for (const auto& Hub : MainCore->GetExistedWorkerHubs())
		{
			if (Hub->GetHubID() == Info.HubID)
			{
				TempList.Add(Hub, TempInfo);
			}
		}
	}

	ManagerMakingActions->LoadMakingActionList(MoveTemp(TempList));
}