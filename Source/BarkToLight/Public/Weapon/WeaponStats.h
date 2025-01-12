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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetDamage, Category = "Stats")
	float Damage = 1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnDamageChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetDamage(float NewValue)
	{
		Damage = NewValue;
		OnDamageChanged.Broadcast(GetValue("Damage"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetDamage() const { return GetValue("Damage"); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetClipSize, Category = "Stats")
	float ClipSize = 20;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnClipSizeChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetClipSize(float NewValue)
	{
		ClipSize = NewValue;
		OnClipSizeChanged.Broadcast(GetValue("ClipSize"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetClipSize() const { return GetValue("ClipSize"); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetAmmoUsageMultiplier,
		Category = "Stats")
	float AmmoUsageMultiplier = 1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnAmmoUsageMultiplierChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetAmmoUsageMultiplier(float NewValue)
	{
		AmmoUsageMultiplier = NewValue;
		OnAmmoUsageMultiplierChanged.Broadcast(GetValue("AmmoUsageMultiplier"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetAmmoUsageMultiplier() const { return GetValue("AmmoUsageMultiplier"); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetFireRateSeconds,
		Category = "Stats")
	float FireRateSeconds = 0.1f;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnFireRateSecondsChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetFireRateSeconds(float NewValue)
	{
		FireRateSeconds = NewValue;
		OnFireRateSecondsChanged.Broadcast(GetValue("FireRateSeconds"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetFireRateSeconds() const { return GetValue("FireRateSeconds"); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetReloadTimeSeconds,
		Category = "Stats")
	float ReloadTimeSeconds = 1;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnReloadTimeSecondsChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetReloadTimeSeconds(float NewValue)
	{
		ReloadTimeSeconds = NewValue;
		OnReloadTimeSecondsChanged.Broadcast(GetValue("ReloadTimeSeconds"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetReloadTimeSeconds() const { return GetValue("ReloadTimeSeconds"); };
};
