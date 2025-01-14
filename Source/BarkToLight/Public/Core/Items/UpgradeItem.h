// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Equippable.h"
#include "Item.h"
#include "UpgradeItem.generated.h"

class UStatsClass;
struct FStatUpgrade;

/**
 * Base class for items that can upgrade a UStatsClass.
 */
UCLASS()
class BARKTOLIGHT_API UUpgradeItem : public UItem, public IEquippable
{
	GENERATED_BODY()
	
public:
	// -- IEquippable --
	virtual void Equip_Implementation(UObject* EquipTo) override;
	virtual void Unequip_Implementation(UObject* EquipTo) override;
	// -----------------
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	TArray<FStatUpgrade> Upgrades;

	// This function is implemented by subclasses so that the upgrade item knows what stats class to upgrade.
	// For example, a weapon upgrade needs to pull the UWeaponStats from the object, while the player upgrade needs to pull the UPlayerStats.
	// This function should return nullptr if the object is not of the correct type, so we can error out properly.
	virtual UStatsClass* GetStatsClassForObject(UObject* Object);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
	TArray<int> Handles;
};
