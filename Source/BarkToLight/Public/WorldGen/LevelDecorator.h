// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LevelDecorator.generated.h"

class ALevelGenerator;

/**
 * 
 */
UCLASS(Abstract)
class BARKTOLIGHT_API ULevelDecorator : public UObject
{
	GENERATED_BODY()

public:
	void Decorate();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Decorator")
	ALevelGenerator* Level;
};
