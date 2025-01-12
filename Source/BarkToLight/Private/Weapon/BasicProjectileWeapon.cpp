// copyright lolol

#include "Weapon/BasicProjectileWeapon.h"

#include "Core/BTLPlayerCharacter.h"
#include "Weapon/Projectile.h"

ABasicProjectileWeapon::ABasicProjectileWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	RequiredStatsClass = UProjectileWeaponStats::StaticClass();
}

void ABasicProjectileWeapon::OnFire_Implementation()
{
	UProjectileWeaponStats* ProjectileStats = Cast<UProjectileWeaponStats>(this->Stats);
	check(ProjectileStats);
	
	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(ProjectileStats->ProjectileClass, FTransform::Identity, this);
	Projectile->SetData(ProjectileStats->ProjectileData);
	Projectile->Fire(OwningPlayer->GetBaseAimRotation().Vector(), ProjectileStats->GetDamage(), this);
	Projectile->FinishSpawning(FTransform(GetActorLocation() + ProjectileSpawnLocation));
}
