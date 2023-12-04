#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumTypeOfActionAndActor.h"
#include "SupportStructsForSaveLoad.h"
#include "EnumAnimationType.h"
#include "InteractableActor.generated.h"

enum class EInteractableActorType;
enum class EActionType;
enum class EResourceType;


class AWorkerHub;
class UPaperFlipbookComponent;
class UBoxComponent;
class UPaperFlipbook;

struct FPlantAnimationStruct;


UCLASS()
class PHILODENDRON_API AInteractableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteractableActor();
	const TArray<EActionType>& GetActionList() const;
	FVector GetLocation() const;
	EInteractableActorType GetActorType() const;
	TObjectPtr<UPaperFlipbook> GetIActorPaperFlipbook() const;
	void Tick(float DeltaTime);
	bool RemoveInteractableActor();
	bool HasAction(EActionType Type) const;


	const ETypeOfActionAndActor GetActorEvolution() const;
	void SetActionList();

	const TMap<EResourceType, int32>* GetAvailableResourceList() const;

	bool LockResource(EResourceType ResourceForLock, TObjectPtr<AWorkerHub> WHub);
	void FreeResourceLockedHub(TObjectPtr<AWorkerHub> WHub);

	bool InitInteractableActor(EInteractableActorType Type, ETypeOfActionAndActor GlobalType, bool IsRemoveAfterEjectResources, const FPlantAnimationStruct* FlipbookStruct, int32 FlipbookNumber);

	bool CreateNewResource(TObjectPtr<AWorkerHub> WHub);

	bool ExtractAllResources(const TObjectPtr<AWorkerHub>& WHub);

	bool LockActor(const TObjectPtr<AWorkerHub>& WHub);

	bool IsWorkerHubLocked(const TObjectPtr<AWorkerHub>& WHub) const;

	const FSupportInteractableActorInfo& GetInteractableActorInfo() const;

	const TMap<TWeakObjectPtr<AWorkerHub>, EResourceType>& GetLockedResources() const;

	const TWeakObjectPtr<AWorkerHub> GetWorkerHubLockActor() const;

	void LoadInteractableActor(FSupportInteractableActorInfo&& Info);

protected:
	void AddResources();
	virtual void BeginPlay() override;

private:

protected:
	UPROPERTY()
	TObjectPtr<UPaperFlipbookComponent> InteractableActorFlipbookComponent;

	UPROPERTY()
	TObjectPtr<UBoxComponent> BoxCollisionComponent;

	const FPlantAnimationStruct* FlipbooksAndResourcesLists = nullptr;

	UPROPERTY()
	FSupportInteractableActorInfo IAInfo;

	UPROPERTY()
	TMap<TWeakObjectPtr<AWorkerHub>, EResourceType> LockedResources;

	UPROPERTY()
	TWeakObjectPtr<AWorkerHub> WorkerHubLockActor = nullptr;
};
