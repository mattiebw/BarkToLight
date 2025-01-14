// copyright lolol

#include "Core/InteractableComponent.h"

UInteractableComponent::UInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableComponent::Interact(ABTLPlayerCharacter* Player)
{
	OnInteractedWith.Broadcast(Player);
}
