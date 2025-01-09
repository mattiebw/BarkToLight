// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RoomFactory.generated.h"

class URoomDecorator;
class ARoom;

/**
 * Factory class used to create room objects and then apply decorators to them.
 */
UCLASS()
class BARKTOLIGHT_API URoomFactory : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Room Factory")
	URoomFactory* CreateRoom(TSubclassOf<ARoom> RoomClass);

	UFUNCTION(BlueprintNativeEvent, Category = "Room Factory")
	URoomFactory* AddDecorator(TSubclassOf<URoomDecorator> DecoratorClass);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Room Factory")
	ARoom* GetRoom() { return Room; }

	UFUNCTION(BlueprintCallable, Category = "Room Factory")
	URoomFactory* Reset();

protected:
	UPROPERTY()
	ARoom* Room;
};
