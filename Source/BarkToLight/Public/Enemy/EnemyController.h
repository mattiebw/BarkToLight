// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "EnemyController.generated.h"

UCLASS()
class BARKTOLIGHT_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
