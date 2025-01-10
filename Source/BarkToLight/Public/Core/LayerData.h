// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LayerData.generated.h"

class ALevelGeneratorBase;
/**
 * Contains data for a layer of hell that the player can traverse.
 */
UCLASS()
class BARKTOLIGHT_API ULayerData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layer Data")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layer Data")
	int Depth = 9;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Layer Data")
	TSubclassOf<ALevelGeneratorBase> LevelGeneratorClass;
};
