// copyright lolol

#include "Layer/WorldGen/PlaceDogDecorator.h"

#include "Core/Dog.h"
#include "WorldGen/RoomsLevelGenerator.h"

void UPlaceDogDecorator::Decorate()
{
	// Perform a BFS to find the room with the highest difficulty.
	TQueue<FRoomNode*> ToBeProcessed;
	ToBeProcessed.Enqueue(&Level->RootRoom);
	FRoomNode* CurrentNode = nullptr;
	FRoomNode* HighestDifficultyNode = nullptr;
	float HighestDifficulty = -1;
	while (ToBeProcessed.Dequeue(CurrentNode))
	{
		if (!CurrentNode->Actor)
			continue;
		
		if (CurrentNode->Actor->Difficulty > HighestDifficulty)
		{
			HighestDifficulty = CurrentNode->Actor->Difficulty;
			HighestDifficultyNode = CurrentNode;
		}

		for (auto& Node : CurrentNode->Children)
		{
			ToBeProcessed.Enqueue(&Node);
		}
	}
	
	// Place the dog in the room.
	ARoom* Room = HighestDifficultyNode->Actor;
	FVector Center, Extent;
	Room->GetActorBounds(true, Center, Extent);
	FHitResult Hit;
	do
	{
		FVector Start = Center + FVector(0, 0, Extent.Z);
		GetWorld()->LineTraceSingleByChannel(Hit, Start, Start - FVector(0, 0, Extent.Z * 2), ECC_WorldStatic);
	} while (!Hit.bBlockingHit);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ADog* Dog = GetWorld()->SpawnActor<ADog>(DogSubclass, Hit.Location, FRotator::ZeroRotator, SpawnParameters);
	Dog->GetActorBounds(true, Center, Extent);
	Dog->AddActorWorldOffset(FVector(0, 0, Extent.Z));
	Room->Parts.Add(Dog);
}
