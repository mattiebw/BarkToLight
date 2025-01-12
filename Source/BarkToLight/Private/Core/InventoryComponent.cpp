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

bool UInventoryComponent::GiveWeapon(AWeapon* Weapon)
{
	for (int i = 0; i < Weapons.Num(); i++)
	{
		if (Weapons[i] == nullptr)
		{
			Weapons[i] = Weapon;
			return true;
		}
	}

	return false;
}

TArray<AWeapon*> UInventoryComponent::SetWeaponSlots(int Slots)
{
	TArray<AWeapon*> Culled;

	if (Slots < Weapons.Num())
	{
		for (int i = Slots; i < Weapons.Num(); i++)
		{
			Culled.Add(Weapons[i]);
			Weapons[i] = nullptr;
		}
	}

	Weapons.SetNum(Slots);

	return Culled;
}
