// copyright lolol

#include "Core/Items/UpgradeItem.h"

#include "BarkToLightLog.h"
#include "Core/StatsClass.h"

void UUpgradeItem::Equip_Implementation(UObject* EquipTo)
{
	UStatsClass* Stats = GetStatsClassForObject(EquipTo);
	if (!Stats)
		return;

	if (!Handles.IsEmpty())
	{
		BTL_LOG_ERROR("Upgrade item being equipped when already equipped.");
		return;	
	}
	
	for (FStatUpgrade& Upgrade : Upgrades)
	{
		Handles.Add(Stats->AddUpgrade(Upgrade));
	}
}

void UUpgradeItem::Unequip_Implementation(UObject* EquipTo)
{
	UStatsClass* Stats = GetStatsClassForObject(EquipTo);
	if (!Stats)
		return;
	
	for (int Handle : Handles)
		Stats->RemoveUpgrade(Handle);

	Handles.Empty();
}

UStatsClass* UUpgradeItem::GetStatsClassForObject(UObject* Object)
{
	BTL_LOG_ERROR("Base UUpgradeItem::GetStatsClassForObject called!");
	return nullptr;
}
