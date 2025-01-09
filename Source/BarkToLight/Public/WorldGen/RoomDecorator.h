// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RoomDecorator.generated.h"

class ARoom;

/**
 * 
 */
UCLASS(Abstract)
class BARKTOLIGHT_API URoomDecorator : public UObject
{
	GENERATED_BODY()

public:
	void Decorate();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Decorator")
	ARoom* Room;
};
