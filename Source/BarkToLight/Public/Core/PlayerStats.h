// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "StatsClass.h"
#include "PlayerStats.generated.h"

UCLASS()
class BARKTOLIGHT_API UPlayerStats : public UStatsClass
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetHealth, Category = "Stats")
	float Health = 100;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnHealthChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetHealth(float NewValue)
	{
		Health = NewValue;
		OnHealthChanged.Broadcast(GetValue("Health"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetHealth() const { return GetValue("Health"); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetStamina, Category = "Stats")
	float Stamina = 100;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnStaminaChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetStamina(float NewValue)
	{
		Stamina = NewValue;
		OnStaminaChanged.Broadcast(GetValue("Stamina"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetStamina() const { return GetValue("Stamina"); }

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetCrouchSpeedMultiplier,
		Category = "Stats")
	float CrouchSpeedMultiplier = 0.6f;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnCrouchSpeedMultiplierChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetCrouchSpeedMultiplier(float NewValue)
	{
		CrouchSpeedMultiplier = NewValue;
		OnCrouchSpeedMultiplierChanged.Broadcast(GetValue("CrouchSpeedMultiplier"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetCrouchSpeedMultiplier() const { return GetValue("CrouchSpeedMultiplier"); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetJumpZ, Category = "Stats")
	float JumpZ = 500;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnJumpZChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetJumpZ(float NewValue)
	{
		JumpZ = NewValue;
		OnJumpZChanged.Broadcast(GetValue("JumpZ"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetJumpZ() const { return GetValue("JumpZ"); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetWeaponSlots, Category = "Stats")
	float WeaponSlots = 2;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnWeaponSlotsChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetWeaponSlots(float NewValue)
	{
		WeaponSlots = NewValue;
		OnWeaponSlotsChanged.Broadcast(GetValue("WeaponSlots"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetWeaponSlots() const { return GetValue("WeaponSlots"); }
};
