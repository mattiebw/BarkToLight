﻿// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Connector.generated.h"

class UBoundsChecker;
class ARoom;
class USplineComponent;

UCLASS(Abstract)
class BARKTOLIGHT_API AConnector : public AActor
{
	GENERATED_BODY()

public:
	AConnector();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Connector")
	void Generate();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Connector")
	void CreateSplineFromRooms(ARoom* From, int FromIndex, ARoom* To, int ToIndex, UBoundsChecker* BoundsChecker);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Connector")
	USplineComponent* Path;
};
