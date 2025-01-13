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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetHealth, Category = "Stats")
	float Health = 5;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnHealthChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetHealth(float NewValue)
	{
		Health = NewValue;
		OnHealthChanged.Broadcast(GetValue("Health"));
	}

	float GetHealth() const { return GetValue("Health"); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetSpeed, Category = "Stats")
	float Speed = 600;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnSpeedChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetSpeed(float NewValue)
	{
		Speed = NewValue;
		OnSpeedChanged.Broadcast(GetValue("Speed"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetSpeed() const { return GetValue("Speed"); };

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetDamageMultiplier,
		Category = "Stats")
	float DamageMultiplier = 1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnDamageMultiplierChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetDamageMultiplier(float NewValue)
	{
		DamageMultiplier = NewValue;
		OnDamageMultiplierChanged.Broadcast(GetValue("DamageMultiplier"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetDamageMultiplier() const { return GetValue("DamageMultiplier"); }
};
