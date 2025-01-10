// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelGeneratorSettings.generated.h"

class URoomDecorator;
class ARoom;
class URoomFactory;
class ULevelDecorator;
class AConnector;

USTRUCT(BlueprintType)
struct FRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room Info")
	TSubclassOf<ARoom> RoomClass;

	// The minimum amount of instances of this room that needs to exist.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room Info")
	int MinimumCount = 0;

	// The maximum amount of instances of this room that can exist.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room Info")
	int MaximumCount = 100;
};

USTRUCT(BlueprintType)
struct FRoomDecoratorInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room Decorator Info")
	TSubclassOf<URoomDecorator> DecoratorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room Decorator Info")
	float Chance = 1.f;
};

USTRUCT(BlueprintType)
struct FLevelDecoratorInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Decorator Info")
	TSubclassOf<ULevelDecorator> DecoratorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Decorator Info")
	float Chance = 1.f;
};

/**
 * This data container asset type contains information used by the level generator (ALevelGenerator)
 * to generate a level.
 */ 
UCLASS()
class BARKTOLIGHT_API ULevelGeneratorSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	ULevelGeneratorSettings();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Generator Settings")
	TArray<FRoomInfo> Rooms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Generator Settings")
	TSubclassOf<ARoom> RootRoomOverride;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Generator Settings")
	TArray<FRoomDecoratorInfo> RoomDecorators;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Generator Settings")
	TSubclassOf<URoomFactory> RoomFactoryClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Generator Settings")
	TSubclassOf<AConnector> ConnectorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Generator Settings")
	TArray<FLevelDecoratorInfo> LevelDecorators;

	// Utility function to check that everything in the settings class is valid.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level Generator Settings")
	bool Validate(FString& ErrorMessage, int MinRooms = 0, int MaxRooms = 2147483647);

	UFUNCTION(CallInEditor, Category = "Level Generator Settings")
	void CheckIsValid();
};
