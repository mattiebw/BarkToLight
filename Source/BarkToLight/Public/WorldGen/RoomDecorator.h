// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RoomDecorator.generated.h"

class ARoom;

/**
 * URoomDecorators are responsible for decorating a room with additional actors or components.
 * They can be derived from to create custom decorators - for example, a "MonsterDecorator" that
 * creates the enemy encounters for a room, or something that simply adds some visual flair.
 */
UCLASS(Abstract)
class BARKTOLIGHT_API URoomDecorator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Room Decorator")
	void Decorate();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Decorator")
	ARoom* Room;
};
