// copyright lolol

#include "Core/HealthComponent.h"

#include "BarkToLightLog.h"
#include "Core/DamageSource.h"

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
	Health -= DamageInstance.Damage;
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
		OnHurt.Broadcast(-Difference, DamageInstance);
	// We should provide the damage amount as a positive number (negative damage would be healing!). So lets negate the difference.

	if (Health <= 0)
		OnKilled.Broadcast(DamageInstance);

	FDamageInstance NewDamageInstance = DamageInstance;
	if (NewDamageInstance.Description.IsEmpty())
	{
		if (NewDamageInstance.Source == nullptr)
			return;
		if (!NewDamageInstance.Source->Implements<UDamageSource>())
		{
			BTL_LOGC_ERROR(GetWorld(), "Source of damage does not implement IDamageSource! Cannot get name.");
			return;
		}
		NewDamageInstance.Description = FString::Format(TEXT("Damaged by {0}"), {
			                                             IDamageSource::Execute_GetDamageSourceName(NewDamageInstance.Source).ToString()
		                                             });
	}
	NewDamageInstance.Source = nullptr; // Clear the source to prevent memory leaks
	DamageLog.Add(NewDamageInstance);
}

void UHealthComponent::Kill(UObject* Source, FString Description)
{
	ReceiveDamageInstance(FDamageInstance(Health, Source, Description));
}

void UHealthComponent::SetHealth(float NewHealth, FString Description)
{
	ReceiveDamageInstance(FDamageInstance(Health - NewHealth, nullptr, Description));
}

void UHealthComponent::SetMaxHealth(float NewMaxHealth)
{
	MaxHealth = NewMaxHealth;
	if (!bAllowOverheal && Health > MaxHealth)
		Health = MaxHealth;
}
