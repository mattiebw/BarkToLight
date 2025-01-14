// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "WorldGen/LevelDecorator.h"
#include "PlaceDogDecorator.generated.h"

class ADog;

/**
 * Place a dog in the level, in the room with the highest difficulty.
 */
UCLASS()
class BARKTOLIGHT_API UPlaceDogDecorator : public ULevelDecorator
{
	GENERATED_BODY()

public:
	virtual void Decorate() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Place Dog Decorator")
	TSubclassOf<ADog> DogSubclass;
};
