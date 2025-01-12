// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "LevelDecorator.h"
#include "PlayerStartDecorator.generated.h"

/**
 * This decorator places the player start in the level.
 * It finds the root room and places the player start in the center of that room.
 */
UCLASS()
class BARKTOLIGHT_API UPlayerStartDecorator : public ULevelDecorator
{
	GENERATED_BODY()

public:
	virtual void Decorate() override;
};
