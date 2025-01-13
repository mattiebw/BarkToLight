// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UpgradeItem.h"
#include "PlayerUpgradeItem.generated.h"

class ABTLPlayerCharacter;
struct FStatUpgrade;

/**
 * An item that can be equipped by a player to give upgrades.
 */
UCLASS()
class BARKTOLIGHT_API UPlayerUpgradeItem : public UUpgradeItem
{
	GENERATED_BODY()

public:
	virtual UStatsClass* GetStatsClassForObject(UObject* Object) override;
};
