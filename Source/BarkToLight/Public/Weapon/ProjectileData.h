// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectileData.generated.h"

class AProjectile;
class UProjectileStats;

/**
 * Data describing the basic properties of a projectile, and it's default stats.
 */
UCLASS()
class BARKTOLIGHT_API UProjectileData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<AProjectile> ProjectileSubclass;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "Weapon")
	UProjectileStats* DefaultStats;
};
