// copyright lolol

#include "Core/Dog.h"

#include "Core/DogController.h"
#include "Core/InteractableComponent.h"

ADog::ADog()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable"));
	InteractableComponent->Prompt = FText::FromString("Save Dog");

	AIControllerClass = ADogController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ADog::BeginPlay()
{
	Super::BeginPlay();

	InteractableComponent->OnInteractedWith.AddUniqueDynamic(this, &ADog::Saved);
}

void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADog::Saved(ABTLPlayerCharacter* Player)
{
	Player->bHasSavedDog = true;
	InteractableComponent->DestroyComponent();
	Cast<ADogController>(GetController())->Saved();
}
