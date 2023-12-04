
#include "ManagerAddons.h"
#include "MainGameStateBase.h"


AManagerAddons::AManagerAddons()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AManagerAddons::BeginPlay()
{
	Super::BeginPlay();
	Cast<AMainGameStateBase>(this->GetWorld()->GetGameState())->SetManagerAddons(this);
	
}

void AManagerAddons::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


const FDeviceAnimationStuct* AManagerAddons::GetAddonFlipbooks(EWorkerAddonType AddonType, EWorkerType WType) const
{
	if (!AddonList.Contains(AddonType))
		return nullptr;

	if (!AddonList[AddonType].AddonFlipbooks.Contains(WType))
		return nullptr;

	return &AddonList[AddonType].AddonFlipbooks[WType];
}

const TMap<EAddonModifiers, float>* AManagerAddons::GetAddonModifiers(EWorkerAddonType Type) const
{
	if (!AddonList.Contains(Type))
		return nullptr;

	return &AddonList[Type].AddonModifiers;
}

const FString* AManagerAddons::GetAddonName(EWorkerAddonType Type, EPossibleLanguages Language) const
{
	if (!AddonList.Contains(Type))
		return nullptr;

	return AddonList[Type].AddonName.Find(Language);
}

const TObjectPtr<UTexture> AManagerAddons::GetAddonIcon(EWorkerAddonType Type) const
{
	if (!AddonList.Contains(Type))
		return nullptr;

	return AddonList[Type].AddonIcon;
}

const TObjectPtr<UTexture> AManagerAddons::GetAddonImage(EWorkerAddonType Type) const
{
	if (!AddonList.Contains(Type))
		return nullptr;

	return AddonList[Type].AddonImage;
}