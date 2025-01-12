// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Core/DamageSource.h"
#include "Core/HealthComponent.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UEnemyStats;
class UEnemyData;

UCLASS()
class BARKTOLIGHT_API AEnemy : public ACharacter, public IDamageSource
{
	GENERATED_BODY()

public:
	AEnemy();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy")
	void OnDeath(const FDamageInstance& DamageInstance);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void InitialiseFromData(UEnemyData* InData);

	UFUNCTION()
	void OnMaxHealthChanged(float NewValue);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy")
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	
	// --- IDamageSource ---
	virtual AActor*           GetDamageSource_Implementation() override;
	virtual FText             GetDamageSourceName_Implementation() const override;
	virtual EDamageSourceType GetDamageSourceType_Implementation() const override;
	// ---------------------
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UEnemyData* Data;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	UEnemyStats* Stats;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UHealthComponent* HealthComponent;

	bool bInitialised = false;
};
