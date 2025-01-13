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
	
	virtual UStatsClass* GetStatsClassForObject(UObject* Object);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stats)
	TArray<int> Handles;
};
