// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include "TreacherousSoul.generated.h"

UCLASS()
class BARKTOLIGHT_API ATreacherousSoul : public AEnemy
{
	GENERATED_BODY()

public:
	ATreacherousSoul();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Treacherous Soul")
	bool bIsAttacking = false;
};
