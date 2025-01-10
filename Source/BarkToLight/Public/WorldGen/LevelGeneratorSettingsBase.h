// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelGeneratorSettingsBase.generated.h"

class ALevelGeneratorBase;

/**
 * The base class for all level generator settings.
 */
UCLASS()
class BARKTOLIGHT_API ULevelGeneratorSettingsBase : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Generator Settings")
	TSubclassOf<ALevelGeneratorBase> ValidGeneratorClass;
};
