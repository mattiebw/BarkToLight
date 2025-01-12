// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "WeaponStats.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

class AWeapon;

/**
 * The type of ammo usage this weapon has.
 * This is mainly used to determine how to display the ammo count in the UI.
 */
UENUM()
enum class EAmmoUsageType : uint8
{
	InfiniteAmmo, // Infinite ammo, or no concept of ammo (e.g. tf2 swords)
	ClipAndReserve, // Ammo is stored in a clip and a reserve pool (e.g. tf2 shotgun)
	SinglePool, // Ammo is stored in a single pool (e.g. tf2 minigun)
	Charge, // Ammo is stored in a charge pool (e.g. tf2 medic gun)
};

/**
 * Data describing the basic properties of a weapon, and it's default stats.
 */
UCLASS()
class BARKTOLIGHT_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponSubclass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	EAmmoUsageType AmmoUsageType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FName AmmoType;
	
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Weapon")
	UWeaponStats* DefaultStats;
};
