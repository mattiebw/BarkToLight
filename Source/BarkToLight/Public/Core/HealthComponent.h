// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

USTRUCT(BlueprintType)
struct FDamageInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	float Damage = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	TScriptInterface<IDamageSource> Source = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FString Description = "";
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, HealthDifference);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHurt, float, Damage, const FDamageInstance&, DamageInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealed, float, Amount, const FDamageInstance&, DamageInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilled, const FDamageInstance&, DamageInstance);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BARKTOLIGHT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float                        DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ReceiveDamageInstance(const FDamageInstance& DamageInstance);
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Kill(TScriptInterface<IDamageSource> Source, FString Description = "");
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(float NewHealth, FString Description = "");

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE float GetHealth() const { return Health; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE float GetHealthPercentage() const { return Health / MaxHealth; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE bool IsAlive() const { return Health > 0; }
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Health")
	FORCEINLINE bool IsDead() const { return Health <= 0; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float Health = 100;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float OverhealDecayRate = 1.f;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bAllowOverheal = true;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnHurt OnHurt;
	UPROPERTY(BlueprintAssignable)
	FOnHealed OnHealed;
	UPROPERTY(BlueprintAssignable)
	FOnKilled OnKilled;
};
