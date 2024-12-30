// copyright lolol

#include "Core/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UHealthComponent::TickComponent(float                        DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Overheal decay
	if (Health > MaxHealth)
	{
		Health -= OverhealDecayRate * DeltaTime;
		if (Health < MaxHealth)
			Health = MaxHealth;
	}
}

void UHealthComponent::ReceiveDamageInstance(const FDamageInstance& DamageInstance)
{
	float OldHealth = Health;
	Health += DamageInstance.Damage;
	if (Health > MaxHealth && !bAllowOverheal)
		Health = MaxHealth;
	else if (Health < 0)
		Health = 0;

	float Difference = Health - OldHealth;
	if (Difference == 0) return;
	OnHealthChanged.Broadcast(Health, Difference);

	if (Difference > 0)
		OnHealed.Broadcast(Difference, DamageInstance);
	else
		OnHurt.Broadcast(-Difference, DamageInstance); // We should provide the damage amount as a positive number (negative damage would be healing!). So lets negate the difference.

	if (Health < 0)
	{
		OnKilled.Broadcast(DamageInstance);
	}
}

void UHealthComponent::Kill(TScriptInterface<IDamageSource> Source, FString Description)
{
	ReceiveDamageInstance(FDamageInstance(Health, Source, Description));
}

void UHealthComponent::SetHealth(float NewHealth, FString Description)
{
	ReceiveDamageInstance(FDamageInstance(NewHealth - Health, TScriptInterface<IDamageSource>(), Description));
}
