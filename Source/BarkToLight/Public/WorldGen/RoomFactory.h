// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RoomFactory.generated.h"

struct FRoomDecoratorInfo;
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

	UFUNCTION(BlueprintCallable, Category = "Room Factory")
	URoomFactory* AddDecoratorsFromSubclasses(TArray<TSubclassOf<URoomDecorator>> Decorators, float ChancePer = 1.f);

	UFUNCTION(BlueprintCallable, Category = "Room Factory")
	URoomFactory* AddDecoratorsFromInfos(TArray<FRoomDecoratorInfo> Decorators);

	UFUNCTION(BlueprintCallable, Category = "Room Factory")
	URoomFactory* SetDifficulty(float Value);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Room Factory")
	FORCEINLINE ARoom* GetRoom() const { return Room; }
	
	UFUNCTION(BlueprintCallable, Category = "Room Factory")
	FORCEINLINE ARoom* Finish() { ARoom* Temp = Room; Reset(); return Temp; }

	UFUNCTION(BlueprintCallable, Category = "Room Factory")
	URoomFactory* Reset();

protected:
	UPROPERTY()
	ARoom* Room;
};
