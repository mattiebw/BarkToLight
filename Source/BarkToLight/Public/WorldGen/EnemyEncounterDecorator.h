// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "RoomDecorator.h"
#include "EnemyEncounterDecorator.generated.h"

class UEnemyData;

USTRUCT(BlueprintType)
struct FEnemyType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UEnemyData* EnemyData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float Weight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float DifficultyCost;

	float MinRoll = 0;
	float MaxRoll = 0;
};

/**
 * This decorator generates the enemy spawner for each room.
 */
UCLASS()
class BARKTOLIGHT_API UEnemyEncounterDecorator : public URoomDecorator
{
	GENERATED_BODY()

public:
	virtual void Decorate_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TArray<FEnemyType> EnemyTypes;
};
