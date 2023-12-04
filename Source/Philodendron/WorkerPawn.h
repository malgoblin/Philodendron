
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WorkerAchivementStruct.h"

#include "EnumActionList.h"
#include "EnumWorkerType.h"
#include "EnumWorkerAddonType.h"
#include "EnumRecipeType.h"
#include "EnumActionTreeElementName.h"
#include "SupportStructsForSaveLoad.h"
#include "WorkerPawn.generated.h"

class UPaperFlipbookComponent;
class UBoxComponent;

class APointToInterest;
class AWorkerAddon;


class AWorkerHub;
class UWorkerMap;
class UKnownObjectInfo;

enum class EAddonPossitionType;

UCLASS()
class PHILODENDRON_API AWorkerPawn : public AActor
{
	GENERATED_BODY()

public:
	AWorkerPawn();
	void InitWorker();
	virtual void Tick(float DeltaTime) override;


	const TMap<EActionTreeElementName, int32>& GetTreeElementList() const;
	const TArray<TObjectPtr<UKnownObjectInfo>>& GetObjectInfoList() const;
	const TObjectPtr<UWorkerMap>& GetWorkerMap() const;

	const FString& GetWorkerName() const;
	void SetWorkerName(const FString& Name);

	bool AddNewAddon(TObjectPtr<AWorkerAddon> NewAddon, EAddonPossitionType AType);
	bool RemoveAddon(TObjectPtr<AWorkerAddon> NewAddon);
	const TMap<EAddonPossitionType, TObjectPtr<AWorkerAddon>>& GetWorkerAddon() const;
	bool HasAddon(EWorkerAddonType AddonType) const;

	bool IsWorkerKnownAboutInteractableActor(const TObjectPtr<UKnownObjectInfo>& Target) const;


	//bool TakeResource(AResourcePhysicalModel* Resource);
	//bool PutResource(const FVector& Target);

	void SetMasterHub(const TObjectPtr<AWorkerHub>& WHub);
	void RemoveMasterHub();

	AWorkerHub* GetWorkerMasterHub() const;

	void SetReadOnly(bool IsReadOnly);

	bool GetReadOnly() const;
	bool GetMemoryBroken() const;

	void SetMemoryBroken();


	void ClearWorker();

	void SetWorkerType(EWorkerType WType);

	void SetWorkerHidden(bool bIsHidden);

	int32 GetPawnMemorySize() const;
	int32 GetPawnMaxMemorySize() const;

	bool AddFullTreeElementContainsInPawn(EActionTreeElementName TreeElement, int32 FullSize);
	bool AddPartOfTreeElement(EActionTreeElementName Name, int32 Part);
	void RemoveTreeElementFromMemory(EActionTreeElementName TreeElement);

	bool AddNewObjectInfo(const TObjectPtr<UKnownObjectInfo>& NewObj);
	bool AddPartOfKnownObject(const TObjectPtr<UKnownObjectInfo>& NewObj, int32 Part);
	void RemoveObjectInfo(const TObjectPtr<UKnownObjectInfo>& NewObj);
	void RemovePartOfObjectInto(const TObjectPtr<UKnownObjectInfo>& NewObj);


	bool AddWorkerMap(const TObjectPtr<UWorkerMap>& WMap);
	bool AddPartOfMap(const TObjectPtr<UWorkerMap>& WMap, int32 Part);
	void RemoveWorkerMap();
	void RemovePartOfMap();

	void GiveAllAddonsToCore();

	int32 GetPawnID() const;
	void SetPawnID(int32 ID);

	const FSupportPawnInfo& GetPawnInfo() const;

	int32 GetWorkerMapID() const;
	int32 GetPartOfWorkerMapID() const;
	int32 GetPartOfMapSize() const;

	const TMap<EActionTreeElementName, int32>& GetPartOfTreeElement() const;
	
	const TArray<TObjectPtr<UKnownObjectInfo>>& GetObjectList() const;

	const TMap<TObjectPtr<UKnownObjectInfo>, int32>& GetPartOfObjectInfo() const;

	void LoadPawnInfo(FSupportPawnInfo&& Info);
	
	void LoadWorkerMap(TObjectPtr<UWorkerMap> FullWorkerMap, TObjectPtr<UWorkerMap> PartOfWorkerMap, int32 PartSize);

	void LoadKnownObjects(TArray<TObjectPtr<UKnownObjectInfo>>&& FullKnownObject, TMap<TObjectPtr<UKnownObjectInfo>, int32>&& PartOfObject);

	const TObjectPtr<AWorkerHub> GetMasterHub() const;

protected:
	virtual void BeginPlay() override;

private:
	void RemoveTreeElement(EActionTreeElementName TreeElement);
	void RemovePartOfTreeElement(EActionTreeElementName TreeElement);

private:	

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperFlipbookComponent> WorkerPawnFlipbookComponent = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollisionComponent = nullptr;

	UPROPERTY()
	TObjectPtr<AWorkerHub> MasterHub = nullptr;

	UPROPERTY()
	FSupportPawnInfo PawnInfo;

	UPROPERTY()
	TObjectPtr<UWorkerMap> WorkerMap = nullptr;

	UPROPERTY()
	TObjectPtr<UWorkerMap> PartOfMap = nullptr;
	
	UPROPERTY()
	int32 PartOfMapSize = 0;

	UPROPERTY()
	TMap<EAddonPossitionType, TObjectPtr<AWorkerAddon>> ConnectedAddon;
	
	UPROPERTY()
	TMap<EAddonPossitionType, FString> AddonSocketName;

	UPROPERTY()
	TArray<TObjectPtr<UKnownObjectInfo>> ObjectList;

	UPROPERTY()
	TMap<TObjectPtr<UKnownObjectInfo>, int32> PartOfObjectInfo;

};
