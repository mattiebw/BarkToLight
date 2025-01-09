// copyright lolol

#include "Enemy/EnemyController.h"

AEnemyController::AEnemyController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
