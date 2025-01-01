// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "StatsClass.h"
#include "PlayerStats.generated.h"

UCLASS()
class BARKTOLIGHT_API UPlayerStats : public UStatsClass
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetHealth, Category = "Stats")
	float Health = 100;
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnHealthChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetHealth(float NewValue)
	{
		Health = NewValue;
		OnHealthChanged.Broadcast(NewValue);
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetStamina, Category = "Stats")
	float Stamina = 100;
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnStaminaChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetStamina(float NewValue)
	{
		Stamina = NewValue;
		OnStaminaChanged.Broadcast(NewValue);
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetSpeed, Category = "Stats")
	float Speed = 600;
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnSpeedChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetSpeed(float NewValue)
	{
		Speed = NewValue;
		OnSpeedChanged.Broadcast(NewValue);
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetCrouchSpeedMultiplier, Category = "Stats")
	float CrouchSpeedMultiplier = 0.6f;
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnCrouchSpeedMultiplierChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetCrouchSpeedMultiplier(float NewValue)
	{
		CrouchSpeedMultiplier = NewValue;
		OnCrouchSpeedMultiplierChanged.Broadcast(NewValue);
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetJumpZ, Category = "Stats")
	float JumpZ = 500;
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnJumpZChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetJumpZ(float NewValue)
	{
		JumpZ = NewValue;
		OnJumpZChanged.Broadcast(NewValue);
	};
};
