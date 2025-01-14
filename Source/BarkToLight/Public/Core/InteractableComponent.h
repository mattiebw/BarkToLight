// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

class ABTLPlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractedWith, ABTLPlayerCharacter*, Player);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BARKTOLIGHT_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractableComponent();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
	void Interact(ABTLPlayerCharacter* Player);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactable")
	FText Prompt = FText::FromString("Interact");

	UPROPERTY(BlueprintAssignable, Category = "Interactable")
	FOnInteractedWith OnInteractedWith;
};
