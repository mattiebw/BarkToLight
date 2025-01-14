// copyright lolol

#include "WorldGen/RoomsLevelGenerator.h"

#include "BarkToLightLog.h"
#include "Components/SplineComponent.h"
#include "WorldGen/Connector.h"
#include "WorldGen/LevelDecorator.h"
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
	// - Run our level decorators, including nav-mesh bounds generation and player start placement.

	RemainingHotPathRooms = HotPathLength;
	TArray<FRoomNode*> RoomsWithFreeOutwardConnectors;
	FRoomNode *        Current    = nullptr, *Prev = nullptr;
	float              Difficulty = 0;

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

		if (Current == nullptr)
			Difficulty = 0; // Root room. No difficulty.
		else if (Current != nullptr && Difficulty == 0)
			Difficulty = BaseDifficulty; // First room with difficulty. Use base difficulty.
		else
			Difficulty += FMath::FRandRange(DifficultyIncreasePerRoomRange.X, DifficultyIncreasePerRoomRange.Y);

		// Create the new room using our factory.
		ARoom* NewRoom = RoomFactory->Reset()
		                            ->CreateRoom(NextRoomClass)
		                            ->SetDifficulty(Difficulty)
		                            ->AddDecoratorsFromInfos(RoomsSettings->RoomDecorators)
		                            ->Finish();
		NewRoom->SetActorLocation(FVector(0, 0, RoomsSettings->MinimumRoomZ));
		for (AActor* Part : NewRoom->Parts)
			Part->AddActorWorldOffset(FVector(0, 0, RoomsSettings->MinimumRoomZ));
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
		Current->Children[CurrentRoomOutConnectorIndex]       = FRoomNode(NewRoom);
		Current->Children[CurrentRoomOutConnectorIndex].Depth = Current->Depth + 1;
		Current                                               = &Current->Children[CurrentRoomOutConnectorIndex];
	}
	
	// If we have a final room, generate it now.
	if (RoomsSettings->FinalRoom)
	{
		ARoom* NewRoom = RoomFactory->Reset()
									->CreateRoom(RoomsSettings->FinalRoom)
									->Finish();
		GeneratedRooms.Add(NewRoom);

		// Find the valid connecting points for the current room and our new, last room.
		int NewRoomInConnectorIndex = NewRoom->GetRandomFreeConnectionPointIndex(true, false, true);
		if (NewRoomInConnectorIndex == -1)
		{
			BTL_LOGC_ERROR(GetWorld(),
						   "In LevelGenerator::Generate(), the final room doesn't have any free inward connectors.");
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
		Current->Children[CurrentRoomOutConnectorIndex]       = FRoomNode(NewRoom);
		Current->Children[CurrentRoomOutConnectorIndex].Depth = Current->Depth + 1;
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
		                            ->SetDifficulty(NodeToBranchFrom->Actor->Difficulty + FMath::FRandRange(
			                            DifficultyIncreasePerRoomRange.X, DifficultyIncreasePerRoomRange.Y))
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
		NodeToBranchFrom->Children[FromConnectorIndex]       = FRoomNode(NewRoom);
		NodeToBranchFrom->Children[FromConnectorIndex].Depth = NodeToBranchFrom->Depth + 1;

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
	UBoundsChecker* BoundsChecker = NewObject<UBoundsChecker>(this);
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
			// First, let's get the transforms of the connectors.
			int ChildConnectorIndex = CurrentNode->Actor->Connectors[i].ConnectedToIndex;
			check(Child->Actor->Connectors[ChildConnectorIndex].ConnectedRoom == CurrentNode->Actor && Child->Actor->
				Connectors[ChildConnectorIndex].ConnectedToIndex == i);
			FTransform ConnectorTF = CurrentNode->Actor->Connectors[i].Offset * CurrentNode->Actor->GetTransform();
			FTransform ChildTF = Child->Actor->Connectors[ChildConnectorIndex].Offset * Child->Actor->GetTransform();

			// Draw some debug graphics to show the connectors.
			if (bDrawDebug)
			{
				DrawDebugSphere(GetWorld(), ConnectorTF.GetLocation(), 50.0f, 12, FColor::Green, false, 10.0f);
				DrawDebugString(GetWorld(), ConnectorTF.GetLocation() + FVector(0, 0, 100),
				                FString::Printf(TEXT("%d"), CurrentNode->Depth), nullptr,
				                FColor::Green, 10.0f, true);
			}

			// Find the rotation such that the forward vector of the child connector is opposite that of the parent connector, plus a random offset.
			// This took so, so long to figure out - I need to learn more about quaternions.
			FRotator OrigRot         = Child->Actor->GetActorRotation();
			FVector  CurrentRotation = ConnectorTF.Rotator().RotateVector(FVector::ForwardVector);
			FVector  ChildRotation   = ChildTF.Rotator().RotateVector(FVector::ForwardVector);
			FQuat    DeltaRotation   = FQuat::FindBetweenNormals(ChildRotation, -CurrentRotation);
			Child->Actor->AddActorWorldRotation(DeltaRotation);
			Child->Actor->AddActorWorldRotation(FRotator(0, FMath::RandRange(
				                                             CurrentNode->Actor->Connectors[i].RandomRotationOffsetRange
				                                             .X,
				                                             CurrentNode->Actor->Connectors[i].RandomRotationOffsetRange
				                                             .Y), 0));
			FRotator NewRot = Child->Actor->GetActorRotation();
			for (AActor* Part : Child->Actor->Parts)
			{
				FVector Offset = Part->GetActorLocation() - Child->Actor->GetActorLocation();
				Part->AddActorWorldOffset(-Offset);
				Part->AddActorWorldOffset((NewRot - OrigRot).RotateVector(Offset));
				Part->AddActorWorldRotation(DeltaRotation);
			}

			// Find a position for the child that doesn't overlap with any other rooms.
			// First, recalculate the transform, as we've changed the rotation.
			// We'll use the direction of the connector to determine the direction we should move.
			// Then, we'll move in that direction and 0, 30, and -30 degrees of yaw variation to try and find a free spot.
			// We'll move the amount of units seperating the connectors, and if we can't find a free spot, we'll increase by 500 units.
			// We'll do this iteration 20 times, and if we can't find a free spot, we'll just give up.
			ChildTF = Child->Actor->Connectors[ChildConnectorIndex].Offset * Child->Actor->GetTransform();
			FTransform CurrentNodeTF = CurrentNode->Actor->GetTransform();
			// MW: I thought just FVector() would work, but it looks like it can be uninitialized!
			CurrentNodeTF.SetLocation(FVector(0, 0, 0));
			FTransform ParentTF    = CurrentNode->Actor->Connectors[i].Offset * CurrentNodeTF;
			FBox       ChildBounds = Child->Actor->GetComponentsBoundingBox();
			FVector    Direction   = ParentTF.Rotator().RotateVector(FVector::ForwardVector);
			FVector    LeftDir     = FRotator(0, 30, 0).RotateVector(Direction);
			FVector    RightDir    = FRotator(0, -30, 0).RotateVector(Direction);
			// Find the distance between the two connectors, and use that as the starting magnitude.
			float   StartingMagnitude = (ChildTF.GetLocation() - ParentTF.GetLocation()).Size() * 0.8f;
			FVector NewLocation;
			FVector OrigLocation = Child->Actor->GetActorLocation();
			for (int j = 1; j <= 20; j++)
			{
				// If we were going -30, 0, 30, we could probably do this in a loop, but I want to go 0, 30, -30.
				// We could make an array and iterate over that, but it's probably just easiest to just unroll the loop.

				NewLocation = ConnectorTF.GetLocation() + Direction * (StartingMagnitude + (500 * j));
				Child->Actor->SetActorLocation(NewLocation);
				ChildBounds = Child->Actor->GetComponentsBoundingBox();
				if (!BoundsChecker->BoundsOverlapAnyBounds(ChildBounds))
					break;

				NewLocation = ConnectorTF.GetLocation() + LeftDir * (StartingMagnitude + (500 * j));
				Child->Actor->SetActorLocation(NewLocation);
				ChildBounds = Child->Actor->GetComponentsBoundingBox();
				if (!BoundsChecker->BoundsOverlapAnyBounds(ChildBounds))
					break;

				NewLocation = ConnectorTF.GetLocation() + RightDir * (StartingMagnitude + (500 * j));
				Child->Actor->SetActorLocation(NewLocation);
				ChildBounds = Child->Actor->GetComponentsBoundingBox();
				if (!BoundsChecker->BoundsOverlapAnyBounds(ChildBounds))
					break;
			}
			FVector Diff = Child->Actor->GetActorLocation() - OrigLocation;
			for (AActor* Part : Child->Actor->Parts)
				Part->AddActorWorldOffset(Diff);

			// Now that we have positioned the child, we can debug draw the new connector locations.
			if (bDrawDebug)
			{
				ChildTF = Child->Actor->Connectors[ChildConnectorIndex].Offset * Child->Actor->GetTransform();
				DrawDebugSphere(GetWorld(), ChildTF.GetLocation(), 50.0f, 12, FColor::Red, false, 10.0f);
				DrawDebugString(GetWorld(), ChildTF.GetLocation() + FVector(0, 0, 100),
				                FString::Printf(TEXT("%d"), CurrentNode->Depth), nullptr,
				                FColor::Red, 10.0f, true);
			}

			BoundsChecker->AddBounds(Child->Actor->GetComponentsBoundingBox());
			// Add the bounds of the child to the bounds checker, with some padding.

			// Finally, generate the connector between the two rooms.
			AConnector* Connector = GetWorld()->SpawnActor<AConnector>(RoomsSettings->ConnectorClass);
			Connector->CreateSplineFromRooms(CurrentNode->Actor, i, Child->Actor, ChildConnectorIndex, BoundsChecker);
			if (!Connector->IsValidConnector())
				Connector->Destroy();
			else
			{
				Connector->Generate_Implementation();
				GeneratedConnectors.Add(Connector);
				BoundsChecker->AddBounds(Connector->GetComponentsBoundingBox());
			}

			ToBeProcessed.Enqueue(Child);
		}
	}

	// Run post decorators
	for (auto LevelDec : RoomsSettings->LevelDecorators)
	{
		if (FMath::FRand() > LevelDec.Chance)
			continue;

		ULevelDecorator* Decorator = NewObject<ULevelDecorator>(this, LevelDec.DecoratorClass);
		Decorator->Level           = this;
		Decorator->Decorate();
		Decorator->ConditionalBeginDestroy();
	}

	BoundsChecker->ConditionalBeginDestroy();

	double End = FPlatformTime::Seconds();
	BTL_LOGC_NOLOC(GetWorld(), "Level generation took %f seconds.", End - Start);
}

void ARoomsLevelGenerator::DestroyLevel()
{
	for (auto Room : GeneratedRooms)
		if (Room) Room->Destroy();
	GeneratedRooms.Empty();
	for (auto Connector : GeneratedConnectors)
		if (Connector) Connector->Destroy();
	GeneratedConnectors.Empty();
	for (auto Actor : OtherGeneratedActors)
		if (Actor) Actor->Destroy();
	OtherGeneratedActors.Empty();

	Rooms.Empty();
	LastSelectedRoom = nullptr;
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
		// MW @todo: What do we do here?
		// For now, lets return a random room from the main array.
		// This is a temporary solution, and should be replaced with a more robust system.
		Output = &RoomsSettings->Rooms[FMath::RandRange(0, Rooms.Num() - 1)];
	}

	if (Output != nullptr)
		LastSelectedRoom = Output;

	return Output != nullptr;
}
