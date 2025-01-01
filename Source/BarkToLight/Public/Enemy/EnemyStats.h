// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Core/StatsClass.h"
#include "EnemyStats.generated.h"

/**
 * Base class for enemy stats (health, damage, etc).
 * Stat definitions are declared using the DECLARE_BTL_STAT macro, and then expanded via Rider.
 */
UCLASS()
class BARKTOLIGHT_API UEnemyStats : public UStatsClass
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetHealth, Category = "Stats")
	float Health = 5;
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnHealthChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetHealth(float NewValue)
	{
		Health = NewValue;
		OnHealthChanged.Broadcast(NewValue);
	};
};
