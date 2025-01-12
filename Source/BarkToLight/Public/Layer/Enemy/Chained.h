// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "Chained.generated.h"

class UProjectileData;
class ABTLPlayerCharacter;

UCLASS()
class BARKTOLIGHT_API AChained : public AEnemy
{
	GENERATED_BODY()

public:
	AChained();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void OnDeath_Implementation(const FDamageInstance& DamageInstance) override;

	UFUNCTION()
	void OnHealthChanged(float NewHealth, float HealthDifference);

protected:
	bool bAlive = true;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Chained")
	ABTLPlayerCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chained")
	UProjectileData* Projectile = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Chained", meta = (MakeEditWidget))
	FVector ProjectileSpawnLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Chained")
	float FiringCooldownSeconds = 0;
};
