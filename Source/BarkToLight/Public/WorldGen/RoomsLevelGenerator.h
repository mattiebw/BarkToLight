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

UCLASS()
class UBoundsChecker : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Bounds Checker")
	FORCEINLINE void AddBounds(const FBox& InBounds)
	{
		Bounds.Add(InBounds);
	}

	UFUNCTION(BlueprintCallable, Category = "Bounds Checker")
	FORCEINLINE void AddBoundsArray(const TArray<FBox>& InBounds)
	{
		Bounds.Append(InBounds);
	}

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Bounds Checker")
	FORCEINLINE bool BoundsOverlapAnyBounds(const FBox& B) const
	{
		for (const FBox& Box : Bounds)
		{
			if (Box.Intersect(B))
				return true;
		}

		return false;
	}

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FBox> Bounds;
};

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
		Actor    = nullptr;
		Children = TArray<FRoomNode>();
	}

	FRoomNode(ARoom* Room)
	{
		Actor    = Room;
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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level Generator")
	FORCEINLINE TArray<ARoom*>& GetGeneratedRooms() { return GeneratedRooms; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level Generator")
	FORCEINLINE TArray<AConnector*>& GetGeneratedConnectors() { return GeneratedConnectors; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level Generator")
	FORCEINLINE TArray<AActor*>& GetOtherGeneratedActors() { return OtherGeneratedActors; }

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
	UPROPERTY(BlueprintReadOnly)
	TArray<AActor*> OtherGeneratedActors;

	// Generator state
	TArray<FRoomInfo> Rooms;
	int               RemainingHotPathRooms, RemainingRooms, TotalAvailableOutputs;
	FRoomInfo*        LastSelectedRoom;

	UPROPERTY(BlueprintReadOnly)
	URoomFactory* RoomFactory;

	UPROPERTY(BlueprintReadWrite, Category = "Level Generator")
	URoomsLevelGeneratorSettings* RoomsSettings;
};
