// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DogController.generated.h"

UCLASS()
class BARKTOLIGHT_API ADogController : public AAIController
{
	GENERATED_BODY()

public:
	ADogController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Saved();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dog Controller")
	UBlackboardData* BlackboardData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dog Controller")
	UBehaviorTree* BehaviorTree; 
};
