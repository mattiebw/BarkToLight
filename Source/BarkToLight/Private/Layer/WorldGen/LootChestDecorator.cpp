// copyright lolol

#include "Layer/WorldGen/LootChestDecorator.h"

#include "Core/Items/LootGenerator.h"
#include "Layer/WorldGen/LootChest.h"
#include "WorldGen/Room.h"

void ULootChestDecorator::Decorate_Implementation()
{
#if 0
	// First, find the bounds of the room.
	FVector Center, Extents;
	Room->GetActorBounds(true, Center, Extents);
	
	// Since all the rooms are in one place when we run this, we need to make it such that only this room will be
	// detected by the line trace. Therefore, we set only this room to block the ECC_CurrentRoom channel.
	TArray<UPrimitiveComponent*> ComponentsForCollision;
	Room->GetComponents<UPrimitiveComponent>(ComponentsForCollision);
	for (UPrimitiveComponent* Component : ComponentsForCollision)
	{
		Component->SetCollisionResponseToChannel(ECC_CurrentRoom, ECR_Block);
	}

	// Try 100 times to find a place to put the loot chest.
	// MW @todo: We should look at having spawn points or something similar to make this more deterministic, instead of
	// brute forcing a random spot (that may not be free!)
	FHitResult Hit;
	int Tries = 100;
	do
	{
		FVector Start = Center + FVector(FMath::FRandRange(-Extents.X, Extents.X), FMath::FRandRange(-Extents.Y, Extents.Y), Extents.Z);
		GetWorld()->LineTraceSingleByChannel(Hit, Start, Start - FVector(0, 0, Extents.Z * 2), ECC_CurrentRoom);
		DrawDebugLine(GetWorld(), Start, Start - FVector(0, 0, Extents.Z * 2), Hit.bBlockingHit ? FColor::Green : FColor::Red, false, 5.0f);
	} while (!Hit.bBlockingHit && Tries-- > 0);

	// Reset the collision responses.
	for (UPrimitiveComponent* Component : ComponentsForCollision)
	{
		Component->SetCollisionResponseToChannel(ECC_CurrentRoom, ECR_Ignore);
	}
	
	// If we didn't find one, error and return.
	if (Tries <= 0)
	{
		BTL_LOGC_WARN(GetWorld(), "Failed to place loot chest in room %s", *Room->GetName());
		return;
	}
#endif

	FVector SpawnLoc = Room->GetActorTransform().TransformVector(Room->ChestSpawnLocations[FMath::RandRange(0, Room->ChestSpawnLocations.Num() - 1)]);

	// Otherwise, spawn the loot chest and move it up by its Z extent so it's on the floor.
	ALootChest* LootChest = GetWorld()->SpawnActor<ALootChest>(LootChestClass, SpawnLoc, FRotator::ZeroRotator);

	// Generate the loot.
	ULootGenerator* Generator = NewObject<ULootGenerator>(LootChest, LootGenerator);
	if (LootTable)
		Generator->LootTable = LootTable;
	Generator->GenerateLootIntoInventory(LootChest->GetInventoryComponent(), Room->Difficulty * 2);

	// Add the loot chest to the room.
	Room->Parts.Add(LootChest);

	// Destroy our generator.
	Generator->ConditionalBeginDestroy();
}
