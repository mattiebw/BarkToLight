// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "EnemyStats.h"
#include "Engine/DataAsset.h"
#include "EnemyData.generated.h"

class AEnemyController;
class AEnemy;

/**
 * This class contains general data for an enemy, such as its name, AI class, and default stats.
 */
UCLASS()
class BARKTOLIGHT_API UEnemyData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<AEnemy> PawnSubclass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<AEnemyController> ControllerSubclass;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Enemy")
	UEnemyStats* DefaultStats;
};
