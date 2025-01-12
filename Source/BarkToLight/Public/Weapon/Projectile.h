// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class AEnemy;
class UProjectileData;
class UProjectileStats;
class UProjectileMovementComponent;

// Base class for a projectile (fired by weapons). Implements the core logic, most projectiles can just use this
// class with changes to their ProjectileData.
UCLASS()
class BARKTOLIGHT_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	AProjectile();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual void SetData(UProjectileData* NewData);

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual void Fire(FVector Direction, float InDamage, UObject* InSource = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Projectile")
	FORCEINLINE UProjectileData* GetData() const { return Data; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Projectile")
	FORCEINLINE UProjectileStats* GetStats() const { return Stats; }
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Projectile")
	void OnHitEnemy(AEnemy* Enemy, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Projectile")
	void OnHitGeo(const FHitResult& Hit);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileData* Data;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Projectile")
	UProjectileStats* Stats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Damage = 0; 
	
	UPROPERTY()
	UObject* Source;
};
