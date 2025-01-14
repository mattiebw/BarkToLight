// copyright lolol

#include "Core/Items/LootGenerator.h"

#include "Core/InventoryComponent.h"
#include "Core/Items/LootTable.h"
#include "Core/Items/UpgradeItem.h"
#include "Weapon/Weapon.h"

void ULootGenerator::GenerateLootIntoInventory(UInventoryComponent* InventoryComponent, float Luck)
{
	// While we have enough luck left, we'll continue adding loot to the pool.
	while (Luck > 0)
	{
		// This roll will determine what kind of item we get.
		// MW @todo: This shouldn't be hardcoded.
		float Roll = FMath::FRand();

		if (Roll < 0.4f)
		{
			// Ammo!
			// Take a random ammo entry from the loot table, and add it to the inventory.
			if (LootTable->Ammo.IsEmpty())
				continue;
			FAmmoEntry& Entry = LootTable->Ammo[FMath::RandRange(0, LootTable->Ammo.Num() - 1)];

			Luck -= Entry.Cost;

			InventoryComponent->GiveAmmo(Entry.Type, FMath::RandRange(Entry.Range.X, Entry.Range.Y));
		} else if (Roll < 0.7f)
		{
			// Upgrade
			// Take a random upgrade entry from the loot table, roll its upgrades, and add it to the inventory.
			if (LootTable->Upgrades.IsEmpty())
				continue;

			FUpgradeItemEntry& Entry = LootTable->Upgrades[FMath::RandRange(0, LootTable->Upgrades.Num() - 1)];
			Luck -= Entry.Cost;

			UUpgradeItem* NewUpgrade = NewObject<UUpgradeItem>(InventoryComponent, Entry.ItemClass);
			NewUpgrade->Appearance = Entry.ItemAppearance;

			// For now, we use double the cost of the item as the luck for the upgrades.
			// Again, @todo, this should not be hardcoded.
			float LuckForUpgrades = FMath::FRandRange(Entry.Cost, Entry.Cost * 2);
			while (LuckForUpgrades > 0)
			{
				FUpgradeEntry& Upgrade = Entry.StatUpgrades[FMath::RandRange(0, Entry.StatUpgrades.Num() - 1)];
				LuckForUpgrades -= Upgrade.Cost;
				NewUpgrade->Upgrades.Add(Upgrade.Upgrade);
			}

			InventoryComponent->GiveItem(NewUpgrade);
		} else
		{
			// Weapon.
			// Take a random weapon from the pool, apply random upgrades, and add it to the inventory.
			// Same as upgrades, we use double the cost of the item as the luck for the upgrades.
			
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
