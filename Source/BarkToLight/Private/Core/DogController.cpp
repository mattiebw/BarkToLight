// copyright lolol

#include "Core/DogController.h"

ADogController::ADogController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADogController::BeginPlay()
{
	Super::BeginPlay();
}

void ADogController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADogController::Saved()
{
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	UseBlackboard(BlackboardData, BlackboardComp);
	RunBehaviorTree(BehaviorTree);
}
