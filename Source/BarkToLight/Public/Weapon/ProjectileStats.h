// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Core/StatsClass.h"
#include "UObject/Object.h"
#include "ProjectileStats.generated.h"

/**
 * Base class for projectile stats.
 * The DECLARE_BTL_STAT macro is used to declare stats, which are then expanded via Rider.
 */
UCLASS()
class BARKTOLIGHT_API UProjectileStats : public UStatsClass
{
	GENERATED_BODY()

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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetInitialSpeed, Category = "Stats")
	float InitialSpeed = 1000;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnInitialSpeedChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetInitialSpeed(float NewValue)
	{
		InitialSpeed = NewValue;
		OnInitialSpeedChanged.Broadcast(GetValue("InitialSpeed"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetInitialSpeed() const { return GetValue("InitialSpeed"); }
};
