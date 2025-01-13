// copyright lolol

#include "Layer/Enemy/Chained.h"

#include "Components/CapsuleComponent.h"
#include "Core/BTLPlayerCharacter.h"
#include "Enemy/EnemyStats.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"

AChained::AChained()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AChained::BeginPlay()
{
	Super::BeginPlay();

	AddActorWorldOffset(FVector(0, 0, 500));

	PlayerCharacter = Cast<ABTLPlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	HealthComponent->OnHealthChanged.AddUniqueDynamic(this, &AChained::OnHealthChanged);
}

void AChained::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bAlive)
		return;

	// Look at the player
	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerCharacter->GetActorLocation());
	GetMesh()->SetWorldRotation(FRotator(0, LookAt.Yaw - 90, -LookAt.Pitch));

	// Morph the mesh
	CurrentMorph = FMath::FInterpTo(CurrentMorph, TargetMorph, DeltaTime, 2);
	GetMesh()->SetMorphTarget("Morph", CurrentMorph);
	
	// Check to see if we should fire at them.
	FiringCooldownSeconds -= DeltaTime;
	if (FiringCooldownSeconds <= 0)
	{
		// Fire at the player
		FHitResult            Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(PlayerCharacter);
		if (GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), PlayerCharacter->GetActorLocation(),
		                                         ECC_WorldStatic, Params))
			return;

		AProjectile::CreateProjectile(this, Projectile,
		                              GetMesh()->GetComponentTransform().TransformPosition(ProjectileSpawnLocation),
		                              (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal(),
		                              15 * Stats->GetDamageMultiplier(),
		                              this);

		FiringCooldownSeconds = FMath::Lerp(0.75f, 1.5f, HealthComponent->GetHealthPercentage());
	}
}

void AChained::OnDeath_Implementation(const FDamageInstance& DamageInstance)
{
	bAlive = false;
	GetMesh()->DestroyComponent();
	GetCapsuleComponent()->DestroyComponent(true);
	// MW @todo: Spawn debris
}

void AChained::OnHealthChanged(float NewHealth, float HealthDifference)
{
	TargetMorph = 1 - HealthComponent->GetHealthPercentage();
}
