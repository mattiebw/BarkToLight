// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Weapon.h"
#include "BasicProjectileWeapon.generated.h"

UCLASS()
class UProjectileWeaponStats : public UWeaponStats
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	TSubclassOf<AProjectile> ProjectileClass = AProjectile::StaticClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	UProjectileData* ProjectileData;
};

UCLASS()
class BARKTOLIGHT_API ABasicProjectileWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	ABasicProjectileWeapon();
	
	virtual void OnFire_Implementation() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (MakeEditWidget))
	FVector ProjectileSpawnLocation;
};
