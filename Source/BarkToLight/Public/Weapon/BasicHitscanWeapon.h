// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "BasicHitscanWeapon.generated.h"

class AEnemy;

UCLASS()
class UHitscanWeaponStats : public UWeaponStats
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetRange, Category = "Stats")
	float Range = 1000;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnRangeChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetRange(float NewValue)
	{
		Range = NewValue;
		OnRangeChanged.Broadcast(GetValue("Range"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetRange() const { return GetValue("Range"); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetFalloffBegin, Category = "Stats")
	float FalloffBegin = 0.2f;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnFalloffBeginChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetFalloffBegin(float NewValue)
	{
		FalloffBegin = NewValue;
		OnFalloffBeginChanged.Broadcast(GetValue("FalloffBegin"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetFalloffBegin() const { return GetValue("FalloffBegin"); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetFalloffEnd, Category = "Stats")
	float FalloffEnd = 0.8f;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnFalloffEndChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetFalloffEnd(float NewValue)
	{
		FalloffEnd = NewValue;
		OnFalloffEndChanged.Broadcast(GetValue("FalloffEnd"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetFalloffEnd() const { return GetValue("FalloffEnd"); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Setter = SetFalloffDamageMultiplier, Category = "Stats")
	float FalloffDamageMultiplier = 0.5f;

public:
	UPROPERTY(BlueprintAssignable, Category = "Stats")
	FOnStatChanged OnFalloffDamageMultiplierChanged;
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void SetFalloffDamageMultiplier(float NewValue)
	{
		FalloffDamageMultiplier = NewValue;
		OnFalloffDamageMultiplierChanged.Broadcast(GetValue("FalloffDamageMultiplier"));
	}
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Stats")
	float GetFalloffDamageMultiplier() const { return GetValue("FalloffDamageMultiplier"); };
};

UCLASS()
class BARKTOLIGHT_API ABasicHitscanWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	ABasicHitscanWeapon();

	virtual void OnFire_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Weapon")
	float GetDamageForHit(const FHitResult& Hit) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnGeometryHit(const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void OnEnemyHit(AEnemy* Enemy, const FHitResult& Hit);
};
