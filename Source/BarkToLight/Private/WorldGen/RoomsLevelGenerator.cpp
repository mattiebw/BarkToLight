// copyright lolol

#include "WorldGen/RoomsLevelGenerator.h"

#include "BarkToLightLog.h"
#include "WorldGen/Connector.h"
#include "WorldGen/Room.h"
#include "WorldGen/RoomFactory.h"
#include "WorldGen/RoomsLevelGeneratorSettings.h"

ARoomsLevelGenerator::ARoomsLevelGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARoomsLevelGenerator::Destroyed()
{
	Super::Destroyed();
	// Destroy the level if the generator is destroyed, as the generator is the only thing which is keeping a reference to all the level objects.
	DestroyLevel();
}

void ARoomsLevelGenerator::Generate_Implementation()
{
	BTL_LOGC_NOLOC(GetWorld(), "Generating level...");
	double Start = FPlatformTime::Seconds();

	RoomsSettings = Cast<URoomsLevelGeneratorSettings>(Settings);
	if (RoomsSettings == nullptr)
	{
		BTL_LOGC_ERROR_NOLOC(
			GetWorld(),
			"In ALevelGenerator, Generate() called with invalid Settings (not URoomsLevelGeneratorSettings).");
		return;
	}

	// First, let's check that we have some settings.
	if (RoomsSettings == nullptr)
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
	if (!RoomsSettings->Validate(Error, RoomCount, RoomCount))
	{
		BTL_LOGC_ERROR_NOLOC(GetWorld(), "In ALevelGenerator, Generate() called with invalid Settings: %s", *Error);
		return;
	}

	// Before we begin generating, let's clear any previous level.
	DestroyLevel();

	// Create our room factory.
	RoomFactory = NewObject<URoomFactory>(this, RoomsSettings->RoomFactoryClass);

	// Copy the rooms array into a local copy, so we can modify the counts.
	Rooms = RoomsSettings->Rooms;

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
	FRoomNode *        Current = nullptr, *Prev = nullptr;

	while (RemainingHotPathRooms > 0)
	{
		// First, if we have a previous room, let's find the connection point that we're going to connect to and mark it as connected.
		if (Current == nullptr)
		{
			// We don't have a room at the moment, so just initialise to an empty node. We'll then use that as our root room/node.
			RootRoom = FRoomNode();
			Current  = &RootRoom;
		}
		else
		{
			// We have a previous room, so let's find a free connection point and mark it as connected.
			// We'll setup our current node to be the node we're connecting to.

			int OutIndex = Current->Actor->GetRandomFreeConnectionPointIndex(false, true, true);
			if (OutIndex == -1)
			{
				BTL_LOGC_ERROR_NOLOC(
					GetWorld(),
					"In LevelGenerator::Generate(), somehow we've created a room on the hot path that doesn't have any free outward connectors.");
				return;
			}
			Current->Actor->MarkConnectionConnected(OutIndex, ERoomConnectorType::Out);
			TotalAvailableOutputs--; // We've used up one of our outputs, so decrement the total available outputs.

			// Check to see if we have any free connectors, so we can record our room for future usage.
			if (Current->Actor->HasAnyFreeConnectionPoints(false, true, true))
				RoomsWithFreeOutwardConnectors.Add(Current);

			Prev    = Current;
			Current = &Current->Children[OutIndex];
		}

		// Now it's time to set up our new room.
		TSubclassOf<ARoom> RoomClass;
		if (RemainingHotPathRooms == HotPathLength && RoomsSettings->RootRoomOverride.Get())
		{
			// This is our root room, and we have a root room override, so we should use that instead.
			RoomClass = RoomsSettings->RootRoomOverride;
		}
		else
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

		// Build our room!
		RoomFactory->Reset()
		           ->CreateRoom(RoomClass)
		           ->AddDecoratorsFromInfos(RoomsSettings->RoomDecorators);

		Current->Actor = RoomFactory->Finish(); // Set the actor for this node.
		// Now, initialise this node's children with the correct number based on the amount of connectors we have.
		Current->Children.Init(FRoomNode(), Current->Actor->Connectors.Num());

		if (Prev != nullptr)
		{
			// Figure out which of our connectors was the inward connector.
			int InIndex = Current->Actor->GetRandomFreeConnectionPointIndex(true, false, true);
			if (InIndex == -1)
			{
				BTL_LOGC_ERROR_NOLOC(
					GetWorld(),
					"In LevelGenerator::Generate(), somehow we've created a room on the hot path that doesn't have any free inward connectors.");
				return;
			}
			Current->Actor->MarkConnectionConnected(InIndex, ERoomConnectorType::In);
			// We don't need to set the Current->Children[InIndex] node, as our graph structure doesn't need to go backwards.
			// We also don't need to mark it as connected in the graph structure, as it is marked connected in the room actor itself,
			// which is what we use to determine if a connection is connected or not, as it holds the FConnectors.
		}

		// Calculate our total available outputs now that we've finished setting up the room and its connectors.
		TotalAvailableOutputs += Current->Actor->GetTotalAvailableOutwardConnections();
		RemainingHotPathRooms--;
	}

	// Filter out any rooms that don't have any free outward connectors.
	RoomsWithFreeOutwardConnectors = RoomsWithFreeOutwardConnectors.FilterByPredicate([](FRoomNode* Node)
	{
		return Node->Actor->HasAnyFreeConnectionPoints(false, true, true);
	});

	RemainingRooms = RoomCount - HotPathLength;

	// Create remaining rooms
	while (RemainingRooms > 0)
	{
		// Find a random room that has free outward connectors.
		if (RoomsWithFreeOutwardConnectors.IsEmpty())
		{
			BTL_LOGC_ERROR(
				GetWorld(),
				"In LevelGenerator::Generate(), we've run out of rooms with free outward connectors before we've placed all of our rooms.");
			return;
		}

		// MW @todo @copypaste: This is a copy-paste of the code above, we should probably refactor this into a function.
		FRoomNode* Node = RoomsWithFreeOutwardConnectors[FMath::RandRange(0, RoomsWithFreeOutwardConnectors.Num() - 1)];
		int        OutIndex = Node->Actor->GetRandomFreeConnectionPointIndex(false, true, true);
		if (OutIndex == -1)
		{
			BTL_LOGC_ERROR(
				GetWorld(),
				"In LevelGenerator::Generate(), somehow we've selected a room with no free outward connectors.");
			return;
		}
		Node->Actor->MarkConnectionConnected(OutIndex, ERoomConnectorType::Out);
		TotalAvailableOutputs--; // We've used up one of our outputs, so decrement the total available outputs.
		// Also, since we've used up one of our outputs, we should remove this room from our list of rooms with free outward connectors if it doesn't have any left.
		if (!Node->Actor->HasAnyFreeConnectionPoints(false, true, true))
			RoomsWithFreeOutwardConnectors.Remove(Node);
		FRoomNode* NextNode = &Node->Children[OutIndex];

		FRoomInfo* NextRoom = nullptr;
		if (!GetNextRoom(NextRoom))
		{
			// We've failed to satisfy the conditions!
			BTL_LOGC_ERROR(GetWorld(), "Level generator failed to satisfy conditions - no valid rooms to place.");
			return;
		}
		NextNode->Actor = RoomFactory->Reset()
		                             ->CreateRoom(NextRoom->RoomClass)
		                             ->AddDecoratorsFromInfos(RoomsSettings->RoomDecorators)
		                             ->Finish();
		NextNode->Children.Init(FRoomNode(), NextNode->Actor->Connectors.Num());
		// Find the inward connector and mark it as connected.
		int InIndex = NextNode->Actor->GetRandomFreeConnectionPointIndex(true, false, true);
		if (InIndex == -1)
		{
			BTL_LOGC_ERROR(
				GetWorld(),
				"In LevelGenerator::Generate(), somehow we've created a room that doesn't have any free inward connectors.");
			return;
		}
		NextNode->Actor->MarkConnectionConnected(InIndex, ERoomConnectorType::In);
		// Check to see if this new room has any free outward connectors, so we can record it for future usage.
		if (NextNode->Actor->HasAnyFreeConnectionPoints(false, true, true))
			RoomsWithFreeOutwardConnectors.Add(NextNode);

		RemainingRooms--;
	}

	// Layout phase
	// Correct actor locations and rotations to prevent overlap
	// BFS through the graph, starting from the root room
	TQueue<FRoomNode*> ToBeProcessed;
	ToBeProcessed.Enqueue(&RootRoom);
	while (!ToBeProcessed.IsEmpty())
	{
		FRoomNode* Node = nullptr;
		ToBeProcessed.Dequeue(Node);

		// Position in the world.

		// For each connector:
		//	- Fix up references in child.
		//	- Create connector if needed.
		//  - Add children to the queue.

		for (int i = 0; i < Node->Children.Num(); i++)
		{
			FRoomNode* Child = &Node->Children[i];

			if (Child->Actor == nullptr)
				continue;


			ToBeProcessed.Enqueue(Child);
		}
	}

	// Run post decorators

	// Create player

	double End = FPlatformTime::Seconds();
	BTL_LOGC_NOLOC(GetWorld(), "Level generation took %f seconds.", End - Start);
}

