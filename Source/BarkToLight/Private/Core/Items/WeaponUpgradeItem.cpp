// copyright lolol

#include "Core/Items/WeaponUpgradeItem.h"

#include "BarkToLightLog.h"
#include "Weapon/Weapon.h"

UStatsClass* UWeaponUpgradeItem::GetStatsClassForObject(UObject* Object)
{
	AWeapon* Weapon = Cast<AWeapon>(Object);
	if (!Weapon)
	{
		BTL_LOG_ERROR("Weapon upgrade item equipped by non-weapon object.");
		return nullptr;
	}

	return Weapon->GetStats();
}
