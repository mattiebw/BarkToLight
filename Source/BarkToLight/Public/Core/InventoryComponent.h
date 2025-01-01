// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

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

protected:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TMap<FName, int> AmmoInventory;
};
