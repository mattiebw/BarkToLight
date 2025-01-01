// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Core/StatsClass.h"
#include "UObject/Object.h"
#include "WeaponStats.generated.h"

/**
 * Base class for weapon stats.
 * The DECLARE_BTL_STAT macro is used to declare stats, which are then expanded via Rider.
 */
UCLASS()
class BARKTOLIGHT_API UWeaponStats : public UStatsClass
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetDamage, Category = "Stats")
	float Damage = 1;
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnDamageChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetDamage(float NewValue)
	{
		Damage = NewValue;
		OnDamageChanged.Broadcast(NewValue);
	};
};
