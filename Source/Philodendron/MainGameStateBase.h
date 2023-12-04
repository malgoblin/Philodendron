#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "EnumWorkerAddonType.h"

#include "EnumTypeOfActionAndActor.h"
#include "EnumWorkerAddonType.h"
#include "EnumInteractableActorType.h"
#include "EnumPossibleLanguages.h"
#include "EnumBuildingType.h"
#include "EnumAnimalType.h"
#include "EnumAnimationType.h"
#include "EnumWorkerType.h"
#include "EnumRecipeType.h"

#include "WorkerAchivementStruct.h"
#include "ManagerActionTree.h"
#include "MainGameStateBase.generated.h"

class UMultiAction;
class AMainPlayer;

class AWorkerAddon;
class AResource;
class UResource;
class AWorkerHub;
class ABuilding;
class AWorkerPawn;
class AInteractableActor;

class AObjectManager;
class ALevelGenerator;
class UPaperFlipbook;
class UTextureRenderTarget2D;

struct FActionParametrs;

enum class EActionTreeElementName;

enum class EActionType;
enum class ERecipeType;

enum class EResourceType;

class AManagerActionTree;
class AManagerActions;
class AManagerRecipe;
class AManagerInteractableActors;
class AManagerResources;
class AManagerMakingActions;
class AManagerAddons;
class AManagerBuilding;

struct FRandomRange;

class APointLight;
class ADirectionalLight;
class UMainSaveGame;

USTRUCT()
struct FResourceList
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<TObjectPtr<AResource>> ResourceList;
};

