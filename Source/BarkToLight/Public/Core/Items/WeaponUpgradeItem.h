// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UpgradeItem.h"
#include "WeaponUpgradeItem.generated.h"

class AWeapon;
struct FStatUpgrade;

/**
 * An item that upgrades a weapon when equipped to one.
 */
UCLASS()
class BARKTOLIGHT_API UWeaponUpgradeItem : public UUpgradeItem
{
	GENERATED_BODY()

public:
	virtual UStatsClass* GetStatsClassForObject(UObject* Object) override;
};
