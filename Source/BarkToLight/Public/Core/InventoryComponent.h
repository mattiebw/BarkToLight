// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryAmmoUpdated, FName, AmmoType, int, NewAmmo);

UCLASS(ClassGroup=(BTL), meta=(BlueprintSpawnableComponent))
class BARKTOLIGHT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int TakeAmmo(FName AmmoType, int AmmoAmount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void GiveAmmo(FName AmmoType, int AmmoAmount);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FORCEINLINE bool HasAnyAmmo(FName AmmoType) const
	{
		return AmmoInventory.Contains(AmmoType) && AmmoInventory[AmmoType] > 0;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FORCEINLINE int GetAmmo(FName AmmoType) const
	{
		return AmmoInventory.Contains(AmmoType) ? AmmoInventory[AmmoType] : 0;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FORCEINLINE int GetAmmoSlotCount() const { return Weapons.Num(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FORCEINLINE AWeapon* GetWeapon(int Index) const { return Weapons.IsValidIndex(Index) ? Weapons[Index] : nullptr; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE bool GiveWeapon(AWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<AWeapon*> SetWeaponSlots(int Slots);

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryAmmoUpdated OnAmmoUpdated;

protected:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TMap<FName, int> AmmoInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<AWeapon*> Weapons;
};