UCLASS()
class PHILODENDRON_API AMainGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	AMainGameStateBase();

	FVector GetCoreLocation() const;
	TWeakObjectPtr<AMainPlayer> GetCore() const;

	TArray<TWeakObjectPtr<AInteractableActor>> FindSpottingInteractableActorPossibleForAction(FVector Source, float RadiusSq) const;


	void RemoveExistedActor(AInteractableActor* InteractableActor);

	TObjectPtr<AWorkerAddon> CreateNewAddon(EWorkerAddonType AddonType, const FVector& Location);

	const TArray<TObjectPtr<AWorkerAddon>>& GetAddonList() const;

	bool CreateNewResource(EResourceType ResourceType, const FVector& Source, float Distance);

	TObjectPtr<AResource> GenerateNewResource(EResourceType ResourceType, const FVector& Location);

	bool IsPointEmpty(const FVector& Point, float EmptyArea) const;

	void SetObjectManager(const TObjectPtr<AObjectManager>& OManager);

	TObjectPtr<UPaperFlipbook> GetWorkerPawnFlipbook(EWorkerType WType);
	const FDeviceAnimationStuct* GetAddonFlipbooks(EWorkerAddonType AddonType, EWorkerType WType);

	void SetLevelGenerator(const TObjectPtr<ALevelGenerator>& LGenerator);

	bool CreateCore(const FVector& Location);

	bool CreateNewInteractableActor(EInteractableActorType Type, FVector Location, int32 ObjectNumber, EInteractableActorCreatingType CreatingType);

	bool CreateInteractableActorFromRecipe(EInteractableActorType Type, FVector Location);

	bool CreateBuildingOnInteractableActor(EBuildingType Type, TWeakObjectPtr<AInteractableActor> IActor, AWorkerHub* WHub);
	bool CreateBuildingOnEmptyPlace(EBuildingType Type, FVector Location);

	TObjectPtr<UTextureRenderTarget2D> GetInterfaceCam() const;

	bool AddAddonsToCore(TObjectPtr<AWorkerAddon> NewAddon);
	bool AttachWorkerToCore(TObjectPtr<AWorkerPawn> WPawn);


	bool GetResourceToWorker(TObjectPtr<AWorkerHub>& WHub, TWeakObjectPtr<AResource> Res);


	void GenerateActionForInteractableActor();
	void GenerateStartedAddons(const TMap<EWorkerAddonType, int32>& StartedAddons);
	void GenerateStartedWorkers(int32 WorkerCount);

	TObjectPtr<UPaperFlipbook> GetWorkerHubFlipbook();

	TWeakObjectPtr<AResource> FindClosestResourceInArea(const TArray<EResourceType>& TypeList, const FVector& Source, int32 RadiusSq) const;

	TArray<TWeakObjectPtr<AInteractableActor>> FindInteractableActorInArea(EInteractableActorType ActorType, const FVector& Source, int32 RadiusSq) const;
	TArray<TWeakObjectPtr<ABuilding>> FindBuildingInArea(EBuildingType BuildType, const FVector& Source, int32 RadiusSq) const;

	bool IsResourceExist(const TObjectPtr<AResource>& Resource) const;

	void SetManagerActionTree(const TObjectPtr<AManagerActionTree>& Manager);
	
	bool CreateHole(const FVector& Location);

	void CreateSun(FVector&& MapEdge);



	int32 GetWorkerPawnMemorySize() const;
	float GetDefaultWorkerSpeed() const;
	float GetDefaultWorkerSpottingRadius() const;
	float GetWorkerRestTime() const;
	int32 GetMinimalFoVCamera() const;
	int32 GetMaxFoVCamera() const;
	const TMap<EPossibleLanguages, FString>* GetAvailableLanguages() const;
	const TObjectPtr<UTexture2D>& GetWorkerPawnIcon() const;
	const TObjectPtr<UTexture2D>& GetWorkerPawnImage() const;
	const TObjectPtr<UTexture2D>& GetWorkerHubIcon() const;
	const TObjectPtr<UTexture2D>& GetWorkerHubImage() const;



	void SetGameLanguages(EPossibleLanguages Languages);
	EPossibleLanguages GetGameLanguages() const;

	//Level Generator
	void GenerateMainLevel();


	// Action Tree Manager
	const TMap<EActionTreeElementName, FElementCost>& GetActionTreeElementCosts() const;
	const TMap<EActionTreeElementName, FElementReward>& GetActionTreeElementReward() const;
	const TMap<EActionTreeElementName, FLinkedElements>& GetActionTreeElementParents() const;
	const TMap<EActionTreeElementName, FLinkedElements>& GetActionTreeElementChildren() const;
	EActionTreeElementName GetActionTreeElementNameForActionName(EActionType AType) const;
	EActionTreeElementName GetActionTreeElementNameForRecipeName(ERecipeType RType) const;
	int32 GetTreeElementSize(EActionTreeElementName TreeElementName) const;
	const FElementReward* GetRewardForTreeElement(EActionTreeElementName TreeElement) const;



	//Action Manager
	void SetManagerActions(const TObjectPtr<AManagerActions>& Manager);
	int32 GetActionSize(EActionType Type) const;
	float GetActionTime(EActionType Type) const;
	bool NeedInteractableActorForAction(EActionType Type) const;
	const TMap<EWorkerAddonType, int32>* GetConditionAddons(EActionType Type) const;
	const TArray<EResourceType>* GetResourcesMinedAction(EActionType Type) const;
	const FPlantAnimationStruct* GetAnimationForIActor(EInteractableActorType Type, int32 ObjectNumber) const;
	int32 GenerateRandomObject(EInteractableActorType Type) const;
	int32 GetDistanceToMakeAction(EActionType Type) const;
	const FString* GetActionName(EActionType Type) const;


	//Recipe Manager
	void SetManagerRecipe(const TObjectPtr<AManagerRecipe>& Manager);
	int32 GetRecipeSize(ERecipeType Type) const;
	const TMap<EResourceType, int32>* GetNeededResourcesForRecipe(ERecipeType Type) const;
	float GetTimeToMakeRecipe(ERecipeType Type) const;
	EPlaceWhereMakingRecipe GetPlaceWhereMakingRecipe(ERecipeType Type) const;
	float GetEmptyAreaAroundEmptyPlace(ERecipeType Type) const;
	EGlobalRecipeType GetGlobalRecipeType(ERecipeType Type) const;
	EInteractableActorType GetInteractableActorForCreating(ERecipeType Type) const;
	EBuildingType GetNeededBuildingForCretingRecipe(ERecipeType Type) const;
	EInteractableActorType GetNeededInteractableActorWhereCreateRecipe(ERecipeType Type) const;
	EBuildingType GetBuildingWhichCreating(ERecipeType Type) const;
	EResourceType GetResourceWhichCreating(ERecipeType Type) const;
	ENeedWorkerToMakeRecipe GetNeededWorkerToMakeRecipe(ERecipeType Type) const;
	const FString* GetRecipeName(ERecipeType Type) const;


	//Interactable Actor Manager
	void SetManagerInteractableActor(const TObjectPtr<AManagerInteractableActors>& Manager);
	const TArray<EActionType>* GetAvailableActions(EInteractableActorType Type) const;
	int32 GetSizeInfoAboutIActor(EInteractableActorType Type) const;
	const TArray<EActionType>& GetKnownActionInCore() const;
	float GetDistanceForCreateResources(EInteractableActorType Type) const;
	FVector3d GetBoxSizeFinalFlipbook(EInteractableActorType Type, int32 ObjectNumber) const;
	const FString* GetInteractableActorName(EInteractableActorType Type) const;


	//Resources Manager
	void SetManagerResources(const TObjectPtr<AManagerResources>& Manager);

	//Making Actions Manager
	void SetManagerMakingActions(const TObjectPtr<AManagerMakingActions>& Manager);
	bool PrepairToNewAction(TObjectPtr<AWorkerHub> WHub, EActionType Type, TWeakObjectPtr<AInteractableActor>& IActor, ERecipeType RecipeType);
	bool ContinueToPrepair(TObjectPtr<AWorkerHub> WHub);

	//Manager Addons
	void SetManagerAddons(const TObjectPtr<AManagerAddons>& Manager);
	const TMap<EAddonModifiers, float>* GetAddonModifiers(EWorkerAddonType Type) const;
	const FString* GetAddonName(EWorkerAddonType Type) const;
	const TObjectPtr<UTexture> GetAddonIcon(EWorkerAddonType Type) const;
	const TObjectPtr<UTexture> GetAddonImage(EWorkerAddonType Type) const;


	//Manager Building
	void SetManagerBuilding(const TObjectPtr<AManagerBuilding>& Manager);

	//SAVE GAME
	void SaveGame();
	void SaveCore(TObjectPtr<UMainSaveGame>& SaveFile);
	void SaveInteractableActors(TObjectPtr<UMainSaveGame>& SaveFile);
	void SavePawns(TObjectPtr<UMainSaveGame>& SaveFile);
	void SaveAddons(TObjectPtr<UMainSaveGame>& SaveFile);
	void SaveResources(TObjectPtr<UMainSaveGame>& SaveFile);
	void SaveHubs(TObjectPtr<UMainSaveGame>& SaveFile);
	void SaveBuilding(TObjectPtr<UMainSaveGame>& SaveFile);
	void SaveMakingAction(TObjectPtr<UMainSaveGame>& SaveFile);

	//LOAD GAME
	bool LoadGame();
	void LoadCore(TObjectPtr<UMainSaveGame>& SaveFile);
	void LoadInteractableActors(TObjectPtr<UMainSaveGame>& SaveFile);
	void LoadPawns(TObjectPtr<UMainSaveGame>& SaveFile);
	void LoadAddons(TObjectPtr<UMainSaveGame>& SaveFile);
	void LoadAddonsToCore(TObjectPtr<UMainSaveGame>& SaveFile);
	void LoadResources(TObjectPtr<UMainSaveGame>& SaveFile);
	void LoadHubs(TObjectPtr<UMainSaveGame>& SaveFile);
	void LoadAttachedHubPawnToCore(TObjectPtr<UMainSaveGame>& SaveFile);
	void LoadBuilding(TObjectPtr<UMainSaveGame>& SaveFile);
	void LoadMakingAction(TObjectPtr<UMainSaveGame>& SaveFile);

