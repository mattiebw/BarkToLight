// copyright lolol

#include "Weapon/Projectile.h"

#include "Enemy/Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapon/ProjectileData.h"
#include "Weapon/ProjectileStats.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent     = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	RootComponent     = MeshComponent;

	MeshComponent->OnComponentHit.AddUniqueDynamic(this, &AProjectile::OnHit);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::SetData(UProjectileData* NewData)
{
	Data  = NewData;
	Stats = DuplicateObject(Data->DefaultStats, this);

	MovementComponent->InitialSpeed = Stats->GetInitialSpeed();
}

void AProjectile::Fire(FVector Direction, float InDamage, UObject* InSource)
{
	MovementComponent->Velocity = Direction;
	// Velocity is treated as a direction, as we have set MovementComponent->InitialSpeed.
	Damage = InDamage;
	Source = InSource;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector              NormalImpulse, const FHitResult& Hit)
{
	if (AEnemy* Enemy = Cast<AEnemy>(OtherActor))
	{
		OnHitEnemy(Enemy, Hit);
	}
	else
	{
		OnHitGeo(Hit);
	}
}

void AProjectile::OnHitGeo_Implementation(const FHitResult& Hit)
{
	Destroy();
}

void AProjectile::OnHitEnemy_Implementation(AEnemy* Enemy, const FHitResult& Hit)
{
	Enemy->GetHealthComponent()->ReceiveDamageInstance(
		FDamageInstance(Damage * Stats->GetDamageMultiplier(), Source, Data->Name.ToString()));
	Destroy();
}
