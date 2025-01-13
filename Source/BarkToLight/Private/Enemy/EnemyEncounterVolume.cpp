// copyright lolol


#include "Enemy/EnemyEncounterVolume.h"


// Sets default values
AEnemyEncounterVolume::AEnemyEncounterVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyEncounterVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyEncounterVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

