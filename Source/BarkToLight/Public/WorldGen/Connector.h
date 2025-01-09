// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Connector.generated.h"

class USplineComponent;

UCLASS(Abstract)
class BARKTOLIGHT_API AConnector : public AActor
{
	GENERATED_BODY()

public:
	AConnector();

	UFUNCTION(BlueprintNativeEvent, Category = "Connector")
	void Generate();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Connector")
	USplineComponent* Path;
};