private:
	bool FindPlaceForNewResource(FVector* Location, const FVector& Source, float Distance);

	void GenerateSunInPossition(FVector&& Possition);

private:
	UPROPERTY()
	TObjectPtr<AObjectManager> ObjManager = nullptr;

	UPROPERTY()
	TObjectPtr<AManagerActions> ManagerActions = nullptr;

	UPROPERTY()
	TObjectPtr<AManagerActionTree> ManagerActionTree = nullptr;

	UPROPERTY()
	TObjectPtr<AManagerRecipe> ManagerRecipe = nullptr;

	UPROPERTY()
	TObjectPtr<AManagerInteractableActors> ManagerIActors = nullptr;

	UPROPERTY()
	TObjectPtr<AManagerResources> ManagerResources = nullptr;

	UPROPERTY()
	TObjectPtr<AManagerAddons> ManagerAddons = nullptr;

	UPROPERTY()
	TObjectPtr<AManagerBuilding> ManagerBuilding = nullptr;

	UPROPERTY()
	TObjectPtr<AManagerMakingActions> ManagerMakingActions = nullptr;

	UPROPERTY()
	TObjectPtr<ALevelGenerator> LvlGenerator = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<AInteractableActor>> ExistedActorsList;

	UPROPERTY()
	TArray<TObjectPtr<ABuilding>> ExistedBuilding;
	
	UPROPERTY()
	TArray<TObjectPtr<AWorkerAddon>> ExistedAddonList;
	
	UPROPERTY()
	float MaxDistToNewResource = 100;

	UPROPERTY()
	TObjectPtr<AMainPlayer> MainCore = nullptr;

	UPROPERTY()
	TMap<EResourceType, FResourceList> ResourcesOnLand;

	UPROPERTY()
	EPossibleLanguages GameLanguages = EPossibleLanguages::Invalid;

	UPROPERTY()
	TObjectPtr<APointLight> PLight = nullptr;

	UPROPERTY()
	TObjectPtr<ADirectionalLight> DLight = nullptr;
};
