// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "BTLPlayerCharacter.h"
#include "GameFramework/Character.h"
#include "Dog.generated.h"

class UInteractableComponent;

UCLASS()
class BARKTOLIGHT_API ADog : public ACharacter
{
	GENERATED_BODY()

public:
	ADog();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Saved(ABTLPlayerCharacter* Player);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dog")
	UInteractableComponent* InteractableComponent;
};
