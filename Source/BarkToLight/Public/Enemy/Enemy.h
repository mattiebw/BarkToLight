// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Core/HealthComponent.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UEnemyStats;
class UEnemyData;

UCLASS()
class BARKTOLIGHT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void InitialiseFromData(UEnemyData* InData);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Enemy")
	FORCEINLINE UHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UEnemyData* Data;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	UEnemyStats* Stats;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	UHealthComponent* HealthComponent;
};
