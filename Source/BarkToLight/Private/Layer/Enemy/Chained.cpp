// copyright lolol

#include "Layer/Enemy/Chained.h"

#include "Core/BTLPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon/Projectile.h"

AChained::AChained()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AChained::BeginPlay()
{
	Super::BeginPlay();

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

	// Check to see if we should fire at them.
	FiringCooldownSeconds -= DeltaTime;
	if (FiringCooldownSeconds <= 0)
	{
		// Fire at the player
		FHitResult Hit;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(PlayerCharacter);
		if (GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), PlayerCharacter->GetActorLocation(), ECC_WorldStatic, Params))
			return;
		
		AProjectile::CreateProjectile(this, Projectile, GetMesh()->GetComponentTransform().TransformPosition(ProjectileSpawnLocation),
		                              (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal(), -15,
		                              this);

		FiringCooldownSeconds = FMath::Lerp(0.75f, 1.5f, HealthComponent->GetHealthPercentage());
	}
}

void AChained::OnDeath_Implementation(const FDamageInstance& DamageInstance)
{
	bAlive = false;
	GetMesh()->DestroyComponent();
	// MW @todo: Spawn debris
}

void AChained::OnHealthChanged(float NewHealth, float HealthDifference)
{
	GetMesh()->SetMorphTarget("Corruption", 1 - HealthComponent->GetHealthPercentage());
}
