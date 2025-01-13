// copyright lolol

#include "Weapon/BasicHitscanWeapon.h"

#include "BarkToLight.h"
#include "Core/BTLPlayerCharacter.h"
#include "Core/BTLPlayerController.h"
#include "Enemy/Enemy.h"

ABasicHitscanWeapon::ABasicHitscanWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	RequiredStatsClass = UHitscanWeaponStats::StaticClass();
}

void ABasicHitscanWeapon::OnFire_Implementation()
{
	// Get the direction the player is looking.
	UHitscanWeaponStats* HitscanStats = Cast<UHitscanWeaponStats>(this->Stats);
	check(HitscanStats);

	// Create our line trace - from the camera, towards aim direction for the range of the weapon.
	FVector Start = OwningPlayer->GetPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector End   = Start + OwningPlayer->GetBaseAimRotation().Vector() * HitscanStats->GetRange();

	// Perform the line trace.
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwningPlayer);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Weapon, Params);

	// We don't care if it didn't hit something.
	if (Hit.bBlockingHit)
	{
		if (AEnemy* Enemy = Cast<AEnemy>(Hit.GetActor()))
		{
			// We hit an enemy - deal damage and call to blueprint for effects.
				Enemy->GetHealthComponent()->ReceiveDamageInstance(FDamageInstance(
				HitscanStats->GetDamage(), this,
				FString::Printf(TEXT("%s's %s"), *OwningPlayer->GetName(), *Data->Name.ToString())));
			OnEnemyHit(Enemy, Hit);
		}
		else
		{
			// We hit geometry - call to blueprint for effects (e.g. decal, particles, sound)ww.
			OnGeometryHit(Hit);
		}
		
		DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.0f);
	} else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 1.0f);
	}
}

float ABasicHitscanWeapon::GetDamageForHit_Implementation(const FHitResult& Hit) const
{
	// Calculate the damage based on the distance the hit travelled.
	// This is a simple linear falloff.

	// First, find the distance:
	float Distance = FVector::Dist(Hit.TraceStart, Hit.ImpactPoint);
	// Then, represent that as a percentage of the weapons range:
	UHitscanWeaponStats* HitscanStats = Cast<UHitscanWeaponStats>(this->Stats);
	check(HitscanStats);
	float DistanceAlongRange = Distance / HitscanStats->GetRange();

	// Cache our stats, as the getter is heavy.
	float FalloffBegin            = HitscanStats->GetFalloffBegin();
	float FalloffEnd              = HitscanStats->GetFalloffEnd();
	float FalloffDamageMultiplier = HitscanStats->GetFalloffDamageMultiplier();
	float Damage                  = HitscanStats->GetDamage();

	if (DistanceAlongRange < FalloffBegin)
	{
		// We're within full damage range, so we deal full damage.
		return Damage;
	}
	if (DistanceAlongRange > FalloffEnd)
	{
		// We're outside the falloff range, so we deal the minimum damage.
		return Damage * FalloffDamageMultiplier;
	}

	// Otherwise, find out how far we are within the falloff range and lerp between the two damage values.
	float FalloffRange    = FalloffEnd - FalloffBegin;
	float FalloffProgress = (DistanceAlongRange - FalloffBegin) / FalloffRange;
	return FMath::Lerp(Damage, Damage * FalloffDamageMultiplier,
	                   FalloffProgress);
}
