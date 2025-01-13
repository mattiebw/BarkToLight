// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "EnemyEncounterVolume.generated.h"

UCLASS()
class BARKTOLIGHT_API AEnemyEncounterVolume : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyEncounterVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
