// copyright lolol

#include "Core/StatsClass.h"

#include "BarkToLightLog.h"

// Add an upgrade to our list of upgrades.
// Then, return the handle so we can remove it later.
int UStatsClass::AddUpgrade(const FStatUpgrade& Upgrade)
{
	int Handle = FMath::Rand();
	StatUpgrades.Add(Handle, Upgrade);
	FProperty* Property = GetClass()->FindPropertyByName(Upgrade.StatName);
	if (Property == nullptr)
	{
		BTL_LOGC_ERROR(GetWorld(), "In StatsClass %s, property %s does not exist", *GetName(), *Upgrade.StatName.ToString());
		return -1;
	}

	return Handle;
}

// Look through our upgrades and remove the one with the given handle.
// Linear search, slow, but we shouldn't be running this often.
// If need be, a map could be used to speed this up.
bool UStatsClass::RemoveUpgrade(int Handle)
{
	if (StatUpgrades.Contains(Handle))
	{
		StatUpgrades.Remove(Handle);
		return true;
	}

	return false;
}

float UStatsClass::GetValue(FName StatName) const
{
	// Find our property by name.
	FProperty* Property = GetClass()->FindPropertyByName(StatName);
	if (Property == nullptr) // If it doesn't exist, log an error and return 0.
	{
		BTL_LOGC_ERROR(GetWorld(), "In StatsClass %s, property %s does not exist", *GetName(), *StatName.ToString());
		return 0;
	}

	// Get our value.
	float Value = 0;
	Property->GetValue_InContainer(this, &Value); // MW @todo: This is awful! Unsafe! Could be not a float, causing buffer overflow!

	// Look through our upgrades and apply any that apply.
	for (auto& UpgradeTuple : StatUpgrades)
	{
		const FStatUpgrade& Upgrade = UpgradeTuple.Value;
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

FString UStatsClass::GetUpgradeString() const
{
	// Use a string builder to avoid the performance hit of string concatenation.
	FStringBuilderBase Builder;

	for (const auto& UpgradeTuple : StatUpgrades)
	{
		const FStatUpgrade& Upgrade = UpgradeTuple.Value;
		
		Builder.Append(Upgrade.StatName.ToString());
		Builder.Append(": ");
		// If it's a multiply upgrade, show it as a percentage.
		if (Upgrade.UpgradeType == EUpgradeType::Multiply)
		{
			if (Upgrade.Value - 1 > 0)
				Builder.Append("+");
			Builder.Append(FString::FromInt((Upgrade.Value - 1) * 100));
			Builder.Append("%");
		} else
		{
			if (Upgrade.Value > 0)
				Builder.Append("+");
			Builder.Append(FString::SanitizeFloat(Upgrade.Value, 0));
		}
		Builder.Append("\n");
	}
	
	return Builder.ToString();
}
