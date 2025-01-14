// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LootGenerator.generated.h"

class UInventoryComponent;
class ULootTable;

/**
 * This object is used to generate the loot for an inventory component (for example, for treasure chests.)
 */
UCLASS()
class BARKTOLIGHT_API ULootGenerator : public UObject
{
	GENERATED_BODY()

public:
	// Used to generate loot into an inventory component (such as a chest).
	UFUNCTION(BlueprintCallable, Category = Items)
	void GenerateLootIntoInventory(UInventoryComponent* InventoryComponent, float Luck);

	// The loot table to use for generating loot.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
	ULootTable* LootTable;
};
