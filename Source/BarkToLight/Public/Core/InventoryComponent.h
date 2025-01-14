// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItem;
class AWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryAmmoUpdated, FName, AmmoType, int, NewAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponSlotsUpdated);

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
	bool GiveWeapon(AWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<AWeapon*> SetWeaponSlots(int Slots);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	int GetFreeWeaponSlots();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FORCEINLINE int GetWeaponSlots() { return Weapons.Num(); }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetWeapon(int Slot, AWeapon* Weapon);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void GiveItem(UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void Clear();

	FORCEINLINE const TMap<FName, int>& GetAmmoInventory() const { return AmmoInventory; }

	FORCEINLINE const TArray<UItem*>& GetItems() const { return Items; }

	FORCEINLINE const TArray<AWeapon*>& GetWeapons() const { return Weapons; }

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryAmmoUpdated OnAmmoUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnWeaponSlotsUpdated OnWeaponSlotsUpdated;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TMap<FName, int> AmmoInventory;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Inventory")
	TArray<UItem*> Items;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<AWeapon*> Weapons;
};
