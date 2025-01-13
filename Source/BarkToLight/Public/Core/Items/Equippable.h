// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Equippable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UEquippable : public UInterface
{
	GENERATED_BODY()
};

/**
 * Functions that an object that can be equipped in some way must implement.
 */
class BARKTOLIGHT_API IEquippable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Source")
	void Equip(UObject* EquipTo);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Damage Source")
	void Unequip(UObject* EquipTo);
};
