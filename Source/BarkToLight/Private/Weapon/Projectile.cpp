// copyright lolol

#include "Weapon/Projectile.h"

#include "Core/BTLPlayerCharacter.h"
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

	if (Data)
		SetData(Data);
}

void AProjectile::SetData(UProjectileData* NewData)
{
	Data  = NewData;
	Stats = DuplicateObject(Data->DefaultStats, this);

	MovementComponent->InitialSpeed           = Stats->GetInitialSpeed();
	MovementComponent->MaxSpeed               = MovementComponent->InitialSpeed;
	MovementComponent->ProjectileGravityScale = 0;
	MovementComponent->ProjectileGravityScale = 0;
	MovementComponent->Friction               = 0;
}

void AProjectile::Fire(FVector Direction, float InDamage, UObject* InSource)
{
	MovementComponent->Velocity = Direction * Stats->GetInitialSpeed();
	Damage                      = InDamage;
	Source                      = InSource;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector              NormalImpulse, const FHitResult& Hit)
{
	if (AEnemy* Enemy = Cast<AEnemy>(OtherActor))
	{
		OnHitEnemy(Enemy, Hit);
		return;
	}

	ABTLPlayerCharacter* Player = Cast<ABTLPlayerCharacter>(OtherActor);
	if (Player && Data->bCanHurtPlayer)
	{
		Player->GetHealthComponent()->ReceiveDamageInstance(
			FDamageInstance(Damage * Stats->GetDamageMultiplier(), Source, Data->Name.ToString()));
		Destroy();
		return;
	}
	
	OnHitGeo(Hit);
}

AProjectile* AProjectile::CreateProjectile(UObject* WorldContextObject, UProjectileData* Data, FVector    Location,
                                           FVector  Direction, float                     Damage, UObject* Source)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
		return nullptr;

	AProjectile* Proj = World->SpawnActorDeferred<AProjectile>(Data->ProjectileSubclass, FTransform::Identity);
	Proj->Data        = Data;
	Proj->FinishSpawning(FTransform(Location));
	Proj->Fire(Direction, Damage, Source);
	return Proj;
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
