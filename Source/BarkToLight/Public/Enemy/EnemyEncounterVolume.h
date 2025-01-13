// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "EnemyEncounterVolume.generated.h"

class UEnemyData;

UCLASS()
class BARKTOLIGHT_API AEnemyEncounterVolume : public ATriggerBox
{
	GENERATED_BODY()

public:
	AEnemyEncounterVolume();

	UFUNCTION()
	void OnOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category = "Enemy Encounter")
	void SetCenterAndExtent(FVector NewCenter, FVector NewExtent);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy Encounter")
	FBox GetBox();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Encounter")
	TMap<UEnemyData*, int> Spawns;
};
