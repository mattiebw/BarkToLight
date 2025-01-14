// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LevelDecorator.generated.h"

class ARoomsLevelGenerator;

/**
 * The base class for level decorators, which run after the main room generation process, and are intended to add additional
 * decorations to the level as a whole, such as props, etc, and other important level-wide features such as a navmesh
 * bounds volume and player start. This class is abstract, and should be subclassed to implement the actual decoration.
 */
UCLASS(Abstract, Blueprintable)
class BARKTOLIGHT_API ULevelDecorator : public UObject
{
	GENERATED_BODY()

public:
	// Decorate the level. When this is called, Level should have been set.
	UFUNCTION(BlueprintCallable, Category = "Level Decorator")
	virtual void Decorate();

	//  Blueprint implementable event for the decorator to implement.
	UFUNCTION(BlueprintImplementableEvent, Category = "Level Decorator")
	void OnDecorate();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Decorator")
	ARoomsLevelGenerator* Level;
};
