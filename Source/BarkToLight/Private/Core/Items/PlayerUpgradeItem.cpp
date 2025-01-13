// copyright lolol

#include "Core/Items/PlayerUpgradeItem.h"

#include "BarkToLightLog.h"
#include "Core/BTLPlayerCharacter.h"
#include "Core/PlayerStats.h"
#include "Core/StatsClass.h"

UStatsClass* UPlayerUpgradeItem::GetStatsClassForObject(UObject* Object)
{
	ABTLPlayerCharacter* Character = Cast<ABTLPlayerCharacter>(Object);
	if (!Character)
	{
		BTL_LOG_ERROR("Player upgrade item equipped by non-player object.");
		return nullptr;
	}

	return Character->GetStats();
}
