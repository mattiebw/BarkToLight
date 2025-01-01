// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

USTRUCT(BlueprintType)
struct FDamageInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	float Damage;
	// UPROPERTY(BlueprintReadOnly, Category = "Damage")
	// TScriptInterface<IDamageSource> Source;
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	UObject* Source;
	UPROPERTY(BlueprintReadWrite, Category = "Damage")
	FString Description;

	FDamageInstance()
	{
		Damage      = 0;
		Source      = nullptr;
		Description = "";
	}

	FDamageInstance(int Damage, UObject* Source, const FString& Description = "")
	{
		this->Damage      = Damage;
		this->Source      = Source;
		this->Description = Description;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, HealthDifference);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHurt, float, Damage, const FDamageInstance&, DamageInstance);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealed, float, Amount, const FDamageInstance&, DamageInstance);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKilled, const FDamageInstance&, DamageInstance);

UCLASS(ClassGroup=(BTL), meta=(BlueprintSpawnableComponent))
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
	void Kill(UObject* Source, FString Description = "");
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetHealth(float NewHealth, FString Description = "");
	UFUNCTION(BlueprintCallable, Category = "Health")
	FORCEINLINE void SetHealthPercentage(float Percentage, FString Description = "")
	{
		SetHealth(MaxHealth * Percentage, Description);
	}

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetMaxHealth(float NewMaxHealth);

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

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	TArray<FDamageInstance> DamageLog;

	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnHurt OnHurt;
	UPROPERTY(BlueprintAssignable)
	FOnHealed OnHealed;
	UPROPERTY(BlueprintAssignable)
	FOnKilled OnKilled;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float Health = 100;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	float OverhealDecayRate = 1.f;
	UPROPERTY(BlueprintReadOnly, Category = "Health")
	bool bAllowOverheal = true;
};
