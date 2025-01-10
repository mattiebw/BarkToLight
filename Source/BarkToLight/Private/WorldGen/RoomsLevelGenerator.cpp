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
	GeneratedRooms.Reserve(RoomCount);

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
		// Lets decrement the remaining hot path rooms now, so we don't forget.
		RemainingHotPathRooms--;

		// First, lets find the room we're going to place.
		TSubclassOf<ARoom> NextRoomClass = nullptr;
		if (Current == nullptr && RoomsSettings->RootRoomOverride.Get() != nullptr)
		{
			// This is the first room and we have a root room override, so we'll use that.
			NextRoomClass = RoomsSettings->RootRoomOverride;
		}
		else
		{
			// We don't have a root room override, so we'll need to find a room that satisfies our conditions.
			// The GetNextRoom function will handle this for us.
			FRoomInfo* NextRoomInfo = nullptr;
			if (!GetNextRoom(NextRoomInfo))
			{
				// We've failed to satisfy the conditions!
				BTL_LOGC_ERROR(GetWorld(), "Level generator failed to satisfy conditions - no valid rooms to place.");
				return;
			}
			NextRoomClass = NextRoomInfo->RoomClass;
		}

		// Create the new room using our factory.
		ARoom* NewRoom = RoomFactory->Reset()
		                            ->CreateRoom(NextRoomClass)
		                            ->AddDecoratorsFromInfos(RoomsSettings->RoomDecorators)
		                            ->Finish();
		GeneratedRooms.Add(NewRoom);

		if (Current == nullptr)
		{
			// If current is null, this is the first room, so we don't need to connect to any previous room.
			// We can just set this room as the root room, update Current, and return.
			// Since RootRoom is a member of this class, it'll be kept alive and we can store a pointer to it.
			RootRoom = FRoomNode(NewRoom);
			Current  = &RootRoom;
			continue;
		}

		// Find the valid connecting points for the current room and our new room.
		int NewRoomInConnectorIndex = NewRoom->GetRandomFreeConnectionPointIndex(true, false, true);
		if (NewRoomInConnectorIndex == -1)
		{
			BTL_LOGC_ERROR(GetWorld(),
			               "In LevelGenerator::Generate(), somehow we've created a room that doesn't have any free inward connectors.");
			return;
		}

		int CurrentRoomOutConnectorIndex = Current->Actor->GetRandomFreeConnectionPointIndex(false, true, true);
		if (CurrentRoomOutConnectorIndex == -1)
		{
			BTL_LOGC_ERROR(GetWorld(),
			               "In LevelGenerator::Generate(), somehow we've selected a room with no free outward connectors.");
			return;
		}

		// Connect the rooms.
		Current->Actor->ConnectTo(CurrentRoomOutConnectorIndex, NewRoom, NewRoomInConnectorIndex);

		// Update the child of the current room, and then update current to be our new room.
		Current->Children[CurrentRoomOutConnectorIndex] = FRoomNode(NewRoom);
		Current                                         = &Current->Children[CurrentRoomOutConnectorIndex];
	}

	// Now we want to populate our arrays and variables that the rest of the generation algorithm will use.
	// We'll do a quick BFS through our graph to do this.
	TQueue<FRoomNode*> ToBeProcessed;
	ToBeProcessed.Enqueue(&RootRoom);
	FRoomNode* CurrentNode = nullptr;
	while (ToBeProcessed.Dequeue(CurrentNode))
	{
		if (CurrentNode->Actor == nullptr)
			continue;

		int AvailableConnectors = CurrentNode->Actor->GetTotalAvailableOutwardConnections();
		TotalAvailableOutputs += AvailableConnectors;
		if (AvailableConnectors > 0)
			RoomsWithFreeOutwardConnectors.Add(CurrentNode);

		// Enqueue all of the children, as part of the BFS.
		for (auto& Node : CurrentNode->Children)
		{
			ToBeProcessed.Enqueue(&Node);
		}
	}

	RemainingRooms = RoomCount - HotPathLength;

	// Create remaining rooms
	while (RemainingRooms > 0)
	{
		// Find a random room that has free outward connectors.
		if (RoomsWithFreeOutwardConnectors.IsEmpty())
		{
			BTL_LOGC_ERROR(GetWorld(),
			               "In LevelGenerator::Generate(), we've run out of rooms with free outward connectors before we've placed all of our rooms.");
			return;
		}

		FRoomNode* NodeToBranchFrom = RoomsWithFreeOutwardConnectors[FMath::RandRange(
			0, RoomsWithFreeOutwardConnectors.Num() - 1)];

		FRoomInfo* NextRoomInfo = nullptr;
		if (!GetNextRoom(NextRoomInfo))
		{
			BTL_LOGC_ERROR(GetWorld(), "Level generator failed to satisfy conditions - no valid rooms to place.");
			return;
		}

		ARoom* NewRoom = RoomFactory->Reset()
		                            ->CreateRoom(NextRoomInfo->RoomClass)
		                            ->AddDecoratorsFromInfos(RoomsSettings->RoomDecorators)
		                            ->Finish();
		GeneratedRooms.Add(NewRoom);

		int FromConnectorIndex = NodeToBranchFrom->Actor->GetRandomFreeConnectionPointIndex(false, true, true);
		if (FromConnectorIndex == -1)
		{
			BTL_LOGC_ERROR(GetWorld(),
			               "In LevelGenerator::Generate(), somehow we've selected a room with no free outward connectors.");
			return;
		}
		int ToConnectorIndex = NewRoom->GetRandomFreeConnectionPointIndex(true, false, true);
		if (ToConnectorIndex == -1)
		{
			BTL_LOGC_ERROR(GetWorld(),
			               "In LevelGenerator::Generate(), somehow we've created a room that doesn't have any free inward connectors.");
			return;
		}

		// Do the connection and update the children.
		NodeToBranchFrom->Actor->ConnectTo(FromConnectorIndex, NewRoom, ToConnectorIndex);
		NodeToBranchFrom->Children[FromConnectorIndex] = FRoomNode(NewRoom);

		// Update our arrays and variables.
		TotalAvailableOutputs--;
		if (!NodeToBranchFrom->Actor->HasAnyFreeConnectionPoints(false, true, true))
			RoomsWithFreeOutwardConnectors.Remove(NodeToBranchFrom);
		int AvailableConnectors = NewRoom->GetTotalAvailableOutwardConnections();
		TotalAvailableOutputs += AvailableConnectors;
		if (AvailableConnectors > 0)
			RoomsWithFreeOutwardConnectors.Add(&NodeToBranchFrom->Children[FromConnectorIndex]);

		RemainingRooms--;
	}

	// Layout phase
	// Correct actor locations and rotations to prevent overlap
	// BFS through the graph, starting from the root room
	ToBeProcessed.Empty(); // Ensure our queue from earlier is empty.
	CurrentNode = nullptr; // Reset our current node.
	ToBeProcessed.Enqueue(&RootRoom);
	while (ToBeProcessed.Dequeue(CurrentNode))
	{
		// For each connector:
		//  - Position the child correctly.
		//	- Create connector if needed.
		//  - Add children to the queue.

		for (int i = 0; i < CurrentNode->Children.Num(); i++)
		{
			FRoomNode* Child = &CurrentNode->Children[i];

			if (Child->Actor == nullptr)
				continue;

			// Position the child correctly.
			int ChildConnectorIndex = CurrentNode->Actor->Connectors[i].ConnectedToIndex;

			FTransform CurrentConnector = CurrentNode->Actor->GetTransform() +
				CurrentNode->Actor->Connectors[i].Offset;
			CurrentConnector.SetScale3D(FVector::OneVector);

			Child->Actor->SetActorTransform(CurrentConnector);

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
