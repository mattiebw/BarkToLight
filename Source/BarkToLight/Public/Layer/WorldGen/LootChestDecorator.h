// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "WorldGen/RoomDecorator.h"
#include "LootChestDecorator.generated.h"

class ALootChest;
class ULootGenerator;
class ULootTable;

/**
 * Basic decorator to place a loot chest somewhere within the room.
 */
UCLASS()
class BARKTOLIGHT_API ULootChestDecorator : public URoomDecorator
{
	GENERATED_BODY()

public:
	virtual void Decorate_Implementation() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Chest")
	ULootTable* LootTable = nullptr;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Chest")
	TSubclassOf<ALootChest> LootChestClass = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot Chest")
	TSubclassOf<ULootGenerator> LootGenerator = nullptr;
};
