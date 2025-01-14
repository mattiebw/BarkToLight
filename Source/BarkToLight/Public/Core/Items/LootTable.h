// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Core/StatsClass.h"
#include "Engine/DataAsset.h"
#include "LootTable.generated.h"

class UItemAppearance;
class UUpgradeItem;
struct FStatUpgrade;
class UWeaponData;

USTRUCT(BlueprintType)
struct FUpgradeEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStatUpgrade Upgrade;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cost;
};

USTRUCT(BlueprintType)
struct FUpgradeItemEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Table Upgrade Entry")
	TSubclassOf<UUpgradeItem> ItemClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Table Upgrade Entry")
	UItemAppearance* ItemAppearance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table Upgrade Entry")
	float Cost;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table Upgrade Entry")
	TArray<FUpgradeEntry> StatUpgrades;
};

USTRUCT(BlueprintType)
struct FWeaponEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table Weapon Entry")
	UWeaponData* WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table Weapon Entry")
	float Cost;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table Weapon Entry")
	TArray<FUpgradeEntry> StatUpgrades;
};

USTRUCT(BlueprintType)
struct FAmmoEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Table Ammo Entry")
	FName Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Table Ammo Entry")
	float Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot Table Ammo Entry")
	FVector2f Range = FVector2f(40, 50);
};

/**
 * This data class represents the possible items and weapons that can be found in a loot chest.
 * Used by ULootGenerator.
 */
UCLASS(BlueprintType)
class BARKTOLIGHT_API ULootTable : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table")
	TArray<FWeaponEntry> Weapons;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table")
	TArray<FUpgradeItemEntry> Upgrades;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Table")
	TArray<FAmmoEntry> Ammo;
};
