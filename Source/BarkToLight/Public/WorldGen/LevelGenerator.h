﻿// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

struct FRoomInfo;
class URoomFactory;
class AConnector;
class ARoom;
class ULevelGeneratorSettings;

USTRUCT(BlueprintType)
struct FRoomNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	ARoom* Actor;
	
	// Blah, blueprints can't do struct recursion!
	// If we wanted to expose this to blueprints, we'd have to make some helper methods.
	// For now, we'll just force generators to be implemented in C++.
	TArray<FRoomNode> Children;

	FRoomNode()
	{
		Actor = nullptr;
		Children = TArray<FRoomNode>();
	}
};

UCLASS()
class BARKTOLIGHT_API ALevelGenerator : public AActor
{
	GENERATED_BODY()

public:
	ALevelGenerator();

	virtual void Destroyed() override;
	
	UFUNCTION(BlueprintCallable, Category = "Level Generator")
	void Generate();

	UFUNCTION(CallInEditor, Category = "Level Generator")
	void GenerateInEditor();

	UFUNCTION(BlueprintCallable, Category = "Level Generator")
	void DestroyLevel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Generator")
	ULevelGeneratorSettings* Settings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Generator")
	FVector2f HotPathRoomCountRange = FVector2f(15, 25);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Generator")
	FVector2f TotalRoomCountRange = FVector2f(30, 40);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Generator")
	FVector2f DifficultyIncreasePerRoomRange = FVector2f(-10, 50);

	UPROPERTY(BlueprintReadWrite, Category = "Level Generator")
	FRoomNode RootRoom;
	
protected:
	bool GetNextRoom(FRoomInfo*& Output);
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ARoom*> GeneratedRooms;
	UPROPERTY(BlueprintReadOnly)
	TArray<AConnector*> GeneratedConnectors;

	// Generator state
	TArray<FRoomInfo> Rooms;
	int RemainingHotPathRooms, RemainingRooms;

	UPROPERTY(BlueprintReadOnly)
	URoomFactory* RoomFactory;
};