void ARoomsLevelGenerator::DestroyLevel()
{
	for (auto Room : GeneratedRooms)
		Room->Destroy();
	GeneratedRooms.Empty();
	for (auto Connector : GeneratedConnectors)
		Connector->Destroy();
	GeneratedConnectors.Empty();
}

bool ARoomsLevelGenerator::GetNextRoom(FRoomInfo*& Output)
{
	if (Rooms.IsEmpty())
	{
		BTL_LOGC_ERROR_NOLOC(GetWorld(), "Level generator failed to satisfy conditions - ran out of rooms to place.");
		return false;
	}

	TArray<FRoomInfo*> ValidRooms;

	// MW @todo @speed: We should probably cache this somewhere, as we're doing this every time we want to get a room.
	for (auto& Room : Rooms)
	{
		if (Room.MaximumCount <= 0) // Skip rooms that have reached their maximum count.
			continue;

		if (RemainingHotPathRooms > 0 || TotalAvailableOutputs <= 1)
		{
			if (Room.RoomClass.GetDefaultObject()->RoomIsNotDeadEnd())
				ValidRooms.Add(&Room);
		}
		else
			ValidRooms.Add(&Room);
	}

	if (ValidRooms.Num() > 1)
		ValidRooms.Remove(LastSelectedRoom);

	if (!ValidRooms.IsEmpty())
		Output = ValidRooms[FMath::RandRange(0, ValidRooms.Num() - 1)];

	if (Output)
	{
		Output->MaximumCount--; // Decrement the maximum count of this room, so we don't place it too many times.
	}
	else
	{
		BTL_LOGC_ERROR_NOLOC(GetWorld(), "Level generator failed to satisfy conditions - no valid rooms to place.");
		// TODO: What do we do here?
		// For now, lets return a random room from the main array.
		// This is a temporary solution, and should be replaced with a more robust system.
		Output = &RoomsSettings->Rooms[FMath::RandRange(0, Rooms.Num() - 1)];
	}

	if (Output != nullptr)
		LastSelectedRoom = Output;

	return Output != nullptr;
}
