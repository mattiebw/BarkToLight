﻿// copyright lolol

#include "Enemy/Enemy.h"

#include "BarkToLightLog.h"
#include "Enemy/EnemyData.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (Data != nullptr)
		InitialiseFromData(Data);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::InitialiseFromData(UEnemyData* InData)
{
	Data = InData;
	Stats = DuplicateObject(Data->DefaultStats, this);

	if (GetClass() != Data->PawnSubclass)
	{
		BTL_LOGC_ERROR_NOLOC(this, "Pawn subclass mismatch! Expected %s, got %s.", *Data->PawnSubclass->GetName(), *GetClass()->GetName());
		return;
	}
}
