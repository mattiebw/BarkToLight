// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "LevelDecorator.h"
#include "GenNavMeshBoundsDecorator.generated.h"

/**
 * This decorator generates a navmesh bounds volume around the level, so we don't have to have a massive oversized bounds
 * to account for however large the level could be.
 */
UCLASS()
class BARKTOLIGHT_API UGenNavMeshBoundsDecorator : public ULevelDecorator
{
	GENERATED_BODY()

public:
	virtual void Decorate() override;
};
