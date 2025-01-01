// copyright lolol

#include "Core/InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	// Inventories don't need to tick - they are just data holders
	PrimaryComponentTick.bCanEverTick = false;
}

int UInventoryComponent::TakeAmmo(FName AmmoType, int AmmoAmount)
{
	if (!AmmoInventory.Contains(AmmoType))
	{
		// We haven't got any of this ammo type. Just return 0 for now.
		return 0;
	}
	
	int& Ammo = AmmoInventory[AmmoType];
	if (Ammo < AmmoAmount)
	{
		// Not enough ammo - take what we can.
		int TakenAmmo = Ammo;
		Ammo = 0;
		return TakenAmmo;
	}

	// We have enough - take all of it.
	Ammo -= AmmoAmount;
	return AmmoAmount;
}

void UInventoryComponent::GiveAmmo(FName AmmoType, int AmmoAmount)
{
	if (!AmmoInventory.Contains(AmmoType))
	{
		AmmoInventory.Add(AmmoType, AmmoAmount);
	}
	else
	{
		AmmoInventory[AmmoType] += AmmoAmount;
	}
}
