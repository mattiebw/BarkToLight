// copyright lolol

#include "Core/StatsClass.h"

#include "BarkToLightLog.h"

int UStatsClass::AddUpgrade(const FStatUpgrade& Upgrade)
{
	FStatUpgrade NewUpgrade = Upgrade;
	NewUpgrade.Handle = FMath::Rand();
	StatUpgrades.Add(NewUpgrade);
	FProperty* Property = GetClass()->FindPropertyByName(Upgrade.StatName);
	if (Property == nullptr)
	{
		BTL_LOGC_ERROR(GetWorld(), "In StatsClass %s, property %s does not exist", *GetName(), *Upgrade.StatName.ToString());
		return -1;
	}

	Property->CallSetter(this, &Upgrade.Value);
	return NewUpgrade.Handle;
}

bool UStatsClass::RemoveUpgrade(int Handle)
{
	for (int i = 0; i < StatUpgrades.Num(); i++)
	{
		if (StatUpgrades[i].Handle == Handle)
		{
			StatUpgrades.RemoveAt(i);
			return true;
		}
	}

	return false;
}

float UStatsClass::GetValue(FName StatName) const
{
	FProperty* Property = GetClass()->FindPropertyByName(StatName);
	if (Property == nullptr)
	{
		BTL_LOGC_ERROR(GetWorld(), "In StatsClass %s, property %s does not exist", *GetName(), *StatName.ToString());
		return 0;
	}

	float Value = 0;
	Property->GetValue_InContainer(this, &Value); // MW @todo: This is awful! Unsafe!

	for (const FStatUpgrade& Upgrade : StatUpgrades)
	{
		if (Upgrade.StatName == StatName)
		{
			switch (Upgrade.UpgradeType)
			{
			case EUpgradeType::Add:
				Value += Upgrade.Value;
				break;
			case EUpgradeType::Multiply:
				Value *= Upgrade.Value;
				break;
			}
		}
	}

	return Value;
}
