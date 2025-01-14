// copyright lolol

#include "Layer/WorldGen/LootChest.h"

#include "Core/HealthComponent.h"
#include "Core/InteractableComponent.h"
#include "Core/InventoryComponent.h"
#include "Weapon/Weapon.h"

ALootChest::ALootChest()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	RootComponent = MeshComponent;
}

void ALootChest::BeginPlay()
{
	Super::BeginPlay();

	InteractableComponent->OnInteractedWith.AddUniqueDynamic(this, &ALootChest::OnInteractedWith);
}

void ALootChest::OnInteractedWith(ABTLPlayerCharacter* Player)
{
	if (bLooted)
		return;
	
	InteractableComponent->DestroyComponent();
	bLooted = true;

	Player->GetHealthComponent()->SetHealthPercentage(1, "Looted a chest");
	Player->LootInventory(InventoryComponent);
	OnLooted();
}

void ALootChest::Destroyed()
{
	Super::Destroyed();

	for (AWeapon* Weapon : InventoryComponent->GetWeapons())
	{
		Weapon->Destroy();
	}
}
