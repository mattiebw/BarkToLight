// copyright lolol

#include "Core/Items/LootGenerator.h"

#include "Core/InventoryComponent.h"
#include "Core/Items/LootTable.h"
#include "Weapon/Weapon.h"

void ULootGenerator::GenerateLootIntoInventory(UInventoryComponent* InventoryComponent, float Luck)
{
	while (Luck > 0)
	{
		float Roll = FMath::FRand();

		if (Roll < 0.4f)
		{
			// Ammo
			if (LootTable->Ammo.IsEmpty())
				continue;
			FAmmoEntry& Entry = LootTable->Ammo[FMath::RandRange(0, LootTable->Ammo.Num() - 1)];

			Luck -= Entry.Cost;

			InventoryComponent->GiveAmmo(Entry.Type, FMath::RandRange(Entry.Range.X, Entry.Range.Y));
		} else if (Roll < 0.8f)
		{
			// Upgrade
		} else
		{
			// Weapon
			if (LootTable->Weapons.IsEmpty())
				continue;
			
			FWeaponEntry& Entry = LootTable->Weapons[FMath::RandRange(0, LootTable->Weapons.Num() - 1)];
			Luck -= Entry.Cost;

			AWeapon* NewWeapon = AWeapon::CreateWeapon(this, Entry.WeaponData);
			float LuckForUpgrades = FMath::FRandRange(Entry.Cost, Entry.Cost * 2);

			while (LuckForUpgrades > 0)
			{
				FUpgradeEntry& Upgrade = Entry.StatUpgrades[FMath::RandRange(0, Entry.StatUpgrades.Num() - 1)];
				LuckForUpgrades -= Upgrade.Cost;
				NewWeapon->GetStats()->AddUpgrade(Upgrade.Upgrade);
			}

			if (InventoryComponent->GetFreeWeaponSlots() == 0)
				InventoryComponent->SetWeaponSlots(InventoryComponent->GetWeaponSlots() + 1);
			InventoryComponent->GiveWeapon(NewWeapon);
		}
	}
}
