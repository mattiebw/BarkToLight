// copyright lolol

#include "Enemy/EnemyEncounterVolume.h"

#include "BarkToLight.h"
#include "Components/BoxComponent.h"
#include "Core/BTLPlayerCharacter.h"
#include "Enemy/Enemy.h"

AEnemyEncounterVolume::AEnemyEncounterVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	OnActorBeginOverlap.AddUniqueDynamic(this, &AEnemyEncounterVolume::OnOverlap);
	GetCollisionComponent()->SetCollisionResponseToChannel(ECC_Weapon, ECR_Ignore);
}

void AEnemyEncounterVolume::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!OtherActor->IsA<ABTLPlayerCharacter>())
		return;

	UBoxComponent* BoxComponent = Cast<UBoxComponent>(GetCollisionComponent());
	FVector CenterLocation = BoxComponent->GetComponentLocation();
	FVector Extents = BoxComponent->GetScaledBoxExtent();

	for (TTuple<UEnemyData*, int>& Spawn : Spawns)
	{
		for (int i = 0; i < Spawn.Value; i++)
		{
			FVector SpawnLocation = CenterLocation;
			FHitResult Hit;
			int Tries = 100;
			do
			{
				SpawnLocation = CenterLocation;
				SpawnLocation.X += FMath::FRandRange(-Extents.X, Extents.X);
				SpawnLocation.Y += FMath::FRandRange(-Extents.Y, Extents.Y);
			
				GetWorld()->LineTraceSingleByChannel(Hit, SpawnLocation, SpawnLocation - FVector(0, 0, Extents.Z), ECC_WorldStatic);
				SpawnLocation = Hit.Location;

				Tries--;
			}
			while (!Hit.bBlockingHit && Tries > 0);

			AEnemy::CreateEnemy(this, SpawnLocation, Spawn.Key);
		}
	}

	Destroy();
}

void AEnemyEncounterVolume::SetCenterAndExtent(FVector NewCenter, FVector NewExtent)
{
	UBoxComponent* BoxComponent = Cast<UBoxComponent>(GetCollisionComponent());
	BoxComponent->SetBoxExtent(NewExtent);
	BoxComponent->SetWorldLocation(NewCenter);
}

FBox AEnemyEncounterVolume::GetBox()
{
	UBoxComponent* BoxComponent = Cast<UBoxComponent>(GetCollisionComponent());
	FVector Center = BoxComponent->GetComponentLocation();
	return FBox(Center - BoxComponent->GetScaledBoxExtent() / 2, Center + BoxComponent->GetScaledBoxExtent() / 2);
}
