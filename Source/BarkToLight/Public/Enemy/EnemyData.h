// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "EnemyController.h"
#include "Engine/DataAsset.h"
#include "EnemyStats.h"
#include "EnemyData.generated.h"

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
	USkeletalMesh* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<UAnimInstance> AnimationBP;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<AEnemyController> ControllerSubclass = AEnemyController::StaticClass();

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Enemy")
	UEnemyStats* DefaultStats;
};
