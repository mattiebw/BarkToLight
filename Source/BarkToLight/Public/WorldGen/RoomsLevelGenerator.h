// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "LevelGeneratorBase.h"
#include "Room.h"
#include "GameFramework/Actor.h"
#include "RoomsLevelGenerator.generated.h"

struct FRoomInfo;
class URoomFactory;
class AConnector;
class ARoom;
class URoomsLevelGeneratorSettings;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
	int Depth = 0;

	FRoomNode()
	{
		Actor = nullptr;
		Children = TArray<FRoomNode>();
	}

	FRoomNode(ARoom* Room)
	{
		Actor = Room;
		Children = TArray<FRoomNode>();
		Children.Init(FRoomNode(), Room->Connectors.Num());
	}
};

UCLASS()
class BARKTOLIGHT_API ARoomsLevelGenerator : public ALevelGeneratorBase
{
	GENERATED_BODY()

public:
	ARoomsLevelGenerator();

	virtual void Destroyed() override;

	virtual void Generate_Implementation() override;

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Level Generator")
	void DestroyLevel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Generator")
	FVector2f HotPathRoomCountRange = FVector2f(15, 25);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Generator")
	FVector2f TotalRoomCountRange = FVector2f(30, 40);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Generator")
	FVector2f DifficultyIncreasePerRoomRange = FVector2f(-10, 50);

	UPROPERTY(BlueprintReadWrite, Category = "Level Generator")
	FRoomNode RootRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Generator")
	bool bDrawDebug = false;
	
protected:
	bool GetNextRoom(FRoomInfo*& Output);
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ARoom*> GeneratedRooms;
	UPROPERTY(BlueprintReadOnly)
	TArray<AConnector*> GeneratedConnectors;

	// Generator state
	TArray<FRoomInfo> Rooms;
	int RemainingHotPathRooms, RemainingRooms, TotalAvailableOutputs;
	FRoomInfo* LastSelectedRoom;

	UPROPERTY(BlueprintReadOnly)
	URoomFactory* RoomFactory;
	
	UPROPERTY(BlueprintReadWrite, Category = "Level Generator")
	URoomsLevelGeneratorSettings* RoomsSettings;
};
