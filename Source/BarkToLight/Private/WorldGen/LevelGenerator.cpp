// copyright lolol

#include "WorldGen/LevelGenerator.h"

#include "BarkToLightLog.h"
#include "WorldGen/Connector.h"
#include "WorldGen/LevelGeneratorSettings.h"

ALevelGenerator::ALevelGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelGenerator::Destroyed()
{
	Super::Destroyed();
	// Destroy the level if the generator is destroyed, as the generator is the only thing which is keeping a reference to all the level objects.
	DestroyLevel();
}

void ALevelGenerator::Generate()
{
	// First, let's check that we have some settings.
	if (Settings == nullptr)
	{
		BTL_LOGC_ERROR_NOLOC(GetWorld(), "In ALevelGenerator, Generate() called with nullptr Settings.");
		return;
	}

	// Lets roll our random counts.
	int HotPathLength = FMath::RandRange(static_cast<int>(HotPathRoomCountRange.X),
	                                     static_cast<int>(HotPathRoomCountRange.Y));
	int RoomCount = FMath::RandRange(static_cast<int>(TotalRoomCountRange.X), static_cast<int>(TotalRoomCountRange.Y));

	// Validate some of our settings.
	// Our total room count needs to be at least as large as our hot path count.
	if (RoomCount < HotPathLength)
	{
		RoomCount = HotPathLength;
		BTL_LOGC_ERROR_NOLOC(
			GetWorld(),
			"In ALevelGenerator, Generate() rolled a total room count less than the hot path length. "
			"We've rounded up the total room count to accomodate, but you should check your ranges.");
	}

	// Now we can do our basic validation checks on the settings.
	FString Error;
	if (!Settings->Validate(Error, RoomCount, RoomCount))
	{
		BTL_LOGC_ERROR_NOLOC(GetWorld(), "In ALevelGenerator, Generate() called with invalid Settings: %s", *Error);
		return;
	}

	// Before we begin generating, let's clear any previous level.
	DestroyLevel();

	// Create our room factory.
	RoomFactory = NewObject<URoomFactory>(this, Settings->RoomFactoryClass);

	// Copy the rooms array into a local copy, so we can modify the counts.
	Rooms = Settings->Rooms;

	// We also want to figure out which of our rooms have outputs, so we don't use a dead end when that would ruin the generation.
	TArray<FRoomInfo*> RoomsWithOutputs;
	for (auto& Room : Rooms)
	{
		if (Room.RoomClass.GetDefaultObject()->RoomIsNotDeadEnd())
			RoomsWithOutputs.Add(&Room);
	}
	
	// Okay, now it's time to actually generate our level.
	// The algorithm goes something like this:
	// - Create a graph structure of our rooms using FRoomNode that satisfies our constraints (or errors out if that's not possible).
	//    - First generate our hot path, and once we have completed that, generate the other rooms.
	//    - For each generated room, create the actor and run our decorators.
	//    - TODO: Maybe we should move the decoration later.
	// - Layout our room actors such that they don't overlap.
	// - Create connectors between rooms.
	// - Run our level decorators.
	// - Done! Position the player.

	RemainingHotPathRooms = HotPathLength;
	TArray<FRoomNode*> RoomsWithFreeOutwardConnectors;
	FRoomNode* Current = nullptr;
	
	while (RemainingHotPathRooms > 0)
	{
		if (Current == nullptr)
		{
			RootRoom = FRoomNode();
			Current = &RootRoom;
		} else
		{
			int OutIndex = Current->Actor->GetRandomFreeConnectionPointIndex(false, true, true);
			if (OutIndex == -1)
				BTL_LOGC_ERROR_NOLOC(GetWorld(), "erm");
			Current->Actor->MarkConnectionConnected(OutIndex, ERoomConnectorType::Out);

			// Check to see if we have any free connectors, so we can record our room for future usage.
			if (Current->Actor->HasAnyFreeConnectionPoints(false, true, true))
				RoomsWithFreeOutwardConnectors.Add(Current);
			
			Current = &Current->Children[OutIndex];
		}

		TSubclassOf<ARoom> RoomClass;
		if (RemainingHotPathRooms == HotPathLength && Settings->RootRoomOverride.Get())
		{
			// This is our root room, and we have a root room override, so we should use that instead.
			RoomClass = Settings->RootRoomOverride;
		} else
		{
			// Otherwise, just use our next room function.
			FRoomInfo* NextRoom = nullptr;
			if (!GetNextRoom(NextRoom))
			{
				// We've failed to satisfy the conditions!
				// TODO: What do we do here?
				return;
			}
			RoomClass = NextRoom->RoomClass;
		}

		RoomFactory->Reset()
			->CreateRoom(RoomClass);
			// ->AddDecorators()

		Current->Children.Init(FRoomNode(), Current->Actor->Connectors.Num());
		Current->Actor = RoomFactory->GetRoom();
		
		RemainingHotPathRooms--;
	}

	RemainingRooms = RoomCount - HotPathLength;

	// Create remaining rooms
	while (RemainingRooms > 0)
	{
		// Find a random room that has free outward connectors.
		RemainingRooms--;
	}

	// Layout phase
	// Correct actor locations and rotations to prevent overlap

	// Create connectors

	// Run post decorators

	// Create player
}

void ALevelGenerator::GenerateInEditor()
{
	Generate();
}

void ALevelGenerator::DestroyLevel()
{
	for (auto Room : GeneratedRooms)
		Room->Destroy();
	GeneratedRooms.Empty();
	for (auto Connector : GeneratedConnectors)
		Connector->Destroy();
	GeneratedConnectors.Empty();
}

bool ALevelGenerator::GetNextRoom(FRoomInfo*& Output)
{
	if (Rooms.IsEmpty())
	{
		BTL_LOGC_ERROR_NOLOC(GetWorld(), "Level generator failed to satisfy conditions - ran out of rooms to place.");
		return false;
	}

	Output = &Rooms[0];
	
	return true;
}
