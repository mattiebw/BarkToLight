// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGeneratorBase.generated.h"

class ULevelGeneratorSettingsBase;

UCLASS()
class BARKTOLIGHT_API ALevelGeneratorBase : public AActor
{
	GENERATED_BODY()

public:
	ALevelGeneratorBase();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Level Generator")
	void Generate();

	UFUNCTION(CallInEditor, Category = "Level Generator")
	void GenerateInEditor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Generator")
	ULevelGeneratorSettingsBase* Settings;
};
