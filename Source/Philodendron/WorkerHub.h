// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnumWorkerType.h"
#include "EnumResourceType.h"
#include "EnumInteractableActorType.h"
#include "WorkerAchivementStruct.h"
#include "InterfaceMovablePawn.h"
#include "HubLog.h"
#include "EnumActionTreeElementName.h"
#include "EnumWorkerHubStatus.h"
#include "EnumBuildingType.h"
#include "SupportStructsForSaveLoad.h"
#include "WorkerHub.generated.h"

enum class EActionType;
enum class ERecipeType;
enum class EWorkerAddonType;

class UPaperFlipbookComponent;
class UPaperFlipbook;
class UBoxComponent;

class AMainAIController;
class UKnownObjectInfo;
class AWorkerPawn;
class AWorkerAddon;
class AResource;
class UWorkerMap;
class AInteractableActor;
class ABuilding;

USTRUCT()
struct FPawnList
{
	GENERATED_BODY()

public:
	TArray<TObjectPtr<AWorkerPawn>> PawnList;
};

UCLASS()
class PHILODENDRON_API AWorkerHub : public APawn
{
	GENERATED_BODY()

public:
	AWorkerHub();
	void InitWorkerHub(const FString& HName);
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	bool AttachWorker(TObjectPtr<AWorkerPawn> WPawn, EWorkerType WType);
	void FreeWorker(TObjectPtr<AWorkerPawn> WPawn);

	bool MoveToPointOnDistance(const FVector& Point, int32 Dist);


	bool PrepairToAction(EActionType Type, TWeakObjectPtr<AInteractableActor> IActor, ERecipeType RecipeType);
	bool CompletedAction(EActionType Type, EInteractableActorType ActorType);
	void MakeRecipeCompleted(ERecipeType Recipe);


	bool RemoveAddon(TObjectPtr<AWorkerAddon> Addon);

	const TMap<EWorkerType, TObjectPtr<AWorkerPawn>>& GetConnectedWorkers() const;

	bool AddNewTreeElementListToMemory(const TArray<EActionTreeElementName>& TreeElementList);

	void RemoveTreeElementListFromMemory(const TArray<EActionTreeElementName>& TreeElementList);

	bool AddNewObjectInfo(const TObjectPtr<UKnownObjectInfo>& ObjInfo);
	void RemoveKnownObjectInfo(const TObjectPtr<UKnownObjectInfo> ObjInfo);

	void StartWork();

	void ClearWorker();

	float GetWorkerSpottingRadius() const;
	float GetWorkerSpottingRadiusSq() const;

	const TArray<ERecipeType>& GetRecipeList() const;
	const TArray<EActionType>& GetActionList() const;
	
	TArray<TObjectPtr<UKnownObjectInfo>> GetInfoList() const;

	//bool TakeResource(AResourcePhysicalModel* Resource);
	//bool PutResource(const FVector& Target);

	void RemoveHub();

	const TArray<FString>& GetHubLogs() const;

	void SetHubHidden(bool bIsHidden);

	const FString& GetHubName() const;

	bool CanMakeAction(EActionType Type) const;


	int32 GetSizeTreeToTreeElementWithoutTreeElementList(EActionTreeElementName Name, const TArray<EActionTreeElementName>& SuperfloursList) const;

	bool HasTreeElement(EActionTreeElementName Name) const;

	int32 GetHubMemotySize() const;
	int32 GetMaxHubMemotySize() const;

	TArray<EActionTreeElementName> GetAllUnknownTreeElementsListToTreeElement(EActionTreeElementName Name) const;
	TArray<EActionTreeElementName> GetAllDeletableTreeElementListToTreeElement(EActionTreeElementName Name) const;
	void RemoveTreeElementsFromList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const;
	void RemoveTreeElementsFromRemovableList(TArray<EActionTreeElementName>& List, EActionTreeElementName ElementForRemove) const;
	void ChangePriorityAfterDoAction(EActionType Type);


	void ClearWorkerAchivements();

	bool IsKnownObject(TWeakObjectPtr<AInteractableActor> IActor) const;

	bool TakeResource(TObjectPtr<AResource> Resource);

	void TurnOnHub();
	void TurnOffHub();

	void SetHubStatus(EWorkerHubStatus HStatus);

	void SetHubRest();
	EWorkerHubStatus GetHubStatus() const;

	bool HasPawn(TObjectPtr<AWorkerPawn> WPawn) const;

	void GiveAllPawnAndAddonsToCore();

	bool CanTreeElementRemoved(EActionTreeElementName Name) const;

	int32 GetCountPossessedResource(EResourceType Type) const;

	TArray<TObjectPtr<UKnownObjectInfo>> GetAllKnownInteractableActor() const;

	TArray<TObjectPtr<UKnownObjectInfo>> GetKnownObject() const;

	FVector GetRandomPointInRadiusAroundPoint(const FVector& Destination, int32 Radius) const;

	bool SpendResourcesForRecipe(ERecipeType Type);

	TArray<TWeakObjectPtr<AInteractableActor>> GetKnownInteractableActorByType(EInteractableActorType Type) const;

	TWeakObjectPtr<AInteractableActor> FindInteractableActorForCreateRecipe(EInteractableActorType Type) const;
	TWeakObjectPtr<ABuilding> FindBuildingForCreateRecipe(EBuildingType Type) const;

	int32 GetHubID() const;
	void SetHubID(int32 NewID);


	const FSupportHubInfo& GetHubInfo() const;
	const TArray<TObjectPtr<AResource>>& GetPossessedResourceList() const;
	const TMap<EWorkerType, TObjectPtr<AWorkerPawn>>& GetConnectedPawn() const;

	void SetHubInfo(FSupportHubInfo&& Info);

	void LoadWorkerPawn(TObjectPtr<AWorkerPawn> WPawn, EWorkerType WType);

	const FSupportAIInfo& GetAIInfo() const;
	void LoadAIInfo(FSupportAIInfo&& Info);


protected:
	virtual void BeginPlay() override;

private:
	void UpdateVelocity();

	bool ConnectPawnToSocket(TObjectPtr<AWorkerPawn> WPawn, EWorkerType WType);

	void GetAddonListFromPawn(TObjectPtr<AWorkerPawn> WPawn, TArray<TObjectPtr<AWorkerAddon>>& AList) const;

	void AddTreeElement(EActionTreeElementName Name);

	void UpdateAvailableHubMemory();

	void UpdateHubActionAndRecipeList();

	void UpdateAddonModificators();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbookComponent> WorkerHubFlipbookComponent = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbook> WorkerHubFlipbook = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollisionComponent = nullptr;

	UPROPERTY(EditAnywhere)
	FSupportHubInfo HubInfo;

	UPROPERTY()
	TArray<TObjectPtr<AResource>> PossessedResourceList;

	UHubLog HubLog;
	
	UPROPERTY()
	TMap<EWorkerType, TObjectPtr<AWorkerPawn>> ConnectedPawn;
	
	UPROPERTY()
	TMap<EWorkerType, FString> SocketPossition;

	UPROPERTY()
	TMap<EActionTreeElementName, FPawnList> TreeElementListWithPawn;

	UPROPERTY()
	TMap<TObjectPtr<UKnownObjectInfo>, FPawnList> KnownObjectInPawns;
};
